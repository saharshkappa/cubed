A simple project built with [raylib](https://www.raylib.com/). This program renders a 3D cube using basic points and lines (wireframe style) and rotates it continuously in space. 

## Features
* **3D Rendering:** Draws a 3D cube from scratch.
* **Wireframe Style:** Uses basic points and lines instead of solid polygons.

## Prerequisites
Before running this project, ensure you have the following set up:
* A working C/C++ Compiler 
* [raylib](https://github.com/raysan5/raylib) installed and configured for your environment.

## Getting Started

### 1. Clone the repository
```bash
git clone [https://github.com/saharshkappa/cubed]
cd [cubed]
```
### 2. To Compile 
Either run the build.bash file
or Run
```
gcc cube.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```

### 3. To Run 
Run
```
./a.out
```