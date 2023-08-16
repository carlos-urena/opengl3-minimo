// Shader program object wrapper class

#ifndef CAUCE_H
#define CAUCE_H

#include <vector>
#include "glincludes.h"


// ****************************************************************************************

// Clase para el cauce de funcionalidad programable (OpenGL 3.3 o superior)
//
class Cauce
{
   public: 

   // crea un objeto cauce vacío
   Cauce() ;

   // compila un shader y lo adjunta al objeto programa 'id_prog' (que debe ser >0)
   //
   // @param shader_type        (GLenum) one of: GL_VERTEX_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER
   // @param shader_description (const char *) text description for error log ('vertex shader', 'fragment shader', etc...)
   // @param shader_source      (const char *) source string
   //
   GLuint compilarAdjuntarShader(  GLenum shader_type, const char * shader_description, const char * shader_source );
                                       
   // lee la 'location' de un uniform, da una advertencia si no está activo
   // @param name (conat char *) - uniform name in the shaders sources
   //
   GLint leerLocation( const char * name );

   // crea, compila y usa el objeto programa
   void crearObjetoPrograma( );

   // lee las 'locations' de los parámetros uniforms y los inicializa 
   void inicializarUniforms();

   // imprime los nombres y tipos de los uniform del programa (para debug)
   void imprimeInfoUniforms();

   // usar (activar) el objeto programa para las siguientes operaciones de visualización
   void activar();

   // Fija el color actual (valor por defecto del aributo de color)
   // @param nuevo_color (glm::vec3) nuevo color para visualizaciones sin tabla de colores. 
   //
   void fijarColor( const glm::vec3 & nuevo_color );

   // set 'use flat color' flag to true or false 
   // @param new_use_flat_color (bool) - 
   //
   void fijarUsarColorPlano( const bool nuevo_usar_color_plano );

   // resets the modelview matrix (MM) stack and set modelview to identity
   void resetMM();

   // pushes a copy of the current modelview matrix on top of the modelview matrix stack
   void pushMM();

   // componese the current modelview matrix with a new matrix
   // @param mat (mat4) -- matrix to componse
   //
   void compMM( const glm::mat4 & mat );
   
   // removes the current matrix on top of the modelview matrix stack (cannot be empty)
   void popMM();

   // sets the projection matrix
   void fijarMatrizProyeccion( const glm::mat4 & new_projection_mat );

   // index for the vertex positions attribute (must be cero)
   static constexpr GLuint ind_atrib_positions = 0 ; 

   // index for the vertex colors attribute (shaders must use attribute with index 1)
   static constexpr GLuint ind_atrib_colores = 1 ;

   // total number of attributes these program objects handles (0->positions, 1->colors)
   static constexpr GLuint num_atribs = 2 ;
   
   protected: // ---------------------------
   
   // program and shaders ids (0 means still not properly initialized)
   GLuint id_prog     = 0 ,
          id_frag_shader = 0 ,
          id_vert_shader = 0 ;


   // pointer to strings with shaders sources (assigned by derived classes constructors)
   const char * vertex_shader_source   = nullptr,
              * fragment_shader_source = nullptr,
              * geometry_shader_source = nullptr ;

   //  buffer used to report logs
   static constexpr GLsizei  buffer_length = 1024*16 ;
   static           GLchar   buffer[ buffer_length ] ;
   static           GLsizei  report_length ; 

   

   // pipeline state variables

   glm::mat4              modelview_mat      = glm::mat4(1.0);  // current modelview matrix (initially equal to the identity matrix)
   std::vector<glm::mat4> modelview_mat_stack ;                 // stack for saved modelview matrices
   GLint                  modelview_mat_loc  = -1 ;             // uniform location for the modelview matrix
   glm::mat4              projection_mat     = glm::mat4(1.0);  // current projection matrix (initially equal to the identity matrix)
   GLint                  projection_mat_loc = -1 ;             // uniform location for projection matrix
   glm::vec3              color              = { 0.0, 0.0, 0.0 }; // current color 
   GLint                  loc_usar_color_plano = -1 ;               // location for the uniform 'use flat color'
   
};


// returns a string with the name of a type from its corresponding OpenGL symbolic constant 
const std::string  typeName( const GLenum type );

#endif
