cmake_minimum_required(VERSION 3.24)
project(Amogus)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories("Amogus-main 2")
include_directories("Amogus-main 2/FlappyBird")
include_directories("Amogus-main 2/FlappyBird/IOShieldOled")
include_directories("Amogus-main 2/FlappyBird/IOShieldOled/utility")
include_directories(FlappyBird)
include_directories(FlappyBird/IOShieldOled)
include_directories(FlappyBird/IOShieldOled/utility)

add_executable(Amogus
        "Amogus-main 2/FlappyBird/cmake-build-debug/Testing/Temporary/LastTest.log"
        "Amogus-main 2/FlappyBird/cmake-build-debug/build.ninja"
        "Amogus-main 2/FlappyBird/cmake-build-debug/cmake_install.cmake"
        "Amogus-main 2/FlappyBird/cmake-build-debug/CMakeCache.txt"
        "Amogus-main 2/FlappyBird/IOShieldOled/examples/IOShield_Oled_Demo/IOShield_Oled_Demo.pde"
        "Amogus-main 2/FlappyBird/IOShieldOled/utility/ChrFont0.c"
        "Amogus-main 2/FlappyBird/IOShieldOled/utility/delay.c"
        "Amogus-main 2/FlappyBird/IOShieldOled/utility/delay.h"
        "Amogus-main 2/FlappyBird/IOShieldOled/utility/FillPat.c"
        "Amogus-main 2/FlappyBird/IOShieldOled/utility/OledChar.c"
        "Amogus-main 2/FlappyBird/IOShieldOled/utility/OledChar.h"
        "Amogus-main 2/FlappyBird/IOShieldOled/utility/OledDriver.c"
        "Amogus-main 2/FlappyBird/IOShieldOled/utility/OledDriver.h"
        "Amogus-main 2/FlappyBird/IOShieldOled/utility/OledGrph.c"
        "Amogus-main 2/FlappyBird/IOShieldOled/utility/OledGrph.h"
        "Amogus-main 2/FlappyBird/IOShieldOled/IOShield Library - Oeld.pdf"
        "Amogus-main 2/FlappyBird/IOShieldOled/IOShieldOled.cpp"
        "Amogus-main 2/FlappyBird/IOShieldOled/IOShieldOled.h"
        "Amogus-main 2/FlappyBird/IOShieldOled/keywords.txt"
        "Amogus-main 2/FlappyBird/TestMain.dSYM/Contents/Resources/DWARF/TestMain"
        "Amogus-main 2/FlappyBird/TestMain.dSYM/Contents/Info.plist"
        "Amogus-main 2/FlappyBird/ButtonHandler.c"
        "Amogus-main 2/FlappyBird/ButtonHandler.c.o"
        "Amogus-main 2/FlappyBird/COPYING"
        "Amogus-main 2/FlappyBird/Declarations.h"
        "Amogus-main 2/FlappyBird/GameData.c"
        "Amogus-main 2/FlappyBird/GameData.c.o"
        "Amogus-main 2/FlappyBird/labwork.S"
        "Amogus-main 2/FlappyBird/labwork.S.o"
        "Amogus-main 2/FlappyBird/Main.c"
        "Amogus-main 2/FlappyBird/Main.c.o"
        "Amogus-main 2/FlappyBird/Makefile"
        "Amogus-main 2/FlappyBird/MipsFunc.c"
        "Amogus-main 2/FlappyBird/MipsFunc.c.o"
        "Amogus-main 2/FlappyBird/MipsGamework.c"
        "Amogus-main 2/FlappyBird/MipsGamework.c.o"
        "Amogus-main 2/FlappyBird/outfile.elf"
        "Amogus-main 2/FlappyBird/outfile.hex"
        "Amogus-main 2/FlappyBird/pic32mx.h"
        "Amogus-main 2/FlappyBird/README.md"
        "Amogus-main 2/FlappyBird/stubs.c"
        "Amogus-main 2/FlappyBird/stubs.c.o"
        "Amogus-main 2/FlappyBird/TestMain"
        "Amogus-main 2/FlappyBird/TestMain.c"
        "Amogus-main 2/FlappyBird/TestMain.c.o"
        "Amogus-main 2/FlappyBird/todo.c"
        "Amogus-main 2/FlappyBird/todo.c.o"
        "Amogus-main 2/FlappyBird/vectors.S"
        "Amogus-main 2/FlappyBird/vectors.S.o"
        "Amogus-main 2/README.md"
        FlappyBird/cmake-build-debug/Testing/Temporary/LastTest.log
        FlappyBird/cmake-build-debug/build.ninja
        FlappyBird/cmake-build-debug/cmake_install.cmake
        FlappyBird/cmake-build-debug/CMakeCache.txt
        FlappyBird/IOShieldOled/examples/IOShield_Oled_Demo/IOShield_Oled_Demo.pde
        FlappyBird/IOShieldOled/utility/ChrFont0.c
        FlappyBird/IOShieldOled/utility/delay.c
        FlappyBird/IOShieldOled/utility/delay.h
        FlappyBird/IOShieldOled/utility/FillPat.c
        FlappyBird/IOShieldOled/utility/OledChar.c
        FlappyBird/IOShieldOled/utility/OledChar.h
        FlappyBird/IOShieldOled/utility/OledDriver.c
        FlappyBird/IOShieldOled/utility/OledDriver.h
        FlappyBird/IOShieldOled/utility/OledGrph.c
        FlappyBird/IOShieldOled/utility/OledGrph.h
        "FlappyBird/IOShieldOled/IOShield Library - Oeld.pdf"
        FlappyBird/IOShieldOled/IOShieldOled.cpp
        FlappyBird/IOShieldOled/IOShieldOled.h
        FlappyBird/IOShieldOled/keywords.txt
        FlappyBird/TestMain.dSYM/Contents/Resources/DWARF/TestMain
        FlappyBird/TestMain.dSYM/Contents/Info.plist
        FlappyBird/ButtonHandler.c
        FlappyBird/ButtonHandler.c.o
        FlappyBird/COPYING
        FlappyBird/Declarations.h
        FlappyBird/GameData.c
        FlappyBird/GameData.c.o
        FlappyBird/labwork.S
        FlappyBird/labwork.S.o
        FlappyBird/Main.c
        FlappyBird/Main.c.o
        FlappyBird/Makefile
        FlappyBird/MipsFunc.c
        FlappyBird/MipsFunc.c.o
        FlappyBird/MipsGamework.c
        FlappyBird/MipsGamework.c.o
        FlappyBird/outfile.elf
        FlappyBird/outfile.hex
        FlappyBird/pic32mx.h
        FlappyBird/README.md
        FlappyBird/stubs.c
        FlappyBird/stubs.c.o
        FlappyBird/TestMain
        FlappyBird/TestMain.c
        FlappyBird/TestMain.c.o
        FlappyBird/todo.c
        FlappyBird/todo.c.o
        FlappyBird/vectors.S
        FlappyBird/vectors.S.o
        README.md)
