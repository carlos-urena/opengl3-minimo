## https://docs.microsoft.com/en-us/cpp/build/walkthrough-compiling-a-native-cpp-program-on-the-command-line?view=msvc-170

build: directory bin-make\main_windows.exe

bin-make\main_windows.exe: ..\..\src\main.cpp
	cl  ..\..\src\main.cpp /link /out:bin-make\main_windows.exe 

clean:
	del bin-make\main_windows.exe

rebuild: clean build

directory: bin-make

bin:
	md bin-make