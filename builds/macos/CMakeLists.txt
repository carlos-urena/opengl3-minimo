## --------------------------------------------------------------------------------
## Ejemplo OpenGL mínimo, versión OpenGL+GLSL 3.3
##
## Archivo de configuración para compilar usando CMake en macOS
## creado: 26-Abril de 2023
##
## --------------------------------------------------------------------------------

cmake_minimum_required (VERSION 3.5.1)
project (main_macos_exe)

set ( ejecutable       "${PROJECT_NAME}" )
set ( flags_compilador "-g -std=c++17 -Wall -Wfatal-errors -Wno-deprecated-declarations" ) 
set ( flags_enlazador  "")
set ( carpeta_fuentes  "../../src")

file (GLOB unidades  ${carpeta_fuentes}/*.cpp )
file (GLOB cabeceras ${carpeta_fuentes}/*.h   )

add_executable( ${ejecutable} ${unidades} ${cabeceras} )

set (CMAKE_CXX_FLAGS        "${CMAKE_CXX_FLAGS}        ${flags_compilador}"  )
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${flags_enlazador}"   )

find_package( glfw3 3.3 REQUIRED )
target_link_libraries( ${ejecutable} OpenGL::GL )

find_package( OpenGL REQUIRED )
target_link_libraries( ${ejecutable} glfw )






