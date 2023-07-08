## --------------------------------------------------------------------------------
## Informática Gráfica, curso 23-24, ejemplo de OpenGL mínimo.
##
## Archivo de configuración para compilar usando CMake en Windows,
## Asume que las librerías necesarias se han instalado con 'vcpkg'
## última modificación: 25 de Junio del 2023
##
## --------------------------------------------------------------------------------

cmake_minimum_required (VERSION 3.5.1)
set( CMAKE_TOOLCHAIN_FILE ~/vcpkg/scripts/buildsystems/vcpkg.cmake)  ## permite a 'find_package' encontrar librerías instaladas con VCPKG
project( opengl3_minimo )

## Opciones para el compilador (configurables)

set( carpeta_fuentes    "../../src" )
set( carpeta_ejecutable "../bin" )
set( nombre_ejecutable  "opengl3_minimo" )
set( opcs_warnings      "/wd4244 /wd4305 /wd4267" ) ## deshabilitar warnings de conversión implicita double --> float, size_t --> int (este ultimo es dudoso..)
set( opcs_primer_error  "" ) ## "-Wfatal-errors"  --> no hay nada equivalente a esto en el compilador de microsoft, no se puede parar con el 1er error
set( flags_compilador   "/std:c++20  /MT /O2 ${opcs_warnings} ${opcs_primer_error}" ) 

## definir donde están las unidades de compilación y los headers

file( GLOB unidades  ${carpeta_fuentes}/*.cpp  )
file( GLOB cabeceras ${carpeta_fuentes}/*.h    )

## buscar librerías en el sistema y añadirlas a todos los targets

find_package( GLEW REQUIRED )
find_package( glfw3 CONFIG REQUIRED )
link_libraries( GLEW::GLEW glfw )

##  definir flags para compilador 

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flags_compilador}")
include_directories( ${carpeta_fuentes})

## definir el ejecutable

add_executable            ( ${nombre_ejecutable} ${unidades} ${cabeceras} )
set_target_properties     ( ${nombre_ejecutable} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${carpeta_ejecutable} )



