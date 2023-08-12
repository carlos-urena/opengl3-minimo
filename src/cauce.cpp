// Ejemplo mínimo de código OpenGL, usa OpenGL 3.3 + GLSL 3.3

#include <iostream>
#include <iomanip>

#include "cauce.h"

// ---------------------------------------------------------------------------------------------


GLchar  Cauce::buffer[ buffer_length ] ;
GLsizei Cauce::report_length ; 

// ---------------------------------------------------------------------------------------------
// Basic pipeline shaders sources

const char * const fuente_vertex_shader = R"glsl(
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

const char * const fuente_fragment_shader = R"glsl(
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

// ---------------------------------------------------------------------------------------------

Cauce::Cauce()
{
   using namespace std ;

   crearObjetoPrograma();
   inicializarUniforms();
   imprimeInfoUniforms();

   cout << "Cauce creado sin errores." << endl ;
}

// ---------------------------------------------------------------------------------------------

void Cauce::inicializarUniforms()
{
   modelview_mat_loc  = leerLocation( "u_mat_modelview" );      
   projection_mat_loc = leerLocation( "u_mat_proyeccion" );     
   use_flat_color_loc = leerLocation( "u_usar_color_plano" );     
}
// ---------------------------------------------------------------------------------------------

void Cauce::imprimeInfoUniforms()
{
   using namespace std ;
   assert( 0 < id_prog );
   assert( glGetError() == GL_NO_ERROR );

   GLint n_uniforms;
   glGetProgramiv( id_prog, GL_ACTIVE_UNIFORMS, &n_uniforms );
   cout << "Cuenta de parámetros uniform activos: " << n_uniforms << endl ;

   for ( int i = 0; i < n_uniforms; i++ )
   {
      GLenum tipo;       // tipo de la variable (float, vec3 or mat4, etc)  
      GLint  n_entradas; // si es array, número de entradas, en otro caso 1.
      glGetActiveUniform( id_prog, (GLuint)i, buffer_length, &report_length, &n_entradas, &tipo, buffer);
      cout << "   Uniform " << i << ": " << buffer << " (" << NombreTipoGL(tipo) << " x" << n_entradas << ")." << endl ;
   }
   assert( glGetError() == GL_NO_ERROR );
}

// ---------------------------------------------------------------------------------------------

GLuint Cauce::compilarAdjuntarShader
(  
   GLenum       shader_type,         // uno de GL_VERTEX_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER
   const char * shader_description,  // texto descriptivo por si hay error ('vertex shader', 'fragment shader', etc...)
   const char * shader_source        // código fuente del shader
)
{  using namespace std ;
   assert( shader_type == GL_VERTEX_SHADER || 
           shader_type == GL_GEOMETRY_SHADER || 
           shader_type == GL_FRAGMENT_SHADER ) ;

   assert( shader_source != nullptr );
   assert( id_prog > 0 );

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

   glAttachShader( id_prog, shader_id );
   assert( glGetError() == GL_NO_ERROR );
   return shader_id ;
}
// ---------------------------------------------------------------------------------------------
// Gets uniform location + warns if it is not active.

GLint Cauce::leerLocation( const char * name )
{
   using namespace std ;
   assert( name != nullptr );
   assert( id_prog > 0 );

   const GLint location = glGetUniformLocation( id_prog, name ); 

   if ( location == -1 )
      cout << "Warning: uniform '" << name << "' is not declared or not used." << endl ;
   
   return location ;
}
// ---------------------------------------------------------------------------------------------

void Cauce::crearObjetoPrograma( )
{
   // check preconditions
   using namespace std ;
   assert( fuente_vertex_shader != nullptr );
   assert( fuente_fragment_shader != nullptr );
   assert( id_prog == 0 );
   assert( glGetError() == GL_NO_ERROR );
   
   // crear el programa, compilar los shaders
   id_prog = glCreateProgram() ;  assert( id_prog > 0 );
   id_frag_shader = compilarAdjuntarShader( GL_VERTEX_SHADER,   "vertex shader",   fuente_vertex_shader );
   id_vert_shader = compilarAdjuntarShader( GL_FRAGMENT_SHADER, "fragment shader", fuente_fragment_shader );
   

   // enlazar el programa y ver si ha habido errores
   GLint estado_prog ;
   glLinkProgram( id_prog ) ;   
   assert( glGetError() == GL_NO_ERROR );
   
   glGetProgramiv( id_prog, GL_LINK_STATUS, &estado_prog );
   if ( estado_prog != GL_TRUE )
   {  
      cout << "Program link error, log:" << endl ;
      glGetProgramInfoLog( id_prog, buffer_length, &report_length, buffer );
      cout << buffer << endl ;
      exit(1);
   }
   
   // activar (usar) el programa
   glUseProgram( id_prog );
   assert( glGetError() == GL_NO_ERROR );
   cout << "El objeto programa se ha creado sin problemas." << endl ; 
}

// ---------------------------------------------------------------------------------------------
// use (activate) this program object for following draw operations

   
void Cauce::activar()
{
   assert( id_prog > 0 );
   assert( glGetError() == GL_NO_ERROR );
   glUseProgram( id_prog );
   assert( glGetError() == GL_NO_ERROR );
}



// ----------------------------------------------------------------------------------------------------


// ---------------------------------------------------------------------------------------------

void Cauce::setColor( const glm::vec3 & new_color )
{
   color = new_color ;
   glVertexAttrib3f(	ind_atrib_colors, color.r, color.g, color.b );
}
// ---------------------------------------------------------------------------------------------

void Cauce::setUseFlatColor( const bool new_use_flat_color )
{
   assert( use_flat_color_loc != -1 ); 
   assert( glGetError() == GL_NO_ERROR );
   glUniform1i( use_flat_color_loc, new_use_flat_color );
   assert( glGetError() == GL_NO_ERROR );
}
// ---------------------------------------------------------------------------------------------

void Cauce::setProjectionMatrix( const glm::mat4 & new_projection_mat )
{
   assert( projection_mat_loc != -1 ); 
   assert( glGetError() == GL_NO_ERROR );
   projection_mat = new_projection_mat ;
   glUniformMatrix4fv( projection_mat_loc, 1, GL_FALSE, glm::value_ptr(projection_mat) );
   assert( glGetError() == GL_NO_ERROR );
}
// ---------------------------------------------------------------------------------------------

void Cauce::resetMM()
{
   assert( modelview_mat_loc != -1 );  
   assert( glGetError() == GL_NO_ERROR );
   modelview_mat = glm::mat4( 1.0f );
   modelview_mat_stack.clear();
   glUniformMatrix4fv( modelview_mat_loc, 1, GL_FALSE, glm::value_ptr(modelview_mat) );
   assert( glGetError() == GL_NO_ERROR );
}
// ---------------------------------------------------------------------------------------------

void Cauce::pushMM()
{
   modelview_mat_stack.push_back( modelview_mat );
}
// ---------------------------------------------------------------------------------------------

void Cauce::compMM( const glm::mat4 & mat )
{
   assert( modelview_mat_loc >= 0 );
   modelview_mat = modelview_mat * mat ;
   glUniformMatrix4fv( modelview_mat_loc, 1, GL_FALSE, glm::value_ptr(modelview_mat) );
}
// ---------------------------------------------------------------------------------------------

void Cauce::popMM()
{
   assert( modelview_mat_loc >= 0 );
   assert( modelview_mat_stack.size() > 0 );
   modelview_mat = modelview_mat_stack[ modelview_mat_stack.size()-1 ] ;
   modelview_mat_stack.pop_back();
   glUniformMatrix4fv( modelview_mat_loc, 1, GL_FALSE, glm::value_ptr(modelview_mat) );
}
// --------------------------------------------------------------------------------------------
