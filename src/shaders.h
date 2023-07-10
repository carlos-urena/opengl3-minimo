// Shader program object wrapper class

#ifndef SHADERS_H
#define SHADERS_H

#include "glincludes.h"

class ProgramObject
{
   public: 

   // creates an empty, non-usable, program object
   ProgramObject() ;

   // create, compile a shader and attach it to the program
   GLuint createCompileAttachShader( GLenum shader_type, const char * shader_description, 
                                     const char * shader_source );
                                       
   // get uniform location. warns if it is not active. 
   GLint getLocation( const char * name );

   // creates, compiles and uses (activates) the program object
   void createCompileUseProgram( );

   // index for the vertex positions attribute (must be cero)
   static constexpr GLuint ind_atrib_posiciones = 0 ;

   protected: // ---------------------------
   
   // program id (0 means still not properly initialized)
   GLuint program_id = 0 ;

   // pointer to strings with shaders sources
   const char * vertex_shader_source   = nullptr,
              * fragment_shader_source = nullptr,
              * geometry_shader_source = nullptr ;

   //  buffer used to report logs
   static constexpr GLsizei  buffer_length = 1024*16 ;
   static           GLchar   buffer[ buffer_length ] ;
   static           GLsizei  report_length ;  

} ;

// ---------------------------------------------------------------

class BasicProgramObject : public ProgramObject 
{
   public:
   BasicProgramObject() ;

   // set current color 
   void setColor( const glm::vec3 & new_color );

   // set 'use flat color' flag to true or false 
   void setUseFlatColor( const bool new_use_flat_color );

   // handling the modelview matrix (MM) stack
   void resetMM();
   void pushMM();
   void compMM( const glm::mat4 & mat );
   void popMM();

   // sets the projection matrix
   void setProjectionMatrix( const glm::mat4 & new_projection_mat );

   // index for the vertex colors attribute (shaders must use attribute with index 1)
   static constexpr GLuint ind_atrib_colores = 1 ;

   // total number of attributes these program objects handles (0->positions, 1->colors)
   // (would be much better to use a vitural function in base class ProgramObject)
   static constexpr GLuint num_atribs = 2 ;

   // --------------------------------------------
   protected:

   glm::mat4              modelview_mat      = glm::mat4(1.0);  // current modelview matrix (initially equal to the identity matrix)
   std::vector<glm::mat4> modelview_mat_stack ;                 // stack for saved matrices
   GLint                  modelview_mat_loc  = -1 ;             // uniform location
   glm::mat4              projection_mat     = glm::mat4(1.0);  // current projection matrix (initially equal to the identity matrix)
   GLint                  projection_mat_loc = -1 ;             // uniform location for projection matrix
   glm::vec3              color              = { 0.0, 0.0, 0.0 }; // current color 
   GLint                  use_flat_color_loc = -1 ;             // 'use flat color' flag
   
};

#endif
