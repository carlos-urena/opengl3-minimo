## https://docs.microsoft.com/en-us/cpp/build/walkthrough-compiling-a-native-cpp-program-on-the-command-line?view=msvc-170

build: directory bin-make\main_windows.exe

exe=bin-make\main_windows.exe
src=..\..\src
glfw_root=\Users\carlo\Downloads\glfw-3.3.8.WIN64
glfw_incl=$(glfw_root)\include
incl=/I$(glfw_incl) /I$(src) 

$(exe): $(src)/main.cpp
	CL  $(incl)  $(src)\main.cpp /link /out:$(exe) 

clean: 
	del $(exe)

rebuild: clean build

directory: bin-make

bin:
	md bin-make