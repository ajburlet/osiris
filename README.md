# Osiris Framework

## What is this?

This is a simulation framework that uses OpenGL to render 3D graphics. 

## How to use

### Dependencies

- Freeglut
- Freetype
- GLM
- GLSDK

All can be found on the  ```dependencies\``` directory, either linked or hard copied into this repository.

### Build

This project uses CMake. For now, the code is fitted to run in Windows, so building in MSVC should work fine. For unix-like 
systems, there are a couple of issues pending. Once that has been taken care of, no reason why it shouldn't work on Linux or
Mac OS.

Here are the steps (assume we are starting from the ```Osiris/``` directory:

1) Create a build directory:

```
# mkdir build
```

2) Change into this directory:

```
# cd build
```

3) Call ```cmake```:

```
# cmake ..
```

This will generate the needed build files (VS project, makefile, etc.) in the format you chose (see cmake ```-G``` 
option documentation for further details).

## Documentation

Refer to: http://ajburlet.github.io/Osiris
