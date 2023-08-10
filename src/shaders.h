// Shader program object wrapper class

#ifndef SHADERS_H
#define SHADERS_H

#include <vector>
#include "glincludes.h"

// ****************************************************************************************
// This class holds a program object name or id, all the uniform locations, and provides 
// helper functions which made it easier to use a program object and modify its status
//
class Pipeline
{
   public: 

   // creates an empty, non-usable, program object
   Pipeline() ;

   // Creates, compiles and attach a shader to the current binded program object
   //
   // @param shader_type        (GLenum) one of: GL_VERTEX_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER
   // @param shader_description (const char *) text description for error log ('vertex shader', 'fragment shader', etc...)
   // @param shader_source      (const char *) source string
   //
   GLuint createCompileAttachShader(  GLenum shader_type, const char * shader_description, const char * shader_source );
                                       
   // get uniform location. warns if it is not active. 
   // @param name (conat char *) - uniform name in the shaders sources
   //
   GLint getLocation( const char * name );

   // creates, compiles and uses (activates) the program object
   void createCompileUseProgram( );

   // read all uniforms locations and initializes the uniforms
   void initializeUniforms();

   // imprime los nombres y tipos de los uniform del programa (para debug)
   void printUniformsInfo();

   // use (activate) this program object for each following operation
   void use();

   // sets current color
   // @param new_color (glm::vec3) new color to use for drawing without vertex colors 
   //
   void setColor( const glm::vec3 & new_color );

   // set 'use flat color' flag to true or false 
   // @param new_use_flat_color (bool) - 
   //
   void setUseFlatColor( const bool new_use_flat_color );

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
   void setProjectionMatrix( const glm::mat4 & new_projection_mat );

   // index for the vertex positions attribute (must be cero)
   static constexpr GLuint ind_atrib_positions = 0 ; 

   // index for the vertex colors attribute (shaders must use attribute with index 1)
   static constexpr GLuint ind_atrib_colors = 1 ;

   // total number of attributes these program objects handles (0->positions, 1->colors)
   static constexpr GLuint num_atribs = 2 ;
   
   protected: // ---------------------------
   
   // program id (0 means still not properly initialized)
   GLuint program_id = 0 ;

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
   GLint                  use_flat_color_loc = -1 ;               // location for the uniform 'use flat color'
   
};


// returns a string with the name of a type from its corresponding OpenGL symbolic constant 
const std::string  typeName( const GLenum type );

#endif
