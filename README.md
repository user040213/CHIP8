# Chip-8 "Emulator"

<p>This project allows the execution of ROMs designed to run on CHIP-8 machines.</p>

<p>This is technically an interpreter, since CHIP-8 is a programming language, but since this program emulates CPU components as well, it might be called an emulator</p>

## About the Project

<p>This is a mostly fully-functioning CHIP-8 emulator, however it lacks sound currently</p>

<p>This was written in pure C++ and makes use of classes to define a System class to loop through the CPU cycles and poll for input from the SDL context</p>

## Using This Repo

<p>You can use the MakeFile (mingw32-make -f MakeFile) </p>

<p>Or compile manually (g++ -std=c++17 -Iinclude -Iinclude/SDL2 -Llib -o Chip8 src/*.cpp -lmingw32 -lSDL2main -lSDL2) </p>

<p>Some ROMs can be found in the ROMs folder, simply drag and drop into the main directory and then run the program with the ROMs filename as the only argument (ex. Chip8 "Pong [Paul Vervalin, 1990].ch8") </p>

## Some Screenshots
![IBM Splash Screen](images/IBMSplash.png)
#### Test Suite
![Opcode Tests](images/OpcodeTest.png)
#### Pong
![Pong](images/PongImage.png)
#### Space Invaders
![Space Invaders Splash](images/SpaceInvaders.png)

## Resources
<p> Below are my sources for the ROMs and the CHIP-8's specs </p>

<p> [Test Suite](https://github.com/Timendus/chip8-test-suite?tab=readme-ov-file#keypad-test)</p>

<p> [Hardware Specifications](https://github.com/mattmikolay/chip-8/wiki/CHIP%E2%80%908-Instruction-Set#notes)</p>

<p> [Pong, Tetris and Space Invaders](https://github.com/kripod/chip8-roms)</p>
