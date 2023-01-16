## --------------------------------------------------------------------------------
## Prácticas de Informática Gráfica, curso 22-23
##
## Archivo de configuración para compilar usando CMake en macOS
## última modificación: 14-Julio de 2022
##
## --------------------------------------------------------------------------------

cmake_minimum_required (VERSION 3.5.1)
project (ogl3min_windows)

##
## Opciones para el compilador (configurables)
##

set (opcs_warnings "/wd4244 /wd4305 /wd4267") ## deshabilitar warnings de conversión implicita double --> float, size_t --> int (este ultimo es dudoso..)
set (opcs_primer_error "") ##"-Wfatal-errors")  --> no hay nada equivalente a esto en el compilador de microsoft, no se puede parar con el 1er error

##
## Paths a las distintas carpetas
##

set (raiz       "../..")    ## relativa a la carpeta donde está este CMakefile ??
set (src        "${raiz}/src")
set (ejecutable "ogl3min_windows")

##
##  definir flags para compilador y enlazador
##  (dependen del sistema operativo, en este caso windows)
##

set (raiz_libs "C:/Users/carlo/Downloads")

set (glew_raiz "${raiz_libs}/glew-2.1.0-win32/glew-2.1.0")
set (glfw_raiz "${raiz_libs}/glfw-3.3.8.bin.WIN64/glfw-3.3.8.bin.WIN64")
set (jpeg_raiz "${raiz_libs}/libjpeg-turbo64")

set (glew_inc "${glew_raiz}/include")
set (glfw_inc "${glfw_raiz}/include")
set (jpeg_inc "${jpeg_raiz}/include")
set (libs_inc "${glew_inc}" "${glfw_inc}" "${jpeg_inc}" )

set (glew_lib "${glew_raiz}/lib/Release/x64/glew32s.lib") ## s --> versión estática, requiere #define GLEW_STATIC antes de #include glew ??
set (glfw_lib "${glfw_raiz}/lib-vc2022/glfw3.lib")
set (jpeg_lib "${jpeg_raiz}/lib/jpeg-static.lib") ## prefiero versión 'static', si está.
set (opgl_lib "opengl32.lib")  ## ya viene incluido en VS o en el driver de nvidia, debe estar en las carpetas del SDK de VS

##
## Opciones del compilador
##   /MT linkar versiones estáticas de la librerías run-time
##

set (flags_compilador "/std:c++17  /MT ${opcs_warnings} ${opcs_primer_error}")  
set (archivos_lib "${glew_lib} ${glfw_lib} ${opgl_lib} ${jpeg_lib}" )

##
## Flags para compilar y enlazar, archivos .lib para enlazar
##

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flags_compilador}")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:MSVCRT /NODEFAULTLIB:LIBCMT ${archivos_lib}")

##
## definir donde están las unidades de compilación y los headers

file (GLOB units   ${src}/*.cpp)
file (GLOB headers ${src}/*.h)


##
## definir el ejecutable y los directorios de include
##

add_executable ( ${ejecutable} ${units} ${headers} )
target_include_directories ( ${ejecutable} PUBLIC ${src} ${libs_inc})

##### imprime algunas y/o todas las variables cuando se pone 'cmake .' (activar/desactivar poniendo true/false)

message(STATUS "CUA: CMAKE_EXE_LINKER_FLAGS == ${CMAKE_EXE_LINKER_FLAGS}")
message(STATUS "CUA: CMAKE_CXX_FLAGS        == ${CMAKE_CXX_FLAGS}")


if ( false )
   get_cmake_property(_variableNames VARIABLES)
   list (SORT _variableNames)
   foreach (_variableName ${_variableNames})
      message(STATUS "${_variableName}=${${_variableName}}")
   endforeach()
endif()

