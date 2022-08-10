## https://docs.microsoft.com/en-us/cpp/build/walkthrough-compiling-a-native-cpp-program-on-the-command-line?view=msvc-170


exe=bin-make/main_windows.exe
src=../../src
librerias=../../../librerias
glfw_root=$(librerias)/glfw-3.3.8.bin.WIN32
glew_root=$(librerias)/glew-2.1.0

glfw_lib=$(glfw_root)/lib-vc2019
glew_lib=$(glew_root)/lib/Release/Win32

incl=-I$(src) -I$(glfw_root)/include -I$(glew_root)/include 
libs=$(glfw_lib)/glfw3.lib $(glew_lib)/glew32.lib

## mt == librería estática (.lib) ??
$(exe): $(src)/main.cpp 
	CL -std:c++14 -EHsc  -MT $(incl)   $(src)/main.cpp   -link  -out:$(exe) $(libs) 

clean: 
	del $(exe)

