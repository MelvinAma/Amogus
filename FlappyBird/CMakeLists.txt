cmake_minimum_required(VERSION 3.23)
project(Amogus)

set(CMAKE_CXX_STANDARD 14)

include_directories(FlappyBird)

add_executable(Amogus
        FlappyBird/cmake-build-debug/Testing/Temporary/LastTest.log
        FlappyBird/cmake-build-debug/build.ninja
        FlappyBird/cmake-build-debug/cmake_install.cmake
        FlappyBird/cmake-build-debug/CMakeCache.txt
        FlappyBird/CMakeLists.txt
        FlappyBird/COPYING
        FlappyBird/GameIcons.c
        FlappyBird/labwork.S
        FlappyBird/labwork.S.o
        FlappyBird/Makefile
        FlappyBird/mipslab.h
        FlappyBird/mipslabdata.c.o
        FlappyBird/mipslabfunc.c
        FlappyBird/mipslabfunc.c.o
        FlappyBird/mipslabmain.c
        FlappyBird/mipslabmain.c.o
        FlappyBird/mipslabwork.c
        FlappyBird/mipslabwork.c.o
        FlappyBird/outfile.elf
        FlappyBird/outfile.hex
        FlappyBird/pic32mx.h
        FlappyBird/stubs.c
        FlappyBird/stubs.c.o
        FlappyBird/time4io.c
        FlappyBird/time4io.c.o
        FlappyBird/vectors.S
        FlappyBird/vectors.S.o
        README.md)
