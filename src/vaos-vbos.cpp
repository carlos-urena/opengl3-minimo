
#include "vaos-vbos.h"
    
constexpr GLsizei stride = 0 ;
constexpr void *  offset = 0 ;
constexpr GLint   first  = 0 ;

// ------------------------------------------------------------------------------------------------------
// devuelve el tamaño en bytes de un valor a partir de entero asociado con el tipo del valor en OpenGL

constexpr inline GLsizeiptr size_in_bytes( const GLenum type )
{
   switch( type )
   {
      case GL_FLOAT          : return sizeof( float );          break ;
      case GL_DOUBLE         : return sizeof( double );         break ;
      case GL_UNSIGNED_BYTE  : return sizeof( unsigned char );  break ;
      case GL_UNSIGNED_SHORT : return sizeof( unsigned short ); break ;
      case GL_UNSIGNED_INT   : return sizeof( unsigned int );   break ;
      default                : assert( false ); return 0 ;      break ;
   }
}

// ----------------------------------------------------------------------------
// 
constexpr inline void check_attr_type( const GLenum type )
{
   assert( type == GL_FLOAT  || 
           type == GL_DOUBLE );
}

// ----------------------------------------------------------------------------
// 
constexpr inline void check_indices_type( const GLenum type )
{
   assert( type == GL_UNSIGNED_BYTE  || 
           type == GL_UNSIGNED_SHORT || 
           type == GL_UNSIGNED_INT   );
}

// ------------------------------------------------------------------------------------------------------
// comprueba que el modo es válido para las llamadas glDrawArrays y glDrawElements

constexpr void check_mode( const GLenum mode )
{
   assert( mode == GL_TRIANGLES  || mode == GL_LINES || mode == GL_POINTS || 
           mode == GL_LINE_STRIP || mode == GL_LINE_LOOP  );  
}

// ******************************************************************************************************
// Clase AttribsVBO

// ------------------------------------------------------------------------------------------------------

AttribsVBO::AttribsVBO( const unsigned p_index, const GLenum p_type, const unsigned p_size, 
                        const unsigned long p_count, const void *p_data )
{
   // copiar valores de entrada y comprobar que son correctos
   index    = p_index ;
   type     = p_type ;
   size     = p_size ;
   count    = p_count ;
   data     = p_data ;
   tot_size = size*count*size_in_bytes( type );

   copyData();
   comprobar() ; 
}  

// ------------------------------------------------------------------------------------------------------
// 

AttribsVBO::AttribsVBO( const unsigned p_index, const std::vector<glm::vec3> & src_vec )
{
   index    = p_index ;
   type     = GL_FLOAT ;
   size     = 3 ;
   count    = src_vec.size();
   data     = src_vec.data();
   tot_size = size*count*size_in_bytes( type );
   
   copyData() ;
   comprobar();
}

// ------------------------------------------------------------------------------------------------------
// 

AttribsVBO::AttribsVBO( const unsigned p_index, const std::vector<glm::vec2> & src_vec )
{
   index    = p_index ;
   type     = GL_FLOAT ;
   size     = 2 ;
   count    = src_vec.size();
   data     = src_vec.data();
   tot_size = size*count*size_in_bytes( type );
   
   copyData();
   comprobar();
}

// ------------------------------------------------------------------------------------------------------
// incializa 'own_data' con una copia de los datos en 'data', y apunta 'data' a 'own_data'

void AttribsVBO::copyData()
{
   assert( data != nullptr );     // 'data' debe apuntar a los datos originales
   assert( 0 < tot_size );        // 'tot_size' debe tener el tamaño total de los datos
   assert( own_data == nullptr ); // impide copiar los datos dos veces 

   own_data = new unsigned char [tot_size] ; // reservar memoria
   assert( own_data != nullptr );            // comprobar que se ha podido reservar la memoria
   std::memcpy( own_data, data, tot_size );  // copiar bytes
   data = own_data ;                         // apuntar a los datos propios
}

// ------------------------------------------------------------------------------------------------------
// comprueba que los descriptoresde de la tabla de datos son correctos, aborta si no

void AttribsVBO::comprobar() const 
{
   assert( data != nullptr );
   check_attr_type( type );
   assert( own_data == nullptr || own_data == data );
   assert( 1 <= size && size <= 4 ); 
   assert( type == GL_FLOAT || type == GL_DOUBLE );
   assert( tot_size == size*count*size_in_bytes( type ));
}

// ------------------------------------------------------------------------------------------------------
// Crea y el VBO en la GPU (solo se puede llamar una vez), deja el VBO habilitado en el 
// índice de atributo 'index'

void AttribsVBO::crearVBO() 
{
   // comprobar precondiciones
   assert( glGetError() == GL_NO_ERROR );
   assert( buffer == 0 );  
   comprobar();

   // generar un nuevo identificador de VBO 
   glGenBuffers( 1, &buffer ); assert( 0 < buffer );

   // fija este buffer como buffer 'activo' actualmente en el 'target' GL_ARRAY_BUFFER
   glBindBuffer( GL_ARRAY_BUFFER, buffer ); 

   // transfiere los datos desde la memoria de la aplicación al VBO en GPU
   glBufferData( GL_ARRAY_BUFFER, tot_size, data, GL_STATIC_DRAW );  
      
   // indicar, para este índice de atributo, la localización y el formato de la tabla en el buffer 
   glVertexAttribPointer( index, size, type, GL_FALSE, stride, offset  );

   // desactivar el buffer
   glBindBuffer( GL_ARRAY_BUFFER, 0 );

   // por defecto, habilita el uso de esta tabla de atributos
   glEnableVertexAttribArray( index );

   // comprobar que no ha habido error durante la creación del VBO
   assert( glGetError() == GL_NO_ERROR );
}
// ------------------------------------------------------------------------------------------------------

AttribsVBO::~AttribsVBO()
{
   delete [] (unsigned char *) own_data ;
   own_data = nullptr ; // probablemente innecesario
   
   if ( buffer != 0 )
   {
      assert( glGetError() == GL_NO_ERROR );
      glDeleteBuffers( 1, &buffer );
      assert( glGetError() == GL_NO_ERROR );
      buffer = 0 ; // probablemente innecesario
   }

}

// ******************************************************************************************************
// Clase IndexesVBO

// ------------------------------------------------------------------------------------------------------

IndexesVBO::IndexesVBO( const GLenum p_type, const GLsizei p_count, const void * p_indices )
{
   type     = p_type ;
   count    = p_count ;
   indices  = p_indices ;
   tot_size = count*size_in_bytes( type ) ;
   
   copyIndices();
   comprobar();
}

// ------------------------------------------------------------------------------------------------------

IndexesVBO::IndexesVBO( const std::vector<glm::uvec3> & src_vec )
{
   type     = GL_UNSIGNED_INT ;
   count    = 3*src_vec.size() ;
   indices  = src_vec.data() ;
   tot_size = count*size_in_bytes( type ) ;

   copyIndices();
   comprobar();
}

// ------------------------------------------------------------------------------------------------------
// incializa 'own_data' con una copia de los datos en 'data', y apunta 'data' a 'own_data'

void IndexesVBO::copyIndices()
{
   assert( indices != nullptr );     // 'indices' debe apuntar a los indices originales
   assert( 0 < tot_size );           // 'tot_size' debe tener el tamaño total de los datos
   assert( own_indices == nullptr ); // impide copiar los datos dos veces 

   own_indices = new unsigned char [tot_size] ;    // reservar memoria
   assert( own_indices != nullptr );               // comprobar que se ha podido reservar
   std::memcpy( own_indices, indices, tot_size );  // copiar bytes
   indices = own_indices ;                         // apuntar a los índices propios
}

// ------------------------------------------------------------------------------------------------------
// comprueba que los valores son correctos, aborta si no

void IndexesVBO::comprobar() const 
{
   assert( indices != nullptr ); 
   check_indices_type( type );
   assert( 0 < count );
   assert( type == GL_UNSIGNED_BYTE || type == GL_UNSIGNED_SHORT || type == GL_UNSIGNED_INT  );
   assert( tot_size == count*size_in_bytes( type ));
}

// ------------------------------------------------------------------------------------------------------
// Crea y el VBO en la GPU (solo se puede llamar una vez), deja el VBO activado ('binded') en el
// 'target' GL_ELEMENT_ARRAY_BUFFER

void IndexesVBO::crearVBO( )
{
   // comprobar precondiciones:
   assert( glGetError() == GL_NO_ERROR ); // comprobar y limpiar errores previos de OpenGL
   assert( buffer == 0 );                 // impedir que se llame más de una vez para este objeto
   comprobar();                           // comprobar que este objeto está en un estado correcto

   // crear el VBO y enviar los índices a la GPU 
   glGenBuffers( 1, &buffer ); assert( 0 < buffer );
   
   // activar ('bind') el buffer en el 'target' GL_ELEMENT_ARRAY_BUFFER
   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buffer ); 

   // transferir los datos desde la memoria de la aplicación al VBO en GPU
   glBufferData( GL_ELEMENT_ARRAY_BUFFER, tot_size, indices, GL_STATIC_DRAW );  
      
   // comprueba que no ha habido error al crear el VBO 
   assert( glGetError() == GL_NO_ERROR );
}


// ---------------------------------------------------------------------------------------------
IndexesVBO::~IndexesVBO()
{
   delete [] (unsigned char *) own_indices ;
   own_indices = nullptr ; // probablemente innecesario

   if ( buffer != 0 )
   {
      assert( glGetError() == GL_NO_ERROR );
      glDeleteBuffers( 1, &buffer );
      assert( glGetError() == GL_NO_ERROR );
      buffer = 0 ; // probablemente innecesario
   }
}

// ******************************************************************************************************
// Clase VAOdescr

// ------------------------------------------------------------------------------------------------------
// crea un VAO no indexado, con la tabla de posiciones de vértices

VAOdescr::VAOdescr( const unsigned p_num_atribs, const std::vector<glm::vec3> & tabla_posiciones ) 
{
   // comprobar precondiciones 
   assert( 0 < p_num_atribs );
   assert( 0 < tabla_posiciones.size() );

   // registrar el número de atributos
   num_atribs = p_num_atribs ;

   // crear el vector con punteros a los descriptores de VBOs (todos a null)
   attr_vbos.resize( num_atribs, nullptr ); 

   // crear el vector con los flags de habilitado/deshabilitado (todos a 'true', por defecto están habilitados)
   attr_habilitado.resize( num_atribs, true );

   // registrar el número de vértices en la tabla de posiciones
   count = tabla_posiciones.size();

   // clonar el descriptor de VBO de posiciones y apuntarlo desde este objeto
   attr_vbos[0] = new AttribsVBO( 0, tabla_posiciones );  
}

// ------------------------------------------------------------------------------------------------------
VAOdescr::VAOdescr( const unsigned p_num_atribs, const std::vector<glm::vec2> & tabla_posiciones ) 
{
   // comprobar precondiciones 
   assert( 0 < p_num_atribs );
   assert( 0 < tabla_posiciones.size() );

   // registrar el número de atributos
   num_atribs = p_num_atribs ;

   // crear el vector con punteros a los descriptores de VBOs (todos a null)
   attr_vbos.resize( num_atribs, nullptr ); 

   // crear el vector con los flags de habilitado/deshabilitado (todos a 'true', por defecto están habilitados)
   attr_habilitado.resize( num_atribs, true );

   // registrar el número de vértices en la tabla de posiciones
   count = tabla_posiciones.size();

   // clonar el descriptor de VBO de posiciones y apuntarlo desde este objeto
   attr_vbos[0] = new AttribsVBO( 0, tabla_posiciones );  
}

// ------------------------------------------------------------------------------------------------------
// incializa un descriptor de VAO no indexado, leyendo de un array (tipo C) de posiciones de vértices

VAOdescr::VAOdescr( const unsigned p_num_atribs, const GLenum p_type, 
                    const unsigned p_size, const unsigned long p_count, const void *p_data ) 
{
   // comprobar precondiciones 
   assert( 0 < p_num_atribs );
   assert( 0 < p_count );
   check_attr_type( p_type );

   // registrar el número de atributos
   num_atribs = p_num_atribs ;

   // crear el vector con los flags de habilitado/deshabilitado (todos a 'true', por defecto están habilitados)
   attr_habilitado.resize( num_atribs, true );

   // crear el vector con punteros a los descriptores de VBOs (todos a null)
   attr_vbos.resize( num_atribs, nullptr ); 

   // registrar el número de vértices en la tabla de posiciones
   count = p_count ;

   // clonar el descriptor de VBO de posiciones y apuntarlo desde este objeto
   attr_vbos[0] = new AttribsVBO( 0, p_type, p_size, p_count, p_data ); 
}


// ------------------------------------------------------------------------------------------------------
// añade una tabla de atributos a un VAO

void VAOdescr::addAttrib( const unsigned index, const std::vector<glm::vec3> tabla_atributo )
{
   // comprobar precondiciones:
   assert( attr_vbos[0] != nullptr ); // el VAO ya debe tener el VBO de posiciones.
   assert( 0 < index );  // no permite el índice 0, son las posiciones y se dan al construir el VAO
   assert( index < num_atribs ); // no permite índices fuera de rango
   assert( attr_vbos[index] == nullptr ); // no permite añadir un atributo dos veces 
   assert( array == 0 ); // no permite añadir atributos si el VAO ya esá alojado en la GPU
   assert( (unsigned long) count == tabla_atributo.size() ); // debe tener el mismo núm de items que VBO posiciones

   // clonar el descriptor del VBO de atributos y referenciarlo desde su entrada
   attr_vbos[index] = new AttribsVBO( index, tabla_atributo ); 
}

// ------------------------------------------------------------------------------------------------------
// añade una tabla de atributos a un VAO, a partir de un array clásico de C

void VAOdescr::addAttrib( const unsigned index, const GLenum p_type, const unsigned p_size, 
                          const unsigned long p_count, const void *p_data )
{
   // comprobar precondiciones:
   assert( attr_vbos[0] != nullptr ); // el VAO ya debe tener el VBO de posiciones.
   assert( 0 < index );  // no permite el índice 0, son las posiciones y se dan al construir el VAO
   assert( index < num_atribs ); // no permite índices fuera de rango
   assert( attr_vbos[index] == nullptr ); // no permite añadir un atributo dos veces 
   assert( array == 0 ); // no permite añadir atributos si el VAO ya esá alojado en la GPU
   assert( (unsigned long) count == p_count ); // debe tener el mismo núm de items que VBO posiciones

   // crear el VBO y referenciarlo desde este objeto
   attr_vbos[index] = new AttribsVBO( index, p_type, p_size, p_count, p_data ); // clonar el descriptor del VBO de atributos y ponerlo en su entrada
}

// ------------------------------------------------------------------------------------------------------

void VAOdescr::addIndices( const GLenum p_type, const GLsizei p_count, const void * p_indices )
{
   // comprobar precondiciones:
   assert( array == 0 ); // no permite añadir atributos si el VAO ya esá alojado en la GPU
   assert( idxs_vbo == nullptr ); // no permite añadir la tabla de índices dos veces
   assert( 0 < p_count );

   // registrar el número de índices (3 por tupla)
   idxs_count = p_count ;

   // crear el VBO y referenciarlo desde este objeto 
   idxs_vbo = new IndexesVBO( p_type, p_count, p_indices );
}

// ------------------------------------------------------------------------------------------------------

void VAOdescr::addIndices( const std::vector<glm::uvec3> & tabla_indices )
{
   // comprobar precondiciones:
   assert( array == 0 ); // no permite añadir atributos si el VAO ya esá alojado en la GPU
   assert( idxs_vbo == nullptr ); // no permite añadir la tabla de índices dos veces
   assert( 0 < tabla_indices.size() );

   // registrar el número de índices (3 por tupla)
   idxs_count = 3*tabla_indices.size() ;
   
   // crear el VBO y referenciarlo desde este objeto 
   idxs_vbo = new IndexesVBO( tabla_indices );
}

// ------------------------------------------------------------------------------------------------------
// ...
void VAOdescr::crearVAO()
{
   assert( glGetError() == GL_NO_ERROR );
   assert( array == 0 ); // asegurarnos que únicamente se invoca una vez para este descriptor

   // crear el VBO (queda 'binded')
   glGenVertexArrays( 1, &array ); assert( array > 0 );
   glBindVertexArray( array );

   // crear (y habilitar) los VBOs de posiciones y atributos en este VAO 
   attr_vbos[0]->crearVBO();
   for( unsigned i = 1 ; i < num_atribs ; i++ )
      if ( attr_vbos[i] != nullptr )
         attr_vbos[i]->crearVBO();
      
   // si procede, crea el VBO de índices 
   if ( idxs_vbo != nullptr )
         idxs_vbo->crearVBO();

   // deshabilitar tablas que no estén habilitadas
   for( unsigned i = 1 ; i < num_atribs ; i++ )
      if ( attr_vbos[i] != nullptr )
         if ( ! attr_habilitado[i] )
            glDisableVertexAttribArray( i );

   assert( glGetError() == GL_NO_ERROR );
}
// ------------------------------------------------------------------------------------------------------
// habilita/deshabilita una tabla de atributos (index no puede ser 0)

void VAOdescr::habilitarAttrib( const unsigned index, const bool habilitar )
{
   // comprobar precondiciones
   assert( 0 < index ); // el atributo 0 siempre está habilitado, no puede deshabilitarse
   assert( index < num_atribs ); // al índice debe estar en su rango
   assert( attr_vbos[index] != nullptr ); // no tiene sentido usarlo para un atributo para el cual no hay tabla

   // registrar el nuevo valor del flag
   attr_habilitado[index] = habilitar ;
   
   // si el VAO ya se ha enviado a la GPU, actualizar estado del VAO en OpenGL
   if ( array != 0 )
   {
      assert( glGetError() == GL_NO_ERROR );
      glBindVertexArray( array );
      
      if ( habilitar ) 
         glEnableVertexAttribArray( index );
      else 
         glDisableVertexAttribArray( index );

      glBindVertexArray( 0 );
      assert( glGetError() == GL_NO_ERROR );
   }

}
// ------------------------------------------------------------------------------------------------------

void VAOdescr::draw( const GLenum mode )
{
   assert( glGetError() == GL_NO_ERROR );
   assert( attr_vbos[0] != nullptr ); // asegurarnos que hay una tabla de coordenadas de posición.
   check_mode( mode );                // comprobar que el modo es el correcto.
   

   // si el VAO no está creado, crearlo y dejarlo 'binded', si ya está creado, solo se hace 'bind'
   if ( array == 0 )
      crearVAO();
   else 
      glBindVertexArray( array );
      
   assert( glGetError() == GL_NO_ERROR );

   // dibujar
   if ( idxs_vbo != nullptr ) // es una secuencia indexada
   {
      const GLenum type  = idxs_vbo->getType() ;
      glDrawElements( mode, idxs_count, type, offset );
   }
   else // no es una secuencia indexada
      glDrawArrays( mode, first, count );

   assert( glGetError() == GL_NO_ERROR );
}

// ------------------------------------------------------------------------------------------------------
// destruye un VAO (libera memoria dinámica de la aplic. y elimina VAO en la GPU)

VAOdescr::~VAOdescr()
{
   // todo ....(destruir VBOdescr clonados, eliminar VAO de la GPU)

   for( unsigned i = 1 ; i < num_atribs ; i++ )
   {  
      delete attr_vbos[i] ;
      attr_vbos[i] = nullptr ; 
   }
   
   delete idxs_vbo ;
   idxs_vbo = nullptr ; 
   

   if ( array != 0 )
   {
      assert( glGetError() == GL_NO_ERROR );
      glDeleteVertexArrays( 1, &array );
      assert( glGetError() == GL_NO_ERROR );
      array = 0 ; // probablemente innecesario
   }
}
// ------------------------------------------------------------------------------------------------------


