cmake_minimum_required(VERSION 4.1)
project(GB_Emulator)

set(CMAKE_CXX_STANDARD 20)

#include_directories(Graphics_Engine/include)
#include_directories(Graphics_Engine/sample_projects/game_of_life)
#include_directories(Graphics_Engine/sample_projects/snake)
#include_directories(Graphics_Engine/src/internal)

add_executable(GB_Emulator
#        Graphics_Engine/include/color.h
#        Graphics_Engine/include/engine.h
#        Graphics_Engine/include/keyboard_mouse_inputs.h
#        Graphics_Engine/sample_projects/falling_sand/fallingsand.cpp
#        Graphics_Engine/sample_projects/game_of_life/gameoflife.cpp
#        Graphics_Engine/sample_projects/game_of_life/GOL_helpers.cpp
#        Graphics_Engine/sample_projects/game_of_life/GOL_helpers.h
#        Graphics_Engine/sample_projects/snake/snake.cpp
#        Graphics_Engine/sample_projects/snake/snake_helpers.cpp
#        Graphics_Engine/sample_projects/snake/snake_helpers.h
#        Graphics_Engine/src/internal/draw.h
#        Graphics_Engine/src/internal/input.h
#        Graphics_Engine/src/internal/renderer.h
#        Graphics_Engine/src/internal/textures.h
#        Graphics_Engine/src/internal/window.h
#        Graphics_Engine/src/draw.cpp
#        Graphics_Engine/src/engine.cpp
#        Graphics_Engine/src/input.cpp
#        Graphics_Engine/src/renderer.cpp
#        Graphics_Engine/src/textures.cpp
#        Graphics_Engine/src/window.cpp
        Emulator/main.cpp
        Emulator/GameBoy.cpp
        Emulator/GameBoy.h
        Emulator/cpu/CPU.cpp
        Emulator/cpu/CPU.h
        Emulator/debug/Debugger.cpp
        Emulator/debug/Debugger.h
        Emulator/joypad/Joypad.cpp
        Emulator/joypad/Joypad.h
        Emulator/memory/Bus.cpp
        Emulator/memory/Bus.h
        Emulator/ppu/PPU.cpp
        Emulator/ppu/PPU.h
        Emulator/timer/Timer.cpp
        Emulator/timer/Timer.h
        Emulator/cartridge/Cartridge.cpp
        Emulator/cartridge/Cartridge.h)
