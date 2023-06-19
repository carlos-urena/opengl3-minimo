## --------------------------------------------------------------------------------
## Ejemplo OpenGL mínimo, versión OpenGL+GLSL 3.3
##
## Archivo de configuración para compilar usando CMake en Linux
## creado: 26-Abril de 2023
##
## --------------------------------------------------------------------------------

cmake_minimum_required (VERSION 3.5.1)
project (opengl3_minimo_linux_exe)

## diversos aspectos configurables

set ( carpeta_fuentes     "../../src")
set ( flags_compilador    "-std=c++20 -Wall -Wfatal-errors -Wno-deprecated-declarations" ) 
set ( nombre_exe_debug    "debug_exe" )
set ( nombre_exe_release  "release_exe" )

## definir directorios donde están los fuentes

file ( GLOB unidades  ${carpeta_fuentes}/*.cpp )
file ( GLOB cabeceras ${carpeta_fuentes}/*.h   )

## fijar las opciones que se van a usar para compilar y enlazar

set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${flags_compilador}")
set( OpenGL_GL_PREFERENCE "GLVND" ) ## usado en 'find_package( OpenGL )',  sirve para seleccionar Nvidia en ordenadores con GPU discreta nvidia e integrada INTEL 

## buscar las librerías que se van a usar 

find_package( GLEW REQUIRED )
find_package( OpenGL REQUIRED )
find_package( glfw3 3.3 REQUIRED )

## definir el ejecutable de 'debug' (se usa la opción adicional: -g)
## se compila con 'make' 

add_executable       ( ${nombre_exe_debug} ${unidades} ${cabeceras} )
set_target_properties( ${nombre_exe_debug} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../bin" )
set_target_properties( ${nombre_exe_debug} PROPERTIES COMPILE_FLAGS "-g" )
target_link_libraries( ${nombre_exe_debug} glfw GLEW OpenGL::GL)


## definir el ejecutable de 'release' (se usa la opción adicional: -O3)
## se compila con 'make release_exe'

add_executable       ( ${nombre_exe_release} EXCLUDE_FROM_ALL ${unidades} ${cabeceras} )
set_target_properties( ${nombre_exe_release} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../bin" )
set_target_properties( ${nombre_exe_release} PROPERTIES COMPILE_FLAGS "-O3" )
target_link_libraries( ${nombre_exe_release} glfw GLEW OpenGL::GL)
