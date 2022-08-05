# Un ejemplo OpenGL mínimo (usando OpenGL 3.3)

En esta carpeta se encuentra el código fuente mínimo de una programa OpenGL en C++11, suficiente para dibujar un triángulo. Se usa exclusivamente funcionalidad de OpenGL 3.3, y por tanto se requiere esa versión como mínimo.

## Compilar, ejecutar y depurar

Este programa se puede compilar y ejecutar en la línea de órdenes o bien usando VS Code.

### Compilar y ejecutar en la línea de órdenes

Este programa se puede compilar con `cmake`, para ello es necesario ir a la carpeta `builds/macos` o `build/linux`, según el sistema operativo.

En esa carpeta debemos asegurarnos de que la carpeta `bin-cmake` está vacía (si no lo estaba ya, borrar todos los archivos ahí, excepto `.gitignore`), entrar en ella y hacer `cmake ..` (una vez, o cada vez que se añadan nuevos fuentes o se quiera cambiar la configuración de compilación). Esto genera diversos archivos en `bin-cmake`.

Para compilar los fuentes, basta con hacer `make` en la carpeta `bin-cmake`. Si va bien se genera el ejecutable `main_macos_exe`, que se ejecuta con `./main_macos_exe` (o bien `main_linux_exe`, en Linux).

Para forzar un recompilado de los fuentes, hacer `make clean && make` en `bin-cmake`.

### Uso de VS Code

Las carpetas `build/linux` y `build/macos` incluyen sendos archivos de extensión `.code-workspace`. Estos archivos se pueden abrir con VS Code de Microsoft, para poder editar, compilar, ejecutar y depurar fácilmente el código.

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

Igualmente, se puede usar _apt_ para instalar _cmake_, que se usa en la versión de OpenGL 3.3.

```
sudo apt install cmake
```

## Requisitos en macOS

En ordenadores macOS hay que tener instalada la herramienta de desarrollo de **XCode**.
Este herramienta de desarrollo incorpora el compilador de C++ del proyecto LLVM, así como el _framework_ de **OpenGL**.

### Descarga e instalación de GLFW con _homebrew_

Usando la herramienta **homebrew** para mac, se puede instalar fácilmente la librería **GLFW**. En caso de no tenerlo disponible, es necesario instalar _homebrew_ ([https://brew.sh/](brew.sh)). Una vez disponible, podemos instalar el paquete `glfw` simplemente con:

```
brew install glfw
```

### Instalación de la orden _cmake_

Para la versión con OpenGL 3.3 hará falta la orden _cmake_ para macOS. El programa puede descargarse directamente de la Web del desarrollador ([https://cmake.org/download/](https://cmake.org/download/)). También se puede instalar con  _homebrew_, con `brew install cmake`.

### Descarga e instalación de GLFW sin _homebrew_

Si no se quiere usar **homebrew**, se puede descargar y compilar GLFW directamente.

Una vez que se tiene disponible la orden `cmake`, se puede descargar el archivo _zip_ de GLFW para macOS del sitio web de GLFW ([http://www.glfw.org/download.html](http://www.glfw.org/download.html)), después se abre el archivo en una carpeta nueva vacía, y al abrirlo se crea un subcarpeta de nombre `glfw-...` (los caracteres concretos en el lugar de los puntos suspensivos dependen de la versión, estos fuentes requieren la versión 3). Finalmente se puede compilar la librería de esta forma:

```
cd glfw-....
cmake -DBUILD_SHARED_LIBS=ON .
make
sudo make install
```
