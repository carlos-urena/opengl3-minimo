// OpenGL+GLFW include directives, 
// uses OpenGL 3.3 + GLSL 3.3 + GLFW 3
// also includes 'tup_mat'

#ifndef GL_INCLUDES_H
#define GL_INCLUDES_H

// includes de la librería estándard de C++
#include <cassert>   // 'assert' (enforce preconditions)
#include <cstring>   // 'strlen' (to compile shaders)
#include <iostream>  // 'cout' and such
#include <iomanip>   // set precision and such
#include <vector>    // 'std::vector' types

// includes de GLM
#include <glm/glm.hpp>               // incluye cosas básicas de GLM (vectores y matrices)
#include <glm/gtc/type_ptr.hpp>      // conversion de vectores y matrices hacia/desde 'float *' con 'value_ptr' (https://glm.g-truc.net/0.9.9/api/a00305.html)
#include <glm/gtc/matrix_access.hpp> // acceso a filas y columnas de una matriz, con glm::row( mat4 )
#include <glm/gtx/string_cast.hpp>   // función 'to_string'
#include <glm/gtx/transform.hpp>     // funciones 'scale', 'rotate', 'translate' sin especificar una matriz original (https://glm.g-truc.net/0.9.9/api/a00362.html)


#ifdef __linux__
// -----------------------------------------------
// Includes y definiciones específicos en Linux
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#else
#ifdef __APPLE__
//------------------------------------------------
// Includes y definiciones específicos en macOS
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
#else
#ifdef _WIN32
//------------------------------------------------
// Includes y definiciones específicos en Windows
// tener en cuenta esto:
//  https://www.glfw.org/docs/3.0/build.html#build_include
//  https://www.glfw.org/docs/3.0.3/build.html
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// ------------------------------------------------
#else
// Emitir error por sistema operativo no soportado
#error "No puedo determinar el sistema operativo, o no esta soportado"
#endif
#endif
#endif


/// aux function to print vectors and matrices

template<class T>
inline std::ostream & operator<< (std::ostream &os, const glm::tvec3<T> &v ) 
{
    return os << "( " << v[0] << ", " << v[1] << ", " << v[2] << " )" ; 
}

template<class T>
inline std::ostream & operator<< (std::ostream &os, const glm::tvec2<T> &v ) 
{
    return os << "( " << v[0] << ", " << v[1] << " )" ;    
}

template<class T>
inline std::ostream & operator<< (std::ostream &os, const glm::tmat4x4<T> &m ) 
{
    using namespace std ;

    const char * izq = "/||\\", * der = "\\||/" ;
    ios estado_anterior( nullptr );

    estado_anterior.copyfmt( os );
    os << endl << fixed << setprecision(7) << showpos ;

    for( unsigned i = 0 ; i < 4 ; i++ )
    {  
        os << izq[i] << " " ;
        for( unsigned j = 0 ; j < 4 ; j++ )
        {   os << setw(15) << m[i][j] ;
            if ( j < 3 ) os << ", " ;
        }
        os << " " << der[i] << endl ;
    }

    os.copyfmt( estado_anterior );
    return os ;
}


#endif // GL_INCLUDES_H
