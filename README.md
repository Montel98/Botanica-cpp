# Botanica-cpp

https://user-images.githubusercontent.com/40472971/162130434-786dcc68-be9e-452b-b784-c4d67fa315b6.mp4

## About
Bonsai trees procedurally generated using probabilitic L-Systems. They grow, bloom and drop leaves depending on the time of year. In the Javascript variant (and soon C++), the tree geometry is mouldable - branches can be pruned and new shoots will grow. This is the desktop C++ variant with improved performance.

The source code consists of the following:
* Renderer *(OpenGL)*
* Tree simulation *(STL C++ / GLSL)*

Demo site (WebGL variant): https://www.botanicademo.com/

## Building from source (Linux)

* ```git clone https://github.com/Montel98/Botanica-cpp``` in a new directory
* Download GLAD, selecting OpenGL version 4.5 under *API*: https://glad.dav1d.de/ and copy files into ```/src``` directory
* Install remaining dependencies with ```sudo agt-get install libglwf3-dev libglew-dev freeglut3-dev```
* Compile using ```make``` and run with ```./bin/Botanica```
