## https://docs.microsoft.com/en-us/cpp/build/walkthrough-compiling-a-native-cpp-program-on-the-command-line?view=msvc-170

build: directory bin-make\main_windows.exe

exe=bin-make\main_windows.exe
src=..\..\src
librerias=..\..\..\librerias
glfw_root=$(librerias)\glfw-3.3.8.bin.WIN64
glew_root=$(librerias)\glew-2.1.0

glfw_lib=$(glfw_root)\lib-vc2019
glew_lib=$(glew_root)\lib\Release\x64

incl=/I$(src) /I$(glfw_root)\include /I$(glew_root)\include 
libs=$(glfw_lib)\glfw3.lib $(glew_lib)\glew32.lib

## mt == librería estática (.lib) ??
$(exe): $(src)/main.cpp
	CL  /std:c++14 /EHsc /MT   $(incl)   $(src)\main.cpp $(libs)  /link  /out:$(exe) 

clean: 
	del $(exe)

rebuild: clean build

directory: bin-make

bin:
	md bin-make