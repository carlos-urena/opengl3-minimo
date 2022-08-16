#ifndef TUP_MAT_H
#define TUP_MAT_H

#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>

namespace tup_mat
{

// definir alias de 'unsigned int' cuyo descriptor tiene un solo token
typedef unsigned int uint ;

// definir M_PI, si no estaba definido ya
#ifndef M_PI
constexpr double M_PI = 3.1415926535897932384626433832795028841971693993751 ;
#endif

// constantes para acceder a las componentes de las tuplas
const unsigned
   X = 0,
   Y = 1,
   Z = 2,

   R = 0,
   G = 1,
   B = 2 ;

   // *********************************************************************
//
// plantilla de clase: TuplaG
// clase para tuplas de valores numéricos (genéricas)
// 
// parámetros: 
//    T - tipo de los valores numéricos (float, double, int, unsigned, etc...)
//    n - número de valores
//
// *********************************************************************

template< class T, unsigned n >
class TuplaG
{
   private:
   T coo[n] ;  // vector de valores escalares

   public:
   // constructor por defecto: no hace nada
   inline TuplaG();

   // constructor usando un array C++
   inline TuplaG( const T * org ) ;

   // acceso de lectura/escritura a un elemento (v[i]=x, x=v[i])
   //T & operator [] (const unsigned i) ;

   // acceso de solo lectura a un elemento ( x=v(i) )
   const T & operator () (const unsigned i) const ;

   // acceso de lectura-escritura a un elemento ( v(i)=x )
   T & operator () (const unsigned i) ;

   // conversion a un puntero de lectura/escritura de tipo T* ( T* p = tupla )
   operator  T * ()  ;

   // conversion a un puntero de solo lectura de tipo: const  T* ( const T* p = tupla )
   operator  const T * ()  const ;

   // suma componente a componente ( v1=v2+v3 )
   TuplaG<T,n> operator + ( const TuplaG & der ) const ;

   // resta componente a componente ( v1=v2-v3 )
   TuplaG<T,n> operator - ( const TuplaG & der ) const ;

   // devuelve tupla negada ( v1 = -v2 )
   TuplaG<T,n> operator - (  ) const ;

   // mult. por escalar por la derecha ( v1=v2*a )
   TuplaG<T,n> operator * ( const T & a ) const ;

   // division por escalar ( v1=v2/a )
   TuplaG<T,n> operator / ( const T & a ) const ;

   // producto escalar (dot)  a = v1.dot(v2)
   T dot( const TuplaG<T,n> & v2 ) const ;

   // operador binario para producto escalar a = v1|v2 ;
   T operator | ( const TuplaG & der ) const ;

   // obtener longitud al cuadrado
   T lengthSq( ) const ;

   // obtener una copia normalizada
   TuplaG<T,n> normalized() const ;
} ;

//----------------------------------------------------------------------
// operadores y funciones no miembros

// mult. por escalar por la izquierda ( v1=a*v2 )
template< class T, unsigned n >
inline TuplaG<T,n> operator *  ( const T & a, const  TuplaG<T,n> & der ) ;

// escritura de un vector en un ostream
template< class T, unsigned n >
inline std::ostream & operator <<  ( std::ostream & os, const TuplaG<T,n> & der ) ;

// *********************************************************************
//
// Plantilla de clase: TuplaG2
// especialización parcial para tuplas de 2 elementos
// (define constructores específicos)
//
// *********************************************************************

template< class T >
class TuplaG2 : public TuplaG<T,2>
{
   public:

   // constructores: por defecto
   TuplaG2() ;
   TuplaG2( const T & c0, const T & c1 ) ;
   TuplaG2( const TuplaG<T,2> & ini );
   void operator = ( const TuplaG<T,2> & der ) ;
} ;



// *********************************************************************
//
// Plantilla de clase: TuplaG3
// especialización parcial para tuplas de 3 elementos
// (define constructores específicos)
//
// *********************************************************************

template< class T >
class TuplaG3 : public TuplaG<T,3>
{
   public:

   // constructores: por defecto
   TuplaG3() ;
   TuplaG3( const T & c0, const T & c1, const T & c2 ) ;
   TuplaG3( const TuplaG<T,3> & ini );
   void operator = ( const TuplaG<T,3> & der ) ;
   void operator = ( const TuplaG<T,4> & der ) ; // asignar ignorando ultimo

   // producto vectorial (cross)  a = v1.cross(v2)
   TuplaG3<T> cross( const TuplaG3<T> & v2 ) const ;
} ;


// *********************************************************************
//
// Plantilla de clase: TuplaG4
// especialización parcial para tuplas de 4 elementos
// (define constructores específicos)
//
// *********************************************************************

template< class T >
class TuplaG4 : public TuplaG<T,4>
{
   public:

   // constructores: por defecto
   TuplaG4() ;
   TuplaG4( const T & c0, const T & c1, const T & c2, const T & c3 ) ;
   TuplaG4( const TuplaG<T,4> & ini );
   void operator = ( const TuplaG<T,4> & der ) ;
} ;



// *********************************************************************
// tipos concretos:

typedef TuplaG2<float>  Tupla2f ;
typedef TuplaG2<double> Tupla2d ;
typedef TuplaG2<uint>   Tupla2u ;
typedef TuplaG2<int>    Tupla2i ;

typedef TuplaG3<float>  Tupla3f ;
typedef TuplaG3<double> Tupla3d ;
typedef TuplaG3<uint>   Tupla3u ;
typedef TuplaG3<int>    Tupla3i ;

typedef TuplaG4<float>  Tupla4f ;
typedef TuplaG4<double> Tupla4d ;
typedef TuplaG4<int>    Tupla4i ;
typedef TuplaG4<uint>   Tupla4u ;


// *********************************************************************
//
// plantilla de clase: MatrizCG
// clase para matrices cuadradas genéricas
//
// *********************************************************************

template< class T, unsigned n >
class MatrizCG
{
   public:
   TuplaG< TuplaG<T,n>, n > mat ;

   MatrizCG<T,n> ()  {}  // constructor por defecto inline (no inicializa)

   // conversion a un puntero de lectura/escritura de tipo: T*
   // ( T* p = matriz )
   inline operator  T * ()  ;

   // conversion a un puntero de solo lectura de tipo: const T*
   // ( const T* p = matriz )
   inline operator  const T * ()  const ;

   // acceso de lectura a una fila
   // (devuelve puntero al primer elemento de una columna)
   //inline const TuplaG<T,n> & operator() ( const unsigned i ) const ;

   // acceso de lectura/escritura a una fila:
   //inline TuplaG<T,n> & operator[] ( int i )  ;

   // componer esta matriz con otra por la derecha
   inline MatrizCG<T,n> operator * ( const MatrizCG<T,n> & der ) const ;

   // acceso de solo lectura usando fila,columna: T x = m(fil,col)
   inline const T & operator()( const unsigned fil, const unsigned col ) const ;

   // acceso de lectura/escritura usando fila,columna: T x = m(fil,col)
   inline T & operator()( const unsigned fil, const unsigned col )  ;

   // multiplicar esta matriz por una tupla por la derecha
   inline TuplaG<T,n> operator * ( const TuplaG<T,n>  & t ) const ;

   // multiplicar esta matriz por una tupla por la derecha (con una dimesnión menos)
   // (se añade un 1, se multiplica, y luego se le quita la ultima componente)
   inline TuplaG<T,n-1> operator * ( const TuplaG<T,n-1>  & t ) const ;
} ;

// escritura de una matriz en un ostream
template< class T, unsigned n > inline
std::ostream & operator <<  ( std::ostream & os, const MatrizCG<T,n> & m ) ;


// --------------------------------------------------------------------
// un par de clases predefinidas 

typedef MatrizCG<float,4> Matriz4f ;
typedef MatrizCG<double,4> Matriz4d ;

// *********************************************************************
// Declaraciones de funciones para transformaciones típicas en IG 

// ---------------------------------------------------------------------
// juego de tests de la matrices
void MAT_Tests() ;

// ---------------------------------------------------------------------
// creación y operadores de matrices: transformaciones de modelado

inline Matriz4f MAT_Ident( ) ;
inline Matriz4f MAT_Traslacion( const Tupla3f & d ) ;
inline Matriz4f MAT_Escalado( const float sx, const float sy, const float sz ) ;
inline Matriz4f MAT_Rotacion( const float ang_gra, const Tupla3f & eje ) ;
inline Matriz4f MAT_Filas( const Tupla3f & fila0, const Tupla3f & fila1, 
                           const Tupla3f & fila2 );

// ---------------------------------------------------------------------
// matrices auxiliares para la transformación de vista

inline Matriz4f MAT_Transpuesta3x3( const Matriz4f & org ) ;
inline Matriz4f MAT_Filas         ( const Tupla3f fila[3] );
inline Matriz4f MAT_Columnas      ( const Tupla3f colum[3] );

// ---------------------------------------------------------------------
// matrices para la transformacion de vista:

inline Matriz4f MAT_LookAt( const Tupla3f & org, const Tupla3f & at, const Tupla3f & vup );
inline Matriz4f MAT_Vista         ( const Tupla3f eje[3], const Tupla3f& org );
inline Matriz4f MAT_Vista_inv     ( const Tupla3f eje[3], const Tupla3f& org );


// ---------------------------------------------------------------------
// matrices de transformación del viewport

inline Matriz4f MAT_Viewport      ( int org_x, int org_y, int ancho, int alto );
inline Matriz4f MAT_Viewport_inv  ( int org_x, int org_y, int ancho, int alto );

// ---------------------------------------------------------------------  
// matrices para la transf. de proyección

inline Matriz4f MAT_Frustum    ( const float l, const float r, const float b, const float t, const float n, const float f );
inline Matriz4f MAT_Ortografica( const float l, const float r, const float b, const float t, const float n, const float f );
inline Matriz4f MAT_Perspectiva( const float fovy_grad, const float raz_asp, const float n, const float f );

// --------------------------------------------------------------------
// calcula la inversa (la última fila debe ser 0, 0, 0, 1 - es decir,
// no debe haber proyeccion)
inline Matriz4f MAT_Inversa( const Matriz4f & m );



// *********************************************************************
// implementaciones in-line



// *********************************************************************
// Implementaciones 'inline' de tuplas 



// *********************************************************************
//
// plantilla de clase:
// TuplaG<T,n>
//
// *********************************************************************

template< class T, unsigned n> inline
TuplaG<T,n>::TuplaG()
{
   
} 

// constructor usando un array C++
template< class T, unsigned n> inline 
TuplaG<T,n>::TuplaG( const T * org ) 
{
   for( unsigned i = 0 ; i < n ; i++ )
      (*this)[i] = org[i] ;
}

//----------------------------------------------------------------------

//template< class T, unsigned n >
//T & TuplaG<T,n>::operator [] (const unsigned i)
//{
   //assert( i < n ) ;
   //return coo[i] ;
//}

//----------------------------------------------------------------------

template< class T, unsigned n > inline
const T & TuplaG<T,n>::operator () (const unsigned i) const
{
   assert( i < n ) ;
   return coo[i] ;
}

//----------------------------------------------------------------------

template< class T, unsigned n > inline
T & TuplaG<T,n>::operator () (const unsigned i) 
{
   assert( i < n ) ;
   return coo[i] ;
}

//----------------------------------------------------------------------
// conversion a un puntero de lectura/escritura de tipo: T* ( T* p = tupla )

template< class T, unsigned n > inline 
TuplaG<T,n>::operator  T * () 
{
   return coo ;
}

//----------------------------------------------------------------------
// conversion a un puntero de solo lectura de tipo T* ( const T* p = tupla )

template< class T, unsigned n > inline 
TuplaG<T,n>::operator  const T * () const 
{
   return coo ;
}

//----------------------------------------------------------------------

template< class T, unsigned n > inline 
TuplaG<T,n> TuplaG<T,n>::operator + ( const TuplaG<T,n> & der ) const
{
   TuplaG<T,n> res ;
   for( unsigned i = 0 ; i < n ; i++ )
      res[i] = (*this)(i)+der(i) ;
   return res ;
}

//----------------------------------------------------------------------

template< class T, unsigned n > inline 
TuplaG<T,n> TuplaG<T,n>::operator - ( const TuplaG<T,n> & der ) const
{
   TuplaG<T,n> res ;
   for( unsigned i = 0 ; i < n ; i++ )
      res[i] = (*this)(i)-der(i) ;
   return res ;
}

//----------------------------------------------------------------------

// devuelve tupla negada ( v1 = -v2 )
template< class T, unsigned n > inline
TuplaG<T,n> TuplaG<T,n>::operator - (  ) const 
{
   TuplaG<T,n> res ;
   for( unsigned i = 0 ; i < n ; i++ )
      res[i] = -(*this)(i) ;
   return res ;
   
}

//----------------------------------------------------------------------

template< class T, unsigned n > inline
TuplaG<T,n> TuplaG<T,n>::operator * ( const T & a ) const 
{
   TuplaG<T,n> res ;
   for( unsigned i = 0 ; i < n ; i++ )
      res[i] = (*this)(i)*a ;
   return res ;
}

//----------------------------------------------------------------------

template< class T, unsigned n >  inline
TuplaG<T,n> TuplaG<T,n>::operator / ( const T & a ) const 
{
   TuplaG<T,n> res ;
   for( unsigned i = 0 ; i < n ; i++ )
      res[i] = (*this)(i)/a ;
   return res ;
}

//----------------------------------------------------------------------

template< class T, unsigned n > inline
TuplaG<T,n> operator * ( const T & a, const TuplaG<T,n> & der )  
{
   TuplaG<T,n> res ;
   for( unsigned i = 0 ; i < n ; i++ )
      res[i] = a*der(i) ;
   return res ;
}

//----------------------------------------------------------------------

template< class T, unsigned n > inline
std::ostream & operator <<  ( std::ostream & os, const TuplaG<T,n> & der ) 
{
   os << "(" ;
   for( unsigned i = 0 ; i < n ; i++ )
   {  os << der(i) ;
      if ( i+1 < n ) 
         os << "," ;
   }
   os << ")" ;
   return os ;   
}

//----------------------------------------------------------------------

// producto escalar (dot)  a = v1.dot(v2)
template< class T, unsigned n > inline
T TuplaG<T,n>::dot( const TuplaG<T,n> & v2 ) const 
{
   double res = 0.0 ;
   for( unsigned int i = 0 ; i < n ; i++ )
      res += double((*this)(i)) * double(v2(i)) ;
   return T(res) ;
}

//----------------------------------------------------------------------

// obtener longitud al cuadrado
template< class T, unsigned n > inline
T TuplaG<T,n>::lengthSq( ) const 
{
   return T( this->dot( *this ) ) ;
}

//----------------------------------------------------------------------
// operador binario para producto escalar

template< class T, unsigned n > inline
T TuplaG<T,n>::operator | ( const TuplaG & der ) const 
{
   return this->dot( der ) ;
}

// ---------------------------------------------------------------------

template< class T, unsigned n > inline
TuplaG<T,n> TuplaG<T,n>::normalized() const 
{
   T lenSq = T(0.0) ;
   for( unsigned i = 0 ; i < n ; i++ )
      lenSq += (*this)(i) * (*this)(i) ;
   
   if ( ! ( lenSq > 0.0 ) )
   {  using namespace std ;
      cout << "lenSq == " << lenSq << endl << flush ;
   }
   
   assert( lenSq > 0.0 ) ;
   
   return (*this)*(T(1.0/sqrt(double(lenSq)))) ;
   
}

// *********************************************************************
//
// plantilla de clase:
//
//   TuplaG2<T>
//
// *********************************************************************


template< class T > inline
TuplaG2<T>::TuplaG2(  ) 
{

}

// ---------------------------------------------------------------------

template< class T > inline
TuplaG2<T>::TuplaG2( const TuplaG<T,2> & ini )
{
   (*this)[0] = ini(0) ;
   (*this)[1] = ini(1) ;
}

// ---------------------------------------------------------------------

template< class T > inline
void TuplaG2<T>::operator = ( const TuplaG<T,2> & der ) 
{
   (*this)[0] = der(0) ;
   (*this)[1] = der(1) ;
}

// ---------------------------------------------------------------------

template< class T > inline
TuplaG2<T>::TuplaG2( const T & c0, const T & c1 ) 
{
   (*this)[0] = c0 ;
   (*this)[1] = c1 ;
}

// *********************************************************************
//
// plantilla de clase:
// TuplaG3<T>
//
// *********************************************************************


template< class T > inline
TuplaG3<T>::TuplaG3(  ) 
{

}

// ---------------------------------------------------------------------

template< class T > inline
TuplaG3<T>::TuplaG3( const TuplaG<T,3> & ini )
{
   (*this)[0] = ini(0) ;
   (*this)[1] = ini(1) ;
   (*this)[2] = ini(2) ;
}

// ---------------------------------------------------------------------

template< class T > inline
void TuplaG3<T>::operator = ( const TuplaG<T,3> & der ) 
{
   (*this)[0] = der(0) ;
   (*this)[1] = der(1) ;
   (*this)[2] = der(2) ;
}

// ---------------------------------------------------------------------

template< class T > inline
void TuplaG3<T>::operator = ( const TuplaG<T,4> & der ) 
{
   (*this)[0] = der(0) ;
   (*this)[1] = der(1) ;
   (*this)[2] = der(2) ;
}

// ---------------------------------------------------------------------

template< class T > inline
TuplaG3<T>::TuplaG3( const T & c0, const T & c1, const T & c2 ) 
{
   (*this)[0] = c0 ;
   (*this)[1] = c1 ;
   (*this)[2] = c2 ;
}
// ---------------------------------------------------------------------

template< class T > inline
TuplaG3<T> TuplaG3<T>::cross( const TuplaG3<T> & v2 ) const 
{
   // cuidado: no hay acceso a 'coo' tal cual, mirar:
   // http://stackoverflow.com/questions/7281072/accessing-public-members-of-base-class-fails
   
   return TuplaG3<T>(  (*this)(1)*v2(2) -  (*this)(2)*v2(1),
                       (*this)(2)*v2(0) -  (*this)(0)*v2(2), 
                       (*this)(0)*v2(1) -  (*this)(1)*v2(0)
                     );
}

// *********************************************************************
//
// plantilla de clase:
//
//   TuplaG4<T>
//
// *********************************************************************


template< class T > inline
TuplaG4<T>::TuplaG4(  ) 
{

}

// ---------------------------------------------------------------------

template< class T > inline
TuplaG4<T>::TuplaG4( const TuplaG<T,4> & ini )
{
   (*this)[0] = ini(0) ;
   (*this)[1] = ini(1) ;
   (*this)[2] = ini(2) ;
   (*this)[3] = ini(3) ;
}

// ---------------------------------------------------------------------

template< class T > inline
void TuplaG4<T>::operator = ( const TuplaG<T,4> & der ) 
{
   (*this)[0] = der(0) ;
   (*this)[1] = der(1) ;
   (*this)[2] = der(2) ;
   (*this)[3] = der(3) ;
}

// ---------------------------------------------------------------------

template< class T > inline
TuplaG4<T>::TuplaG4( const T& c0, const T& c1, const T& c2, const T& c3 ) 
{
   (*this)[0] = c0 ;
   (*this)[1] = c1 ;
   (*this)[2] = c2 ;
   (*this)[3] = c3 ;
}



// *********************************************************************
// implementaciones inline de matrizg

// ---------------------------------------------------------------------
// conversion a un puntero de lectura/escritura de tipo T* ( T* p = m )
// (devuelve puntero al primer elemento de la primera columna)
template< class T, unsigned n > inline
MatrizCG<T,n>::operator  T * ()
{
   return mat[0] ;
}

// ---------------------------------------------------------------------
// conversion a un puntero de solo lectura de tipo: const T* ( T* p = m )
// (devuelve puntero al primer elemento de la primera columna)
template< class T, unsigned n > inline
MatrizCG<T,n>::operator  const T * ()  const
{
   return & (mat(0)(0)) ;
}

//// ---------------------------------------------------------------------
//// acceso de lectura a una fila:
//template< class T, unsigned n > inline
//const TuplaG<T,n> & MatrizCG<T,n>::operator() ( const unsigned i ) const
//{
   //return mat(i) ;
//}

////----------------------------------------------------------------------

//template< class T, unsigned n > inline
//TuplaG<T,n> & MatrizCG<T,n>::operator[] ( int i )
//{
   //return mat[i] ;
//}

//----------------------------------------------------------------------

template< class T, unsigned n > inline
std::ostream & operator <<  ( std::ostream & os, const MatrizCG<T,n> & m )
{
   using namespace std ;

   ios estado_anterior( nullptr );
   estado_anterior.copyfmt( cout );
   cout << fixed << setprecision(9) ;

   const char * izq = "/||\\", * der = "\\||/" ;
   using namespace std ;
   for( unsigned fil = 0 ; fil < n ; fil++ )
   {  os << izq[fil] << " " ;
      for( unsigned col = 0 ; col < n ; col++ )
      {   os << setw(15) << m(fil,col) ;
          if ( col < n-1 ) os << " , " ;
      }
      os << " " << der[fil] << endl ;
   }

   cout.copyfmt( estado_anterior );
   return os ;
}

//----------------------------------------------------------------------
// componer una matriz con otra por la derecha

template< class T, unsigned n > inline
MatrizCG<T,n> MatrizCG<T,n>::operator * ( const MatrizCG<T,n> & der ) const
{
   MatrizCG<T,n> res ;

   for( unsigned fil = 0 ; fil < n ; fil++ )
   for( unsigned col = 0 ; col < n ; col++ )
   {  res(fil,col) = 0.0f ;
      for( unsigned k = 0 ; k < n ; k++ )
         res(fil,col) += (*this)(fil,k)*der(k,col) ;
   }
   return res ;
}

// ---------------------------------------------------------------------
// acceso de solo lectura usando fila,columna: T x = m(fil,col)

template< class T, unsigned n > inline
const T & MatrizCG<T,n>::operator()( const unsigned fil, const unsigned col ) const
{
   assert( fil < n );
   assert( col < n ) ;
   return mat(col)(fil) ;
}


// ---------------------------------------------------------------------
// acceso de lectura/escritura usando fila,columna: m(fil,col) = v

template< class T, unsigned n > inline
T & MatrizCG<T,n>::operator()( const unsigned fil, const unsigned col )
{
   assert( fil < n );
   assert( col < n ) ;
   return mat(col)(fil) ;
}

// ---------------------------------------------------------------------
// multiplicar esta matriz por una tupla por la derecha
template< class T, unsigned n > inline
TuplaG<T,n> MatrizCG<T,n>::operator * ( const TuplaG<T,n>  & t ) const
{
   TuplaG<T,n>  res ;
   for( unsigned fil = 0 ; fil < n ; fil++ )
   {  res[fil] = 0.0 ;
      for( unsigned col = 0 ; col < n ; col++ )
         res(fil) += (*this)(fil,col) * t(col) ;
   }
   return res ;
}

// ---------------------------------------------------------------------
// multiplicar esta matriz por una tupla por la derecha (con una dimesnión menos)
// (se añade un 1, se multiplica, y luego se le quita la ultima componente)

template< class T, unsigned n > inline
TuplaG<T,n-1> MatrizCG<T,n>::operator * ( const TuplaG<T,n-1>  & t ) const
{
   TuplaG<T,n> t1 ;

   // añadir un 1 a t, crear 't1'
   for( unsigned i = 0 ; i < n-1 ; i++ )
      t1(i) = t(i) ;
   t1(n-1) = T(1); // añadir un uno al final

   //// multiplicar por 't1', obtener 'resn'
   //for( unsigned fil = 0 ; fil < n ; fil++ )
   //{  resn(fil) = 0.0 ;
      //for( unsigned col = 0 ; col < n ; col++ )
         //resn(fil) += (*this)(fil,col) * t1(col) ;
   //}

   TuplaG<T,n> res1 = (*this)*t1 ;

   // copiar 'resn' en 'res'
   TuplaG<T,n-1>  res ;
   for( unsigned i = 0 ; i < n-1 ; i++ )
      res(i) = res1(i) ;

   return res ;
}

// *********************************************************************



// ---------------------------------------------------------------------
// Implementaciones de las funciones de matrices:

inline Matriz4f MAT_Ident(  )
{
   Matriz4f res ;
   for( unsigned fil = 0 ; fil < 4 ; fil++ )
   for( unsigned col = 0 ; col < 4 ; col++ )
      res(fil,col) = (col == fil) ? 1.0f : 0.0f ;

   return res ;
}

//----------------------------------------------------------------------

inline Matriz4f MAT_Filas( const Tupla3f & fila0, const Tupla3f & fila1, const Tupla3f & fila2 )
{
   Matriz4f res = MAT_Ident();

   for( unsigned col = 0 ; col < 3 ; col++ )
   {  res(0,col) = fila0(col) ;
      res(1,col) = fila1(col) ;
      res(2,col) = fila2(col) ;
   }
   return res ;
}
// ---------------------------------------------------------------------

// inline Matriz4f MAT_Traslacion( const float vec[3] )
// {
//    Matriz4f res = MAT_Ident();

//    for( unsigned fil = 0 ; fil < 3 ; fil++ )
//       res(fil,3) = vec[fil] ;

//    return res ;
// }

// ---------------------------------------------------------------------

inline Matriz4f MAT_Traslacion( const Tupla3f & d )
{
   Matriz4f res = MAT_Ident();

   res(0,3) = d(0) ;
   res(1,3) = d(1) ;
   res(2,3) = d(2) ;

   return res ;
}
// ---------------------------------------------------------------------

inline Matriz4f MAT_Escalado( const float sx, const float sy, const float sz )
{
   Matriz4f res = MAT_Ident();

   res(0,0) = sx ;
   res(1,1) = sy ;
   res(2,2) = sz ;

   return res ;
}

// ---------------------------------------------------------------------

inline Matriz4f MAT_Rotacion( const float ang_gra, const Tupla3f & eje )
{
   const Tupla3f ejen = eje.normalized() ;

   const double
      ang_rad = double(ang_gra)*double(2.0)*double(M_PI)/double(360.0) ,
      c       = cos(ang_rad),
      s       = sin(ang_rad);

   const double
      exn     = ejen[X],
      eyn     = ejen[Y],
      ezn     = ejen[Z],
      hx      = (1.0-c)*exn ,
      hy      = (1.0-c)*eyn ,
      hz      = (1.0-c)*ezn ;

   Matriz4f res = MAT_Ident();

   res(0,0) = hx*exn+c     ; res(0,1) = hx*eyn-s*ezn ; res(0,2) = hx*ezn+s*eyn ;
   res(1,0) = hy*exn+s*ezn ; res(1,1) = hy*eyn+c     ; res(1,2) = hy*ezn-s*exn ;
   res(2,0) = hz*exn-s*eyn ; res(2,1) = hz*eyn+s*exn ; res(2,2) = hz*ezn+c     ;

   return res ;
}

// ---------------------------------------------------------------------

// inline Matriz4f MAT_Rotacion( const float ang_gra, const Tupla3f & eje )
// {
//    return MAT_Rotacion( ang_gra, eje(0), eje(1), eje(2) );
// }

// ---------------------------------------------------------------------

inline Matriz4f MAT_LookAt( const Tupla3f & org, const Tupla3f & at, const Tupla3f & vup ) 
                          //( const float origen[3], const float centro[3], const float vup[3] )
{
   const Tupla3f n           = org-at ;
   const float   n_length_sq = n.lengthSq();
   assert( 0.0 < n_length_sq );

   Tupla3f eje[3] ; // array con los tres vectores de los ejes del S.R. de la cámara

   eje[Z] = n/std::sqrt( n_length_sq );           // eje Z desde el p.a. hacia el obs., normalizado
   eje[X] = ( vup.cross( eje[Z] )).normalized(),  // eje Z apunta en la dir. contraria a la camara
   eje[Y] = eje[Z].cross( eje[X] );               // eje Y perpendicular a los otros dos.

   Matriz4f
      trasl = MAT_Traslacion( -org ),
      rot   = MAT_Ident() ; // matriz de cambio de base mundo --> camara

   for( unsigned col = X ; col <= Z ; col++ )
   for( unsigned fil = X ; fil <= Z ; fil++ )
      rot(fil,col) = eje[fil](col) ;

   return rot * trasl ;
}
// ---------------------------------------------------------------------

inline Matriz4f MAT_Frustum( const float l, const float r, const float b, const float t, const float n, const float f )
{
   const float eps = 1e-6 ;
   assert( fabs(r-l) > eps && fabs(t-b) > eps  && fabs(n-f) > eps );

   const float
      irl = 1.0f/(r-l) ,
      itb = 1.0f/(t-b) ,
      inf = 1.0f/(n-f) ;
   const float
      a0 = 2.0f*n*irl ,
      a2 = (r+l)*irl,
      b1 = 2.0f*n*itb ,
      b2 = (t+b)*itb ,
      c2 = (n+f)*inf ,
      c3 = 2.0f*f*n*inf ;

   Matriz4f
      res ;

   res(0,0) = a0  ; res(0,1) = 0.0; res(0,2) = a2  ; res(0,3) = 0.0 ;
   res(1,0) = 0.0 ; res(1,1) = b1 ; res(1,2) = b2  ; res(1,3) = 0.0 ;
   res(2,0) = 0.0 ; res(2,1) = 0.0; res(2,2) = c2  ; res(2,3) = c3  ;
   res(3,0) = 0.0 ; res(3,1) = 0.0; res(3,2) = -1.0; res(3,3) = 0.0 ;

   return res ;
}

// ---------------------------------------------------------------------

inline Matriz4f MAT_Ortografica( const float l, const float r, 
                                 const float b, const float t, 
                                 const float n, const float f )
{
   const float eps = 1e-6 ;
   assert( fabs(r-l) > eps && fabs(t-b) > eps  && fabs(n-f) > eps );

   const float
      irl = 1.0f/(l-r) ,
      itb = 1.0f/(b-t) ,
      inf = 1.0f/(n-f) ;
   const float
      a0 = -2.0f*irl ,
      a3 = (r+l)*irl,
      b1 = -2.0f*itb ,
      b3 = (t+b)*itb ,
      c2 = 2.0f*inf ,
      c3 = (f+n)*inf ;

   Matriz4f
      res ;

   res(0,0) = a0  ; res(0,1) = 0.0 ; res(0,2) = 0.0 ; res(0,3) = a3  ;
   res(1,0) = 0.0 ; res(1,1) = b1  ; res(1,2) = 0.0 ; res(1,3) = b3  ;
   res(2,0) = 0.0 ; res(2,1) = 0.0 ; res(2,2) = c2  ; res(2,3) = c3  ;
   res(3,0) = 0.0 ; res(3,1) = 0.0 ; res(3,2) = 0.0 ; res(3,3) = 1.0 ;

   return res ;
}

// ---------------------------------------------------------------------

inline Matriz4f MAT_Perspectiva( const float fovy_grad, const float raz_asp, const float n, const float f )
{
   const float eps = 1e-6 ;
   assert( raz_asp > eps && fovy_grad > eps  && fabs(n-f) > eps );

   const float
      fovy_rad = fovy_grad*2.0f*M_PI/360.0f,
      t = n*tan(0.5*fovy_rad),
      r = t/raz_asp,
      b = -t ,
      l = -r ;

   return MAT_Frustum( l,r,b,t,n,f );
}


// ---------------------------------------------------------------------
// traspuesta (IGNORA LAS TRASLACIONES)

inline Matriz4f MAT_Transpuesta3x3( const Matriz4f & org )
{
   Matriz4f res = MAT_Ident() ;

   for( unsigned i = 0 ; i < 3 ; i++ )
   for( unsigned j = 0 ; j < 3 ; j++ )
      res(i,j) = org(j,i);

   return res ;
}
//----------------------------------------------------------------------
// construir la sub-matriz 3x3 dando sus tres filas

inline Matriz4f MAT_Filas( const Tupla3f fila[3] )
{
   Matriz4f res = MAT_Ident();

   for( unsigned col = 0 ; col < 3 ; col++ )
   {
      res(0,col) = fila[0](col) ;
      res(1,col) = fila[1](col) ;
      res(2,col) = fila[2](col) ;
   }
   return res ;
}
//----------------------------------------------------------------------
// construir la sub-matriz 3x3 dando sus tres columnas

inline Matriz4f MAT_Columnas( const Tupla3f colum[3] )
{
   Matriz4f res = MAT_Ident();

   for( unsigned fil = 0 ; fil < 3 ; fil++ )
   {
      res(fil,0) = colum[0](fil) ;
      res(fil,1) = colum[1](fil) ;
      res(fil,2) = colum[2](fil) ;
   }
   return res ;
}
// ---------------------------------------------------------------------
// matriz del viewport  (deja las Z igual: entre -1 y 1)

inline Matriz4f MAT_Viewport( int org_x, int org_y, int ancho, int alto )
{
   return MAT_Traslacion( { float(org_x), float(org_y), 0.0 } )*
          MAT_Escalado( float(ancho), float(alto), 1.0 )*
          MAT_Escalado( 0.5, 0.5, 1.0 )*
          MAT_Traslacion( { 1.0, 1.0, 1.0 }) ;
}
// ---------------------------------------------------------------------
// matriz inversa de la matriz del viewport

inline Matriz4f MAT_Viewport_inv( int org_x, int org_y, int ancho, int alto )
{
   return MAT_Traslacion( { -1.0, -1.0, -1.0 } ) *
          MAT_Escalado( 2.0, 2.0, 1.0 )*
          MAT_Escalado( 1.0/float(ancho), 1.0/float(alto), 1.0 )*
          MAT_Traslacion( { -float(org_x), -float(org_y), 0.0 } ) ;
}

// ---------------------------------------------------------------------
// matriz de vista, a partir del marco de coordenadas de la camara:
inline Matriz4f MAT_Vista( const Tupla3f eje[3], const Tupla3f& org )
{
   // traslación por -origen, seguida de productos escalares
   return MAT_Filas( eje )*MAT_Traslacion( -org ) ;
}
// ---------------------------------------------------------------------
// matriz inversa de la vista, a partir del marco de coordenadas de la camara:
inline Matriz4f MAT_Vista_inv( const Tupla3f eje[3], const Tupla3f& org )
{
   // rotaciones seguidas de traslación por origen
   return  MAT_Traslacion( org )*MAT_Columnas( eje );
}

// --------------------------------------------------
// métodos auxiliares para calcular la inversa de una matriz

// -----------------------------------------------------------------------
// calculo del determinante de la submatriz 3x3
inline float MAT_Determinante3x3( const Matriz4f & m )
{
   return + m(0,0)*m(1,1)*m(2,2)
          + m(0,1)*m(1,2)*m(2,0)
          + m(0,2)*m(1,0)*m(2,1)

          - m(0,0)*m(1,2)*m(2,1)
          - m(0,1)*m(1,0)*m(2,2)
          - m(0,2)*m(1,1)*m(2,0) ;
}

// --------------------------------------------------------------------
// calcula la inversa de una matriz 4x4 (sin términos de proyección)
// (la última fila debe ser 0, 0, 0, 1)

inline Matriz4f MAT_Inversa( const Matriz4f & m )
{
   assert( m(3,0) == 0.0 );
   assert( m(3,1) == 0.0 );
   assert( m(3,2) == 0.0 );
   assert( m(3,3) == 1.0 );

   // 1. calcular matriz de cofactores ('cofac')
   Matriz4f cofac = MAT_Ident();

   for( int i = 0 ; i < 3 ; i++ )
   for( int j = 0 ; j < 3 ; j++ )
   {
      const int i1 = (i+1)%3, i2 = (i+2)%3,
                j1 = (j+1)%3, j2 = (j+2)%3;
      cofac(i,j) = m(i1,j1)*m(i2,j2) - m(i1,j2)*m(i2,j1) ;
   }

   // 2. calcular determinante (det) (usando la primera fila de 'm' y de 'cofac')
   const float det = m(0,0)*cofac(0,0) +  m(0,1)*cofac(0,1) + m(0,2)*cofac(0,2) ;
   assert( 1e-6 < fabs(det) );
   const float inv_det = 1.0/det ;

   // 3. calcular la matriz inversa de la sub-matrix 3x3 (inv3x3) como la
   // adjunta (transpuesta de los cofactores), dividida por el determinante:
   Matriz4f inv3x3 = MAT_Ident();
   for( int i = 0 ; i < 3 ; i++ )
   for( int j = 0 ; j < 3 ; j++ )
      inv3x3(i,j) = inv_det * cofac(j,i) ;

   // 4. calcular la matriz de traslación inversa
   Matriz4f trasl_inv = MAT_Ident() ;
   for( int i = 0 ; i < 3 ; i++ )
      trasl_inv(i,3) = -m(i,3) ;

   // 5. calcular (y devolver) la inversa completa
   // (traslación inversa seguida de la inversa 3x3)
   return inv3x3 * trasl_inv;
}


// ---------------------------------------------------------------------
// juego de tests de la matrices

inline void MAT_TestInv( const Matriz4f & mat )
{
   using namespace std ;

   // print determinante, solo para test
   const float det = MAT_Determinante3x3( mat );
   cout << "det (1) == " << det << endl ;

   const Matriz4f inv = MAT_Inversa( mat );
   const Matriz4f ide = inv*mat ;

   cout << "mat == " << endl
        << mat << endl
        << "inv == " << endl
        << inv << endl
        << "ide == " << endl
        << ide << endl ;

}

inline void MAT_Tests()
{

   // test de la matriz inversa:
   const Matriz4f mat1 = MAT_Escalado( 0.5, 0.9, -1.1 )*
                        MAT_Rotacion( 34.0, { 1.0, 2.0, 3.0 } )*MAT_Traslacion( { 1.5, -2.2, -1.1 })*MAT_Escalado( -1.5, -2.7, 5.0 )*
                        MAT_Rotacion( -130.0, { -3.2, 2.0, -1.0 } )*MAT_Traslacion( { -0.8, 1.0, -1.7 } )*MAT_Escalado( 0.5, 2.0, -1.5 );

   const Matriz4f mt = MAT_Traslacion( { 1.0, 2.0, 3.0 }),
                  mr = MAT_Rotacion( -130.0, { -3.2, 2.0, -1.0 } ),
                  me = MAT_Escalado( 2.0, -1.0, 0.7 ) ;

   MAT_TestInv( me*mat1*mt*mr*me );
}




} // fin namespace tup_mat

#endif // ifndef TUP_MAT_H