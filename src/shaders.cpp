// Ejemplo mínimo de código OpenGL, usa OpenGL 3.3 + GLSL 3.3

#include <iostream>
#include <iomanip>

#include "shaders.h"

GLchar  ProgramObject::buffer[ buffer_length ] ;
GLsizei ProgramObject::report_length ;  

// ---------------------------------------------------------------------------------------------
// Clase: Program Object 

ProgramObject::ProgramObject()
{

}
// ---------------------------------------------------------------------------------------------

GLuint ProgramObject::createCompileAttachShader
(  
   GLenum       shader_type,         // one of: GL_VERTEX_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER
   const char * shader_description,  // text description for error log ('vertex shader', 'fragment shader', etc...)
   const char * shader_source        // source string
)
{  using namespace std ;
   assert( shader_type == GL_VERTEX_SHADER || 
           shader_type == GL_GEOMETRY_SHADER || 
           shader_type == GL_FRAGMENT_SHADER ) ;

   assert( shader_source != nullptr );
   assert( program_id > 0 );

   assert( glGetError() == GL_NO_ERROR );

   const GLuint shader_id     = glCreateShader( shader_type );
   GLint        source_length = strlen( shader_source );

   glShaderSource( shader_id, 1, (const GLchar **) &shader_source, &source_length ) ;
   glCompileShader( shader_id ) ;

   glGetShaderInfoLog( shader_id, buffer_length, &report_length, buffer );
   if ( report_length > 0 )
   {
      cout << shader_description << " compilation log: " << endl ;
      cout << buffer << endl ;
   }

   GLint compile_status ;
   glGetShaderiv( shader_id, GL_COMPILE_STATUS, &compile_status );

   if ( compile_status != GL_TRUE )
   {
      cout << "Compilation errors, aborting." << endl ;
      exit(1);
   }

   glAttachShader( program_id, shader_id );
   assert( glGetError() == GL_NO_ERROR );
   return shader_id ;
}
// ---------------------------------------------------------------------------------------------
// Gets uniform location + warns if it is not active.

GLint ProgramObject::getLocation( const char * name )
{
   using namespace std ;
   assert( name != nullptr );
   assert( program_id > 0 );

   const GLint location = glGetUniformLocation( program_id, name ); 

   if ( location == -1 )
      cout << "Warning: uniform '" << name << "' is not declared or not used." << endl ;
   
   return location ;
}
// ---------------------------------------------------------------------------------------------
// Creates, compiles and uses (activates) the program object

void ProgramObject::createCompileUseProgram( )
{
   // check preconditions
   using namespace std ;
   assert( vertex_shader_source != nullptr );
   assert( fragment_shader_source != nullptr );
   assert( program_id == 0 );
   assert( glGetError() == GL_NO_ERROR );
   
   // create program, ceate and compile shaders, link program
   program_id = glCreateProgram() ;  assert( program_id > 0 );
   createCompileAttachShader( GL_VERTEX_SHADER,   "vertex shader",   vertex_shader_source );
   createCompileAttachShader( GL_FRAGMENT_SHADER, "fragment shader", fragment_shader_source );
   if ( geometry_shader_source != nullptr )
      createCompileAttachShader( GL_GEOMETRY_SHADER, "geometry shader", geometry_shader_source );
   glLinkProgram( program_id ) ;

   // check for link errors
   GLint estado_prog ;
   glGetProgramiv( program_id, GL_LINK_STATUS, &estado_prog );
   if ( estado_prog != GL_TRUE )
   {  cout << "Program link error, log:" << endl ;
      glGetProgramInfoLog( program_id, buffer_length, &report_length, buffer );
      cout << buffer << endl ;
      exit(1);
   }
   
   // use program
   glUseProgram( program_id );
   assert( glGetError() == GL_NO_ERROR );
   cout << "Program object created." << endl ; 
}

// ---------------------------------------------------------------------------------------------
// use (activate) this program object for following draw operations

   
void ProgramObject::use()
{
   assert( program_id > 0 );
   assert( glGetError() == GL_NO_ERROR );
   glUseProgram( program_id );
   assert( glGetError() == GL_NO_ERROR );
}

// ---------------------------------------------------------------------------------------------
// Basic program object source

const char * const basic_vertex_shader_source = R"glsl(
   #version 330 core

   // Parámetros uniform (variables de entrada iguales para todos los vértices en cada primitiva)

   uniform mat4 u_mat_modelview;  // variable uniform: matriz de transformación de posiciones
   uniform mat4 u_mat_proyeccion; // variable uniform: matriz de proyección
   uniform bool u_usar_color_plano; // 1 --> usar color plano, 0 -> usar color interpolado

   // Atributos de vértice (variables de entrada distintas para cada vértice)
   // (las posiciones de posición siempre deben estar en la 'location' 0)

   layout( location = 0 ) in vec3 atrib_posicion ; // atributo 0: posición del vértice
   layout( location = 1 ) in vec3 atrib_color ;    // atributo 1: color RGB del vértice

   // output variables, going to the geometry shader

   out      vec3 var_color_interpolado ; // color RGB del vértice (el mismo que proporciona la aplic.)
   flat out vec3 var_color_plano  ; // color RGB del 'provoking vertex'

   // función principal que se ejecuta una vez por vértice
   void main()
   {
      // copiamos color recibido en el color de salida, tal cual
      var_color_interpolado = atrib_color ;
      var_color_plano       = atrib_color ;

      // calcular las posiciones del vértice en posiciones de mundo y escribimos 'gl_Position'
      // (se calcula multiplicando las cordenadas por la matrices 'modelview' y 'projection')
      gl_Position = u_mat_proyeccion * u_mat_modelview * vec4( atrib_posicion, 1);
   }
)glsl";

// ------------------------------------------------------------------------------------------------------

const char * const basic_fragment_shader_source = R"glsl(
    #version 330 core

   uniform bool u_usar_color_plano;     // false -> usar color interpolado, true --> usar color plano, 
   in      vec3 var_color_interpolado ; // color interpolado en el pixel.
   flat in vec3 var_color_plano ;       // color (plano) producido por el 'provoking vertex'
   layout( location = 0 ) out vec4 out_color_fragmento ; // variable de salida (color del pixel)
   
   void main()
   {
      if ( u_usar_color_plano )
         out_color_fragmento = vec4( var_color_plano, 1.0 ) ; // el color del pixel es el color interpolado
      else 
         out_color_fragmento = vec4( var_color_interpolado, 1.0 ); // el color plano (de un único vértice)
   }
)glsl";



// ----------------------------------------------------------------------------------------------------

BasicProgramObject::BasicProgramObject()
{
   // set shaders sources pointers
   vertex_shader_source   = basic_vertex_shader_source ;
   fragment_shader_source = basic_fragment_shader_source ;
   //geometry_shader_source = basic_geometry_shader_source ; // optional (may be null) - just testing

   // create all the shaders:
   createCompileUseProgram();

   // get uniforms locations
   modelview_mat_loc    = getLocation( "u_mat_modelview" );      assert( modelview_mat_loc  != -1 );
   projection_mat_loc   = getLocation( "u_mat_proyeccion" );     assert( projection_mat_loc != -1 );
   use_flat_color_loc   = getLocation( "u_usar_color_plano" );   assert( use_flat_color_loc != -1 );
   
   // done.
   using namespace std ;
   cout << "Basic program object created with no errors." << endl ;
}
// ---------------------------------------------------------------------------------------------

void BasicProgramObject::setColor( const glm::vec3 & new_color )
{
   color = new_color ;
   glVertexAttrib3f(	ind_atrib_colores, color.r, color.g, color.b );
}
// ---------------------------------------------------------------------------------------------

void BasicProgramObject::setUseFlatColor( const bool new_use_flat_color )
{
   assert( use_flat_color_loc != -1 ); 
   assert( glGetError() == GL_NO_ERROR );
   glUniform1i( use_flat_color_loc, new_use_flat_color );
   assert( glGetError() == GL_NO_ERROR );
}
// ---------------------------------------------------------------------------------------------

void BasicProgramObject::setProjectionMatrix( const glm::mat4 & new_projection_mat )
{
   assert( projection_mat_loc != -1 ); 
   assert( glGetError() == GL_NO_ERROR );
   projection_mat = new_projection_mat ;
   glUniformMatrix4fv( projection_mat_loc, 1, GL_FALSE, glm::value_ptr(projection_mat) );
   assert( glGetError() == GL_NO_ERROR );
}
// ---------------------------------------------------------------------------------------------

void BasicProgramObject::resetMM()
{
   assert( modelview_mat_loc != -1 );  
   assert( glGetError() == GL_NO_ERROR );
   modelview_mat = glm::mat4( 1.0f );
   modelview_mat_stack.clear();
   glUniformMatrix4fv( modelview_mat_loc, 1, GL_FALSE, glm::value_ptr(modelview_mat) );
   assert( glGetError() == GL_NO_ERROR );
}
// ---------------------------------------------------------------------------------------------

void BasicProgramObject::pushMM()
{
   modelview_mat_stack.push_back( modelview_mat );
}
// ---------------------------------------------------------------------------------------------

void BasicProgramObject::compMM( const glm::mat4 & mat )
{
   assert( modelview_mat_loc >= 0 );
   modelview_mat = modelview_mat * mat ;
   glUniformMatrix4fv( modelview_mat_loc, 1, GL_FALSE, glm::value_ptr(modelview_mat) );
}
// ---------------------------------------------------------------------------------------------

void BasicProgramObject::popMM()
{
   assert( modelview_mat_loc >= 0 );
   assert( modelview_mat_stack.size() > 0 );
   modelview_mat = modelview_mat_stack[ modelview_mat_stack.size()-1 ] ;
   modelview_mat_stack.pop_back();
   glUniformMatrix4fv( modelview_mat_loc, 1, GL_FALSE, glm::value_ptr(modelview_mat) );
}
// --------------------------------------------------------------------------------------------

