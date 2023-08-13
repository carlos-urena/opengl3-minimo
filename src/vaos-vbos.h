#ifndef VBOS_VAOS_H
#define VBOS_VAOS_H

#include <vector>
#include "glincludes.h"


// --------------------------------------------------------------------------------------------

// Guarada los datos y metadatos de un VBO con una tabla de atributos de vértice
//
class DescrVBOAtribs
{
   private: 

   // metadatos y datos del buffer

   GLuint       buffer   = 0 ; // nombre o id del buffer en la GPU (0 antes de crearlo, >0 después)
   GLuint       index    = 0 ; // índice de atributo (<num_attrs)
   GLenum       type     = 0 ; // tipo de los valores (GL_FLOAT o GL_DOUBLE)
   GLint        size     = 0 ; // numero de valores por tupla (usualmente 2,3, o 4)
   GLsizei      count    = 0 ; // número de tuplas en la tabla (>0)
   GLsizeiptr   tot_size = 0 ; // tamaño completo de la tabla en bytes (=count*size*sizeof(c-type))
   
   const void * data     = nullptr ; // datos originales en la CPU (null antes de saberlos, no null después)
   void *       own_data = nullptr ; // si no nulo, tiene copia de los datos (propiedad de este objeto).
   
   void copyData() ; // copia los datos del VBO desde 'data' a 'own_data'

   public:

   // constructores, asumen offset y stride a 0 (AOS con una tabla por VBO)
   
   DescrVBOAtribs( const unsigned p_index, const GLenum p_type, const unsigned p_size, 
               const unsigned long p_count, const void *p_data );  
   DescrVBOAtribs( const unsigned p_index, const std::vector<glm::vec3> & src_vec );
   DescrVBOAtribs( const unsigned p_index, const std::vector<glm::vec2> & src_vec );

   DescrVBOAtribs() = delete ; // impide usar constructor por defecto (sin parámetros)

   void comprobar() const ; // comprueba que los valores son correctos, aborta si no
   void crearVBO() ; // Crea y el VBO en la GPU (solo se puede llamar una vez), deja el VBO habilitado

   // Devuelve true solo si el VBO ya ha sido creado en la GPU
   inline bool creado() const { return buffer != 0; } 

   // Devuelve el índice de este VBO
   inline GLuint getIndex() const { return index; }

   // Devuelve el número de vértices
   inline GLuint getCount() const { return count; }

   ~DescrVBOAtribs();  // libera toda la memoria ocupada por el VBO en CPU y en GPU
} ;

// --------------------------------------------------------------------------------------------

// Guarda los datos y metadatos de un VBO de índices
//
class DescrVBOInds
{
   private:

   // metadatos y datos del buffer
   GLuint       buffer   = 0 ; // nombre o id del buffer en la GPU (0 antes de crearlo, >0 después)
   GLenum       type     = 0 ; // tipo de los valores (GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, GL_UNSIGNED_INT)
   GLsizei      count    = 0 ; // número de índices en la tabla (>0)
   GLsizeiptr   tot_size = 0 ; // tamaño completo de la tabla en bytes (=count*sizeof(c-type))
   
   const void * indices     = nullptr ; // datos originales en la CPU (null antes de saberlos, no null después)
   void *       own_indices = nullptr ; // si no nulo, tiene copia de los datos (propiedad de este objeto).
   
   void copyIndices() ; // incializa 'own_indices' con una copia de los datos en 'indices', y apunta 'indices' a 'own_indices'

   public:

   DescrVBOInds() = delete ; // impide usar constructor por defecto (sin parámetros)

   DescrVBOInds( const GLenum p_type, const GLsizei p_count, const void * p_data );
   DescrVBOInds( const std::vector<glm::uvec3> & src_vec );

   // comprueba que los valores son correctos, aborta si no
   void comprobar() const ;

   // Devuelve true solo si el VBO ya ha sido creado en la GPU
   inline bool creado() const { return buffer != 0; }

   // Crear y activar el VBO de índices, es decir:
   //   1. Crea el VBO y envía la tabla de índices a la GPU (únicamente la primera vez)
   //   2. Hace 'bind' de la tabla en el 'target' GL_ELEMENT_ARRAY_BUFFER
   void crearVBO( );

   // devuelve el número de vértices
   //inline GLuint getCount() const { return count; }

   // devuelve el número de vértices
   inline GLuint getType() const { return type; }

   // Dibuja el VAO actualmente activado usando esta tabla de índices
   // supone que el VAO, los VBOs de atributos y este VBO de índices ya están activados en openGL.
   void drawElements( const GLenum mode );

   // destruye un VAO (libera memoria dinámica de la aplic. y elimina VAO en la GPU)
   ~DescrVBOInds();

} ;

// -----------------------------------------------------------------------------------------------------

// Guarda los datos y metadatos de los VBOs que forman un VAO
//
class DescrVAO
{
   private:

   // nombre o identificador del VAO en la GPU (0 si no creado en GPU)
   GLuint array = 0 ; 

   // número de tablas de atributos que tiene el VAO (incluyendo las posicione)
   GLuint num_atribs = 0;

   // número de vértices en la tabla de posiciones de vértices 
   GLsizei count = 0 ;

   // número de índices en la tabla de índices (si hay índices, en otro caso 0)
   GLsizei idxs_count = 0 ;

   // si la secuencia es indexada, VBO de attrs, en otro caso
   DescrVBOInds * idxs_vbo   = nullptr ; 
   
   // array de descriptores de VBOs de atributos
   std::vector<DescrVBOAtribs *> attr_vbos ;

   // array que indica si cada tabla de atributos está habilitada o deshabilitada
   std::vector<bool> attr_habilitado ;

   void check( const unsigned index ); // comprueba precondiciones antes de añadir tabla de atribs

   public:    

   // inicializa un descriptor de VAO no indexado, leyendo una tabla de posiciones de vértices
   DescrVAO( const unsigned p_num_atribs, const std::vector<glm::vec3> & tabla_posiciones ) ;
   DescrVAO( const unsigned p_num_atribs, const std::vector<glm::vec2> & tabla_posiciones ) ;

   // incializa un descriptor de VAO no indexado, leyendo de un array (tipo C) de posiciones de vértices
   DescrVAO( const unsigned p_num_atribs, const GLenum p_type, const unsigned p_size, const unsigned long p_count, const void *p_data ) ;

   DescrVAO() = delete ; // impide usar constructor por defecto (sin parámetros)

   // crea el VAO en la GPU
   void crearVAO();

   // añade una tabla de atributos a un VAO

   void addAttrib( const unsigned index, const std::vector<glm::vec3> tabla_atributo  );
   void addAttrib( const unsigned index, const std::vector<glm::vec2> tabla_atributo  );
   void addAttrib( const unsigned index, const GLenum p_type, const unsigned p_size, 
                   const unsigned long p_count, const void *p_data );

   void addIndices( const std::vector<glm::uvec3> & tabla_indices );
   void addIndices( const GLenum p_type, const GLsizei p_count, const void * p_indices );

   // habilita/deshabilita una tabla de atributos (index no puede ser 0)
   void habilitarAttrib( const unsigned index, const bool habilitar );

   // ....
   void draw( const GLenum mode ) ;

   // ....
   ~DescrVAO();
} ;




#endif

