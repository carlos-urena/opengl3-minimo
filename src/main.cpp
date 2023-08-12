// Ejemplo mínimo de código OpenGL, usa OpenGL 3.3 + GLSL 3.3 + GLFW



// includes de la librería estándard de C++
#include <cassert>   // 'assert' (enforce preconditions)
#include <cstring>   // 'strlen' (to compile shaders)
#include <iostream>  // 'cout' and such
#include <iomanip>   // set precision and such
#include <vector>    // 'std::vector' types

// incluir cabeceras de OpenGL y GLM
#include "glincludes.h"

// incluir cabeceras auxiliares para shaders, vaos y vbos.
#include "shaders.h"     // ProgramObject and BasicProgramObject
#include "vaos-vbos.h"   // AttribsVBO, IndexesVBO, DescrVAO

// ---------------------------------------------------------------------------------------------
// Constantes y variables globales

// constexpr GLuint
//     ind_atrib_posiciones = 0,      // índice del atributo de vértice con su posiciones (debe ser el índice 0, siempre)
//     ind_atrib_colors    = 1,      // índice del atributo de vértice con su color RGB
//     num_atribs           = 2 ;     // número de atributos que gestionan los shaders
bool
    redibujar_ventana   = true ,   // puesto a true por los gestores de eventos cuando cambia el modelo y hay que regenerar la vista
    terminar_programa   = false ;  // puesto a true en los gestores de eventos cuando hay que terminar el programa
GLFWwindow *
    ventana_glfw        = nullptr; // puntero a la ventana GLFW
int
    ancho_actual        = 512 ,    // ancho actual del framebuffer, en pixels
    alto_actual         = 512 ;    // alto actual del framebuffer, en pixels
DescrVAO
    * vao_ind          = nullptr , // identificador de VAO (vertex array object) para secuencia indexada
    * vao_no_ind       = nullptr , // identificador de VAO para secuencia de vértices no indexada
    * vao_glm          = nullptr ; // identificador de VAO para secuencia de vértices guardada en vectors de vec3
Cauce 
    * cauce            = nullptr ; // puntero al objeto de la clase 'Cauce' en uso.


// ---------------------------------------------------------------------------------------------
// función que se encarga de visualizar un triángulo relleno en modo diferido,
// no indexado, usando la clase 'DescrVAO' (declarada en 'vaos-vbos.h')
// el triángulo se dibuja en primer lugar relleno con colores, y luego las aristas en negro


void DibujarTriangulo_NoInd( )
{
    assert( glGetError() == GL_NO_ERROR );

    // la primera vez, crear e inicializar el VAO
    if ( vao_no_ind == nullptr )
    {
        // número de vértices que se van a dibujar
        constexpr unsigned num_verts = 3 ;

        // tablas de posiciones y colores de vértices (posiciones en 2D, con Z=0)
        const GLfloat
            posiciones[ num_verts*2 ] = {  -0.8, -0.8,      +0.8, -0.8,     0.0, 0.8      },
            colores   [ num_verts*3 ] = {  1.0, 0.0, 0.0,   0.0, 1.0, 0.0,  0.0, 0.0, 1.0 };

        // Crear VAO con posiciones, colores e indices
        vao_no_ind = new DescrVAO( cauce->num_atribs, GL_FLOAT, 2, num_verts, posiciones );
        vao_no_ind->addAttrib( cauce->ind_atrib_colors, GL_FLOAT, 3, num_verts, colores );    
    }
    
    assert( glGetError() == GL_NO_ERROR );

    // duibujar relleno usando los colores del VAO
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    cauce->setUseFlatColor( false );
    vao_no_ind->habilitarAttrib( cauce->ind_atrib_colors, true );
    vao_no_ind->draw( GL_TRIANGLES );

    assert( glGetError() == GL_NO_ERROR );

    // dibujar las líneas usando color negro
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    cauce->setUseFlatColor( true );
    cauce->setColor( { 0.0, 0.0, 0.0 });
    vao_no_ind->habilitarAttrib( cauce->ind_atrib_colors, false );
    vao_no_ind->draw( GL_TRIANGLES );

    assert( glGetError() == GL_NO_ERROR );
}

// ---------------------------------------------------------------------------------------------
// función que se encarga de visualizar un triángulo  en modo diferido,
// indexado, usando la clase  'DescrVAO' (declarada en vaos-vbos.h)
// el triángulo se dibuja en primer lugar relleno con colores, y luego las aristas en negro

void DibujarTriangulo_Ind( )
{
    assert( glGetError() == GL_NO_ERROR );

    if ( vao_ind == nullptr )
    {
         // número de vértices e índices que se van a dibujar
        constexpr unsigned num_verts = 3, num_inds  = 3 ;

        // tablas de posiciones y colores de vértices (posiciones en 2D, con Z=0)
        const GLfloat
            posiciones[ num_verts*2 ] = {  -0.4, -0.4,      +0.4, -0.4,     0.0, +0.4      },
            colores   [ num_verts*3 ] = {  1.0, 0.0, 0.0,   0.0, 1.0, 0.0,  0.0, 0.0, 1.0 } ;
        const GLuint
            indices   [ num_inds    ] = { 0, 1, 2 };

        vao_ind = new DescrVAO( cauce->num_atribs, GL_FLOAT, 2, num_verts, posiciones );
        vao_ind->addAttrib( cauce->ind_atrib_colors, GL_FLOAT, 3, num_verts, colores ) ;
        vao_ind->addIndices( GL_UNSIGNED_INT, num_inds, indices );
    }
   
    assert( glGetError() == GL_NO_ERROR );
    
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    cauce->setUseFlatColor( false );
    vao_ind->habilitarAttrib( cauce->ind_atrib_colors, true );
    vao_ind->draw( GL_TRIANGLES );

    assert( glGetError() == GL_NO_ERROR );
   
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    cauce->setColor( { 0.0, 0.0, 0.0 });
    vao_ind->habilitarAttrib( cauce->ind_atrib_colors, false );
    vao_ind->draw( GL_TRIANGLES );

    assert( glGetError() == GL_NO_ERROR );
}

// ---------------------------------------------------------------------------------------------
// función que se encarga de visualizar un triángulo relleno en modo diferido,
// usando vectores con entradas de tipos GLM (vec2, vec3, uvec3)
// el triángulo se dibuja en primer lugar relleno con colores, y luego las aristas en negro

void DibujarTriangulo_glm( )
{    
    using namespace std ;
    using namespace glm ;

    assert( glGetError() == GL_NO_ERROR );

    if ( vao_glm == nullptr )
    {

        // tablas de posiciones y colores de vértices (posiciones en 2D, con Z=0)
        const vector<vec2>   posiciones = {  {-0.4, -0.4},     {+0.42, -0.47},   {0.1, +0.37}    };
        const vector<vec3>   colores    = {  {1.0, 1.0, 0.0},  {0.0, 1.0, 1.0},  {1.0, 0.0, 1.0} };
        const vector<uvec3>  indices    = {  { 0, 1, 2 }};   // (un único triángulo)      

        vao_glm = new DescrVAO( cauce->num_atribs, posiciones );
        vao_glm->addAttrib( cauce->ind_atrib_colors, colores ) ;
        vao_glm->addIndices( indices );

        assert( glGetError() == GL_NO_ERROR );
    }
   
    assert( glGetError() == GL_NO_ERROR );
    
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    cauce->setUseFlatColor( false );
    vao_glm->habilitarAttrib( cauce->ind_atrib_colors, true );
    vao_glm->draw( GL_TRIANGLES );

    assert( glGetError() == GL_NO_ERROR );
   
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    cauce->setColor( { 0.0, 0.0, 0.0 });
    vao_glm->habilitarAttrib( cauce->ind_atrib_colors, false );
    vao_glm->draw( GL_TRIANGLES );

    assert( glGetError() == GL_NO_ERROR );
}

// ---------------------------------------------------------------------------------------------
// función que se encarga de visualizar el contenido en la ventana

void VisualizarFrame( )
{
    using namespace std ;
    using namespace glm ;

    // comprobar y limpiar variable interna de error
    assert( glGetError() == GL_NO_ERROR );

    // usar (acrivar) el objeto programa (no es necesario hacerlo en 
    // cada frame si solo hay uno de estos objetos, pero se incluye 
    // para hacer explícito que el objeto programa debe estar activado)
    cauce->activar();

    // establece la zona visible (toda la ventana)
    glViewport( 0, 0, ancho_actual, alto_actual );

    // fija la matriz de transformación de posiciones de los shaders 
    // (la hace igual a la matriz identidad)
    cauce->resetMM();

    // fija la matriz de proyeccion (la hace igual a la matriz identidad)
    cauce->setProjectionMatrix( glm::mat4(1.0) );

    // limpiar la ventana
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // habilitar EPO por Z-buffer (test de profundidad)
    glDisable( GL_DEPTH_TEST );

    // Dibujar un triángulo, es una secuncia de vértice no indexada.
    DibujarTriangulo_NoInd();

    // usa el color plano para el segundo triángulo
    cauce->setUseFlatColor( true );

    // dibujar triángulo indexado (rotado y luego desplazado) 
    cauce->pushMM();
        cauce->compMM( translate( vec3{ 0.4f, 0.1f, -0.1f}  ));
        cauce->compMM( rotate(  radians(23.0f), vec3{ 0.0f, 0.0f, 1.0f}   ));
        DibujarTriangulo_Ind();     // indexado
    cauce->popMM();

    // dibujar un triángulo usando vectores de GLM
    DibujarTriangulo_glm() ;

    // comprobar y limpiar variable interna de error
    assert( glGetError() == GL_NO_ERROR );

    // esperar a que termine 'glDrawArrays' y entonces presentar el framebuffer actualizado
    glfwSwapBuffers( ventana_glfw );

}


// ---------------------------------------------------------------------------------------------
// función que se invoca cada vez que cambia el número de pixels del framebuffer
// (cada vez que se redimensiona la ventana)

void FGE_CambioTamano( GLFWwindow* ventana, int nuevo_ancho, int nuevo_alto )
{
    using namespace std ;
    cout << "FGE cambio tamaño, nuevas dimensiones: " << nuevo_ancho << " x " << nuevo_alto << "." << endl ;
    ancho_actual      = nuevo_ancho ;
    alto_actual       = nuevo_alto ;
    redibujar_ventana = true ; // fuerza a redibujar la ventana
}
// ---------------------------------------------------------------------------------------------
// función que se invocará cada vez que se pulse o levante una tecla.

void FGE_PulsarLevantarTecla( GLFWwindow* ventana, int key, int scancode, int action, int mods )
{
    using namespace std ;
    cout << "FGE pulsar levantar tecla, número de tecla == " << key << "." << endl ;
    // si se pulsa la tecla 'ESC', acabar el programa
    if ( key == GLFW_KEY_ESCAPE )
        terminar_programa = true ;
}
// ---------------------------------------------------------------------------------------------
// función que se invocará cada vez que se pulse o levante un botón del ratón

void FGE_PulsarLevantarBotonRaton( GLFWwindow* ventana, int button, int action, int mods )
{
    // nada, por ahora

}
// ---------------------------------------------------------------------------------------------
// función que se invocará cada vez que cambie la posición del puntero

void FGE_MovimientoRaton( GLFWwindow* ventana, double xpos, double ypos )
{
    // nada, por ahora
}
// ---------------------------------------------------------------------------------------------
// función que se invocará cada vez que mueva la rueda del ratón.

void FGE_Scroll( GLFWwindow* ventana, double xoffset, double yoffset )
{
    // nada, por ahora
}
// ---------------------------------------------------------------------------------------------
// función que se invocará cuando se produzca un error de GLFW

void ErrorGLFW ( int error, const char * description )
{
    using namespace std ;
    cerr
        << "Error en GLFW. Programa terminado" << endl
        << "Código de error : " << error << endl
        << "Descripción     : " << description << endl ;
    exit(1);
}
// ---------------------------------------------------------------------------------------------
// código de inicialización de GLFW

void InicializaGLFW( int argc, char * argv[] )
{
    using namespace std ;

    // intentar inicializar, terminar si no se puede
    glfwSetErrorCallback( ErrorGLFW );
    if ( ! glfwInit() )
    {
        cout << "Imposible inicializar GLFW. Termino." << endl ;
        exit(1) ;
    }

    // especificar versión de OpenGL y parámetros de compatibilidad que se querrán
   // (pedimos opengl 330, tipo "core" (sin compatibilidad con versiones anteriores)
   glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
   glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 ); 
   glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE ); // indica que tambien debe funcionar si se usa con un driver con version superior a la 3.3
   glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE ); // indica que no es compatible hacia atrás con versiones previas a la 3.3

    // especificar que función se llamará ante un error de GLFW
    glfwSetErrorCallback( ErrorGLFW );

    // crear la ventana (var. global ventana_glfw), activar el rendering context
    ventana_glfw = glfwCreateWindow( 512, 512, "IG ejemplo mínimo (OpenGL 3+)", nullptr, nullptr );
    glfwMakeContextCurrent( ventana_glfw ); // necesario para OpenGL

    // leer y guardar las dimensiones del framebuffer en pixels
    glfwGetFramebufferSize( ventana_glfw, &ancho_actual, &alto_actual );

    // definir cuales son las funciones gestoras de eventos (con nombres 'FGE_....')
    glfwSetFramebufferSizeCallback( ventana_glfw, FGE_CambioTamano );
    glfwSetKeyCallback            ( ventana_glfw, FGE_PulsarLevantarTecla );
    glfwSetMouseButtonCallback    ( ventana_glfw, FGE_PulsarLevantarBotonRaton);
    glfwSetCursorPosCallback      ( ventana_glfw, FGE_MovimientoRaton );
    glfwSetScrollCallback         ( ventana_glfw, FGE_Scroll );
}

// ---------------------------------------------------------------------------------------------
// función para inicializar GLEW (necesario para las funciones de OpenGL ver 2.0 y posteriores)
// en macOS no es necesario (está vacía)

void InicializaGLEW()
{
#ifndef __APPLE__
    using namespace std ;
    GLenum codigoError = glewInit();
    if ( codigoError != GLEW_OK ) // comprobar posibles errores
    {
        cout << "Imposible inicializar ’GLEW’, mensaje recibido: " << endl
             << (const char *)glewGetErrorString( codigoError ) << endl ;
        exit(1);
    }
    else
        cout << "Librería GLEW inicializada correctamente." << endl << flush ;

#endif
}

// ---------------------------------------------------------------------------------------------

void InicializaOpenGL()
{
    using namespace std ;
    
    assert( glGetError() == GL_NO_ERROR );

    cout  << "Datos de versión e implementación de OpenGL" << endl
         << "    Implementación de : " << glGetString(GL_VENDOR)  << endl
         << "    Hardware          : " << glGetString(GL_RENDERER) << endl
         << "    Versión de OpenGL : " << glGetString(GL_VERSION) << endl
         << "    Versión de GLSL   : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl ;

    
    InicializaGLEW(); // En linux y windows, fija punteros a funciones de OpenGL version 2.0 o superiores
    
    
    assert( glGetError() == GL_NO_ERROR );
    
    glClearColor( 1.0, 1.0, 1.0, 0.0 ); // color para 'glClear' (blanco, 100% opaco)
    glDisable( GL_CULL_FACE );          // dibujar todos los triángulos independientemente de su orientación
    cauce = new Cauce() ;            // crear el objeto programa (variable global 'cauce')
    
    assert( cauce != nullptr );
    assert( glGetError() == GL_NO_ERROR );
}
// ---------------------------------------------------------------------------------------------

void BucleEventosGLFW()
{
    while ( ! terminar_programa )
    {   
        if ( redibujar_ventana )
        {   
            VisualizarFrame();
            redibujar_ventana = false; // (evita que se redibuje continuamente)
        }
        glfwWaitEvents(); // esperar evento y llamar FGE (si hay alguna)
        terminar_programa = terminar_programa || glfwWindowShouldClose( ventana_glfw );
    }
}
// ---------------------------------------------------------------------------------------------

int main( int argc, char *argv[] )
{
    using namespace std ;
    cout << "Programa mínimo de OpenGL 3.3 o superior" << endl ;

    InicializaGLFW( argc, argv ); // Crea una ventana, fija funciones gestoras de eventos
    InicializaOpenGL() ;          // Compila vertex y fragment shaders. Enlaza y activa programa. Inicializa GLEW.
    BucleEventosGLFW() ;          // Esperar eventos y procesarlos hasta que 'terminar_programa == true'
    glfwTerminate();              // Terminar GLFW (cierra la ventana)

    cout << "Programa terminado normalmente." << endl ;
}
