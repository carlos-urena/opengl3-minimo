#ifndef VBOS_VAOS_H
#define VBOS_VAOS_H

#include <vector>
#include "glincludes.h"

#define CError()  assert( glGetError() == GL_NO_ERROR );

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
   
   void copyData() ; // copia los datos del VBO a una zona de memoria controlada por esta instancia

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
   
   // Inicializa 'own_indices' con una copia de los datos en 'indices', y apunta 
   // 'indices' a 'own_indices'
   //
   void copyIndices() ; 

   public:

   DescrVBOInds() = delete ; // impide usar constructor por defecto (sin parámetros)

   // Crea un descriptor de VBO de índices, a partir de sus metadatos (type, count) y un 
   // puntero a los datos (p_indices)
   // 
   // @param p_type  (GLEnum)   tipo de los datos (GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT o GL_UNSIGNED_INT)
   // @param p_count (unsigned) número de índices (>0)
   // @param p_data  (void *)   puntero al array de índices (no nulo, solo se lee)   
   //
   DescrVBOInds( const GLenum p_type, const GLsizei p_count, const void * p_data ); 

   // Crea un descriptor de VBO de índices, a partir de una tabla de tuplas con 3 enteros
   // (se puede usar típicamente para tablas de triángulos)
   // 
   // @param src_vec  (vector<uvec3> &)  tabla de tuplas de 3 enteros (triángulos)
   //
   DescrVBOInds( const std::vector<unsigned> & src_vec );
   
   
   // Crea un descriptor de VBO de índices, a partir de una tabla de tuplas con 3 enteros
   // (se puede usará típicamente para tablas de triángulos)
   // 
   // @param src_vec  (vector<uvec3> &)  tabla de tuplas de 3 enteros (triángulos)
   // 
   DescrVBOInds( const std::vector<glm::uvec3> & src_vec );


   // Comprueba que los metadatos son correctos, aborta si no
   void comprobar() const ;

   // Devuelve true solo si el VBO ya ha sido creado en la GPU
   inline bool creado() const { return buffer != 0; }

   // Crear y activar el VBO de índices, es decir:
   //   1. Crea el VBO y envía la tabla de índices a la GPU (únicamente la primera vez)
   //   2. Hace 'bind' de la tabla en el 'target' GL_ELEMENT_ARRAY_BUFFER
   //
   void crearVBO( );

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

   // si hay índices, tiene el tipo de los índices 
   GLenum idxs_type ;

   // si la secuencia es indexada, VBO de attrs, en otro caso
   DescrVBOInds * buffer_indices   = nullptr ; 
   
   // vector de punteros a los descriptores de VBOs de atributos
   std::vector<DescrVBOAtribs *> buffer_atributo ;

   // array que indica si cada tabla de atributos está habilitada o deshabilitada
   std::vector<bool> atrib_habilitado ;

   void check( const unsigned index ); // comprueba precondiciones antes de añadir tabla de atribs

   public:    

   // impide usar constructor por defecto (sin parámetros)
   DescrVAO() = delete ; 

   // Crea un VAO no indexado, dando una tabla de coordendas (vec3) de posición de vértices
   //
   // @param p_num_atribs     (unsigned)      número de atributos que puede tener este VAO 
   // @param tabla_posiciones (vector<vec3>)  tabla de coordenadas de vértices (únicmante se lee)
   //
   DescrVAO( const unsigned p_num_atribs, const std::vector<glm::vec3> & tabla_posiciones ) ;
   
   // Crea un VAO no indexado, dando una tabla de coordendas (vec2) de posición de vértices
   //
   // @param p_num_atribs     (unsigned)      número de atributos que puede tener este VAO 
   // @param tabla_posiciones (vector<vec3>)  tabla de coordenadas de vértices (únicmante se lee)
   //
   DescrVAO( const unsigned p_num_atribs, const std::vector<glm::vec2> & tabla_posiciones ) ;

   // Crea un descriptor de VAO, a partir de los metadatos de la tabla 
   // de posiciones (type, size, count) y un puntero a los datos (p_data)
   // 
   // @param p_num_atribs (unsigned) número de atributos que tendrá este VAO como máximo 
   // @param p_type  (GLenum)   tipo de los datos (GL_FLOAT o GL_DOUBLE)
   // @param p_size  (unsigned) tamaño de las tuplas o vectores (2, 3 o 4)
   // @param p_count (unisgned) número de tuplas (>0)
   // @param p_data  (void *)   puntero al array de tuplas (no nulo)   
   //
   DescrVAO( const unsigned p_num_atribs, const GLenum p_type, const unsigned p_size, 
             const unsigned long p_count, const void *p_data ) ;

   
   // Crea el identificador del VAO (array), crea los identificadores de los VBOs asociados, 
   // transfiere los datos de todas las tablas a la GPU.
   //
   void crearVAO();

   // Añade una tabla de atributos a este VAO, a partir de un vector de 'vec3' con 
   // los datos del atributo
   //
   // @param index (unsigned) índice del atributo (no puede ser 0, la tabla de posiciones se da en el constructor)
   // @param tabla_atributo (vector<vec3>) datos para el VBO 
   //
   void addAttrib( const unsigned index, const std::vector<glm::vec3> tabla_atributo  );

   // Añade una tabla de atributos a este VAO, a partir de un vector de 'vec2' con 
   // los datos del atributo
   //
   // @param index (unsigned) índice del atributo (no puede ser 0, la tabla de posiciones se da en el constructor)
   // @param tabla_atributo (vector<vec3>) datos para el VBO 
   //
   void addAttrib( const unsigned index, const std::vector<glm::vec2> tabla_atributo  );

   // Añade una tabla de atributos a este VAO, a partir de los metadatos y datos 
   // del atributo
   //
   // @param index (unsigned) índice del atributo (no puede ser 0, la tabla de posiciones se da en el constructor)
   // @param tabla_atributo (vector<vec3>) tabla de datos del VBO a crear 
   //
   void addAttrib( const unsigned index, const GLenum p_type, const unsigned p_size, 
                   const unsigned long p_count, const void *p_data );

   // Añade una tabla de índices a este VAO a partir de los datos en un vector de 'uvec3' 
   // (por tanto el VAO pasa a ser indexado). Se usa típicamente para tablas de triángulos.
   //
   // @param tabla_indices (vector<uvec3>) tabla de índices, organizados en vectores de 3 unsigned (únicamente se lee)
   //
   void addIndices( const std::vector<glm::uvec3> & tabla_indices );
   
   // Añade una tabla de índices a este VAO a partir de 
   // los datos en un vector de 'uvec3' (por tanto el VAO pasa a ser indexado)
   // Se usa típicamente para tablas de triángulos.
   //
   // @param tabla_indices (vector<uvec3>) tabla de índices, organizados en vectores de 3 unsigned (únicamente se lee)
   //
   void addIndices( const std::vector<unsigned> & tabla_indices );

   // Añade una tabla de índices a este VAO a partir de 
   // los metadatos y datos de la tabla de índices (por tanto el VAO pasa a ser indexado)
   //
   // @param p_type (GLenum) tipo de los índices, debe ser:
   // @param p_count (GLsizei = unsigned long) número de índices en la tabla (>0)
   // @param p_indices (void *) puntero a la tabla con los índices  
   //
   void addIndices( const GLenum p_type, const GLsizei p_count, const void * p_indices );

   // habilita/deshabilita una tabla de atributos (index no puede ser 0)
   void habilitarAttrib( const unsigned index, const bool habilitar );

   // ....
   void draw( const GLenum mode ) ;

   // ....
   ~DescrVAO();
} ;




#endif

