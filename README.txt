
Project Based on Scriptbots by Andrej Karpathy

SCRIPTBOTS
Author: Andrej Karpathy
Contributors: Casey Link
Licence: BSD

Project website and attached forum: 
https://sites.google.com/site/scriptbotsevo/home

-----------------------------------------------------


BUILDING:
To compile you will need:

CMake >= 2.8 (http://www.cmake.org/cmake/resources/software.html)

VTK >= 7.1.1. you need to compile VTK and change the VTK_DIR attributes to the path to VTK on your system

OpenGL and GLUT (http://www.opengl.org/resources/libraries/glut/)
* Linux: freeglut (http://freeglut.sourceforge.net/)
It will use OpenMP to speed up everything, in case you have multicore cpu.

If you are on Ubuntu (or debian) you can install all the dependencies (except vtk) with:
$ apt-get install cmake build-essential libopenmpi-dev libglut32-dev libxi-dev libxmu-dev

the original scriptbots project is runable on windows but I haven't tested it