# Un ejemplo OpenGL mínimo (usando OpenGL 3.3)

En esta carpeta se encuentra el código fuente mínimo de una programa OpenGL en C++11, suficiente para dibujar un triángulo. Se usa exclusivamente funcionalidad de OpenGL 3.3, y por tanto se requiere esa versión como mínimo. Los fuentes se pueden compilar y ejecutar en Windows, Linux o macOS, para ello se deben de instalar los requisitos software que se indican aquí abajo, y después compilar siguiendo estas instrucciones.

## Compilar, ejecutar y depurar

Este programa se puede compilar y ejecutar en la línea de órdenes o bien usando VS Code.

### Compilar y ejecutar en la línea de órdenes, en macOS, Linux o Windows

En Windows hay que usar el terminal llamado __Developer Command Prompt__, es la aplicación de terminal de Windows, pero configurada con las variables de entorno necesarias para compilar desde la línea de órdenes. En Linux y macOS se puede usar un terminal normal.

Estos fuentes se deben compilar con `cmake`, para ello es necesario ir a la carpeta `builds/macos`, `builds/linux`, o `builds\windows`, según el sistema operativo. 
En esa carpeta debemos asegurarnos de que la carpeta `bin-cmake` está vacía (si no lo estaba ya, hay que borrar todos los archivos ahí, excepto `.gitignore`), entrar en ella y hacer 

```
cmake ..
``` 

Esto hay que hacerlo una vez, o cada vez que se añadan nuevos fuentes o se quiera cambiar la configuración de compilación. Esto genera diversos archivos y carpetas en `bin-cmake`. Después, para compilar los fuentes, basta con hacer `cd` a la carpeta `bin-cmake` y una vez ahí ejecutar 

```
cmake --build .
```

o bien, en macOS y Linux, podemos hacer simplemente 

``` 
make 
``` 

Si la compilación va bien se genera el ejecutable, que tiene el nombre  `main_linux_exe` en Linux,  `main_macos_exe` en macOS o bien `main_windows.exe` en Windows (este último en la subcarpeta `Debug` dentro de `bin-cmake`).
Para forzar un recompilado de todos los fuentes, basta con vaciar la carpeta `bin-cmake` y volver a hacer `cmake ..` en ella.


### Uso de VS Code en Linux, macOS y Windows.

Las carpetas `build/linux`, `build/macos` y `build/windows` incluyen archivos de extensión `.code-workspace`. Estos archivos se pueden abrir con VS Code de Microsoft, para poder editar, compilar, ejecutar y depurar fácilmente el código.

## Requisitos en Linux

En linux es necesario tener instalado el compilador de C++ de GNU o del proyecto LLVM, esto permite invocar la orden `g++` y la orden `make`. Si no se tiene disponible, el compilador de GNU puede instalarse con

```
sudo apt install build-essential
```

Alternativamente, el compilador del proyecto LLVM (junto con la orden _make_) se puede instalar con:

```
sudo apt install clang
sudo apt install make
```


Además se deben instalar los paquetes _libglew-dev_ y _glfw3-dev_, se puede hacer con:

```
sudo apt install libglew-dev
sudo apt install libglfw3-dev
```

Igualmente, se puede usar _apt_ para instalar _cmake_, que se usa para poder compilar fácilmente el ejemplo, se hace con:

```
sudo apt install cmake
```


## Requisitos en macOS

En ordenadores macOS hay que tener instalada la herramienta de desarrollo de **XCode** ([developer.apple.com/xcode](https://developer.apple.com/xcode/)).
Este herramienta de desarrollo incorpora (entre otros) el compilador de C++ del proyecto LLVM adaptado por Apple, el IDE de desarrollo para Apple, así como el _framework_ de **OpenGL**. 

Una vez instalado _XCode_ (y si no se ha hecho durante la instalación) es necesario instalar un componente adicional de XCode llamado _Command line Tools_ (CLT), se puede hacer desde la propia línea de órdenes con:

```
xcode-select --install
```

Además de _XCode_, también podemos usar el instalador de paquetes open source **Homebrew** ([brew.sh](https://brew.sh/index_es)), para instalarlo se deben seguir las instrucciones que podemos encontrar en la página Web, aunque en realidad es fácil hacerlo desde la línea de órdenes con:

```
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

La librería OpenGL ya viene instalada con _XCode_, así que únicamente hará falta instalar la orden **cmake** y la librería **GLFW**. Ambos paquetes se pueden instalar fácilmente con _homebrew_, usando:  

```
brew install cmake
brew install glfw
```

## Requisitos en Windows

En Windows hay que instalar **Visual Studio** ([visualstudio.microsoft.com](https://visualstudio.microsoft.com))
 de Microsoft, es un entorno de desarrollo y una suite compiladores e intérpretes para varios lenguajes de programación. 

Este entorno de desarrollo incluye numerosos componentes para sus distintos lenguajes de programación. Para este repositorio únicamente hay que instalar los componentes para desarrollo de aplicaciones de escritorio con C y C++, aquí hay instrucciones específicas para esto:

[learn.microsoft.com/es-es/cpp/build/vscpp-step-0-installation](https://learn.microsoft.com/es-es/cpp/build/vscpp-step-0-installation)

Este software incluye tanto _cmake_ como _git_.

Los archivos de compilación están preparados para compilar en la línea de órdenes usando librerías instaladas con **vcpkg**, que es un instalador (open source) de paquetes con librerías de C/C++ de Microsoft ([vcpkg.io](https://vcpkg.io)).  Para instalar _vcpkg_ debes hacer `cd` a tu carpeta _home_ (es decir, la carpeta `C:\Users\usuario\`) y una vez en ella clonar el repositorio de _vcpkg_ con:

```
git clone https://github.com/Microsoft/vcpkg.git
```

Si todo va bien se crea una carpeta de nombre `vcpkg` dentro de tu carpeta _home_. Es posible situar esta carpeta en cualquier otro lugar del sistema de archivos, pero los scripts de compilación y otros archivos de configuración esperan que esté en tu _home_. Por tanto, si la situas en otro sitio, tendrás que adapatar dichos scripts de compilación. Para finalizar la instalación debes hacer `cd` a tu carpeta _home_, y ahí ejecutar:

```
.\vcpkg\bootstrap-vcpkg.bat
```

En la carpeta `vcpkg` quedará el archivo ejecutable `vcpkg.exe`, que se puede ejecutar directamente desde la línea de órdenes. En windows debemos de instalar las librerías **GLEW** y **GLFW**, se puede hacer `cd` a la carpeta `vcpkg` y ejecutar 

```
.\vcpkg install glew  --triplet x64-windows
.\vcpkg install glfw3 --triplet x64-windows
```

El _switch_ `--triplet` indica que se instalen las versiones de 64 bits _dinámicas_ de estas librerías. La instalación de GLEW conlleva la instalación de la librería OpenGL.








