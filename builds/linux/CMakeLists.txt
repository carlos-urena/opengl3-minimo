## --------------------------------------------------------------------------------
## Ejemplo OpenGL mínimo, versión OpenGL+GLSL 3.3
##
## Archivo de configuración para compilar usando CMake en Linux
## creado: 26-Abril de 2023
## actualizaciones:
##    2023 Jun 19 : separo carpetas 'cmake' y 'bin', añado targets debug y release
##
## --------------------------------------------------------------------------------

cmake_minimum_required (VERSION 3.5.1)
project (opengl3_minimo_linux)

## diversos aspectos configurables

set ( carpeta_fuentes     "../../src")
set ( flags_compilador    "-std=c++20 -Wall -Wfatal-errors -Wno-deprecated-declarations" ) 
set ( nombre_exe_debug    "debug_exe" )
set ( nombre_exe_release  "release_exe" )

## definir directorios donde están los fuentes

file ( GLOB unidades  ${carpeta_fuentes}/*.cpp )
file ( GLOB cabeceras ${carpeta_fuentes}/*.h   )

## fijar las opciones que se van a usar para compilar y enlazar

set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${flags_compilador} -I${carpeta_fuentes}")
set( OpenGL_GL_PREFERENCE "GLVND" ) ## usado en 'find_package( OpenGL )',  sirve para seleccionar Nvidia en ordenadores con GPU discreta nvidia e integrada INTEL 

## definir las librerías que se van e enlazar

find_package( GLEW REQUIRED )
find_package( OpenGL REQUIRED )
find_package( glfw3 3.3 REQUIRED )

link_libraries( glfw GLEW OpenGL::GL)

## definir ejecutables: 'debug_exe' (opción '-g') y 'release_exe' (opción '-O3')

add_executable       ( ${nombre_exe_debug} ${unidades} ${cabeceras} )
set_target_properties( ${nombre_exe_debug} PROPERTIES COMPILE_FLAGS "-g" )

add_executable       ( ${nombre_exe_release} EXCLUDE_FROM_ALL ${unidades} ${cabeceras} )
set_target_properties( ${nombre_exe_release} PROPERTIES COMPILE_FLAGS "-O3" )

set_target_properties( ${nombre_exe_debug} ${nombre_exe_release} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../bin" )

