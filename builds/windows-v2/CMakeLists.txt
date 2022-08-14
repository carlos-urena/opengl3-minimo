## ----------------------------------------------------------------------------
## Prácticas de Informática Gráfica
##
## Archivo de configuración para compilar usando CMake en Windows
## última modificación: Agosto 2022
## ----------------------------------------------------------------------------


set ( CMAKE_GENERATOR_PLATFORM "x64")
cmake_minimum_required (VERSION 3.10)
project (pracs_ig_exe)
set     (ejecutable pracs_ig_exe)


## Nota: warning eliminados:
##   warning 4305: conversion de 'double' a 'float'
##   warning 4244: conversion de 'double' a 'const float'
##   warning 4267: conversión de 'size_t' a 'unsigned int'

set (flags_compilador "/wd4305 /wd4244 /wd4267")

########################################################################
## Archivo de configuración para compilar en windows

## componer los paths a los distintas carpetas de trabajo

set (raiz "../..")
set (src  ${raiz}/src)

## definir flags para compilador y enlazador

set (packs_base "${raiz}/../librerias/packages-win" )

set (jpeg_base "${packs_base}/libjpeg.9.2.0.1/build/native")
set (jpeg_inc "${jpeg_base}/include")
set (jpeg_lib "${jpeg_base}/lib/v140/x64/Debug/dynamic/cdecl")

set ( nupengl_base "${packs_base}/nupengl.core.0.1.0.1/build/native/")
set ( nupengl_inc "${nupengl_base}/include")
set ( nupengl_lib "${nupengl_base}/lib/x64/" )

set ( nupengl_base_redist "${packs_base}/nupengl.core.redist.0.1.0.1/build/native")
file ( GLOB nupengl_dlls "${nupengl_base_redist}/bin/x64/*.dll" )

set (jpeg_base_redist "${packs_base}/libjpeg.redist.9.2.0.1/build/native")
file ( GLOB jpeg_dlls "${jpeg_base_redist}/bin/v140/x64/Debug/dynamic/cdecl/*.dll")

set (flags_enlazador "")

## especificar a 'cmake' los directorios para ejecutable y los flags

##set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${raiz}/${diralu}/bin")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++14 ${flags_compilador}")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${flags_enlazador}")

## definir donde están las unidades de compilación

file (GLOB units ${src}/*.cpp)
file (GLOB headers ${src}/*.h)

message(STATUS "units=${units}")

## definir el ejecutable y los directorios de include y librerías

add_executable ( ${ejecutable} ${units} ${headers}  )
target_include_directories ( ${ejecutable} PUBLIC ${src} ${jpeg_inc} ${nupengl_inc} )
target_link_directories ( ${ejecutable} PUBLIC ${jpeg_lib} ${nupengl_lib} ${jpeg_dlls} ${nupengl_dlls} )
target_link_libraries( ${ejecutable} glfw3dll glew32  glu32 opengl32 jpeg )

### copiar las DLLs necesarias en la carpteta de salida:
add_custom_command ( TARGET ${ejecutable} POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy ${nupengl_dlls} Debug/
                     COMMAND ${CMAKE_COMMAND} -E copy ${jpeg_dlls} Debug/ )

##### imprime todas las variables cuando se pone 'cmake .'

if ( false )
   get_cmake_property(_variableNames VARIABLES)
   list (SORT _variableNames)
   foreach (_variableName ${_variableNames})
      message(STATUS "${_variableName}=${${_variableName}}")
   endforeach()
endif()
