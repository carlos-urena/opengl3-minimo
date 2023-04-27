## --------------------------------------------------------------------------------
## Ejemplo OpenGL mínimo, versión OpenGL+GLSL 3.3
##
## Archivo de configuración para compilar usando CMake en macOS
## creado: 26-Abril de 2023
##
## --------------------------------------------------------------------------------

cmake_minimum_required (VERSION 3.5.1)
project (main_linux_exe)
set ( ejecutable "${PROJECT_NAME}" )
set ( carpeta_fuentes  "../../src")

set ( flags_compilador "-g -std=c++11 -Wall -Wfatal-errors -Wno-deprecated-declarations" ) 

file (GLOB unidades  ${carpeta_fuentes}/*.cpp )
file (GLOB cabeceras ${carpeta_fuentes}/*.h   )

set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${flags_compilador}")
set( OpenGL_GL_PREFERENCE "GLVND" ) ## sirve para seleccionar Nvidia en ordenadores con GPU discreta nvidia e integrada INTEL 

find_package( GLEW REQUIRED )
find_package( OpenGL REQUIRED )
find_package( glfw3 3.3 REQUIRED )

add_executable ( ${ejecutable} ${unidades} ${cabeceras} )
target_link_libraries( ${ejecutable} glfw GLEW OpenGL::GL)
