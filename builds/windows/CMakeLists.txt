## --------------------------------------------------------------------------------
## Informática Gráfica, curso 23-24, ejemplo de OpenGL mínimo.
##
## Archivo de configuración para compilar usando CMake en windows,
## Asume que las librerías necesarias se han instalado con 'vcpkg'
## última modificación: 24 de Abril del 2023
##
## --------------------------------------------------------------------------------

cmake_minimum_required (VERSION 3.5.1)
set( CMAKE_TOOLCHAIN_FILE ~/vcpkg/scripts/buildsystems/vcpkg.cmake)
project (opengl3_minimo_windows)

## Opciones para el compilador (configurables)

set (ejecutable "opengl3_minimo_windows")  ## nombre del ejecutable
set (opcs_warnings "/wd4244 /wd4305 /wd4267") ## deshabilitar warnings de conversión implicita double --> float, size_t --> int (este ultimo es dudoso..)
set (opcs_primer_error "") ##"-Wfatal-errors")  --> no hay nada equivalente a esto en el compilador de microsoft, no se puede parar con el 1er error

## Paths a las distintas carpetas
set (dir_src   "../../src")

## definir donde están las unidades de compilación y los headers

file (GLOB unidades    ${dir_src}/*.cpp  )
file (GLOB cabeceras   ${dir_src}/*.h    )

##  definir flags para compilador y enlazador
##  (dependen del sistema operativo, en este caso windows)

set (flags_compilador "/std:c++17  /MT ${opcs_warnings} ${opcs_primer_error}")  
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flags_compilador}")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:MSVCRT /NODEFAULTLIB:LIBCMT")

## definir el ejecutable: unidades (archivos .cpp) y cabeceras (.h)  que se deben compilar

add_executable( ${ejecutable} ${unidades} ${cabeceras} )

## buscar librerías en el sistema 
## añado: GLEW, GLFW, libjpeg

find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

## añadir los flags necesarios para las librerías (para includes y para enlazar)

target_include_directories( ${ejecutable} PUBLIC  ${dir_src})
target_link_libraries( ${ejecutable} PRIVATE GLEW::GLEW glfw   )



