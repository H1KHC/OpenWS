# OpenWS - A Window System Frame In OpenGL Programs

## Introduction

The Window System Frame Library (OpenWS) is a cross-platform, open-source C/C++ extension library. It provides an easy-use frame which supports mutiple-virtual-window processing in OpenGL programs.

## Dependences

OpenWS is based on glfw, glew and the headers and libraries of your system.

As it uses framebuffer to maintain windows, this library may doesn't support the old machines.

And you may also need OpenMP library(gcc option `-fopenmp`) for the best performance.

## Compiling OpenWS

OpenWS uses makefile to compile.

To compile this library, you may simply cd into this directory, and run `make` to build.

Some options are available:
- `DEBUG=true` : debug version
- `STATIC=true`: static library
- `BIT=64`     : 64-bit version (unimplemented)
- `sample`       will build samples

## Using OpenWS

Maybe you can have a look at the samples?
