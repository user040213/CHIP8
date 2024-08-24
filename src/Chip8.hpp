#ifndef CHIP8_H
#define CHIP8_H

#include <cstdint>
#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <filesystem>
#include <cstring>

#define DISPLAY_COLUMNS 64
#define DISPLAY_ROWS 32
#define DISPLAY_SIZE 64*32 
#define RAM_START 0x200
#define RAM_SIZE 4096
#define CLOCKHZ 720 
#define DRAWHZ 60
#define DELAYHZ 60


/*
Chip8 class to "emulate" Chip-8 functionalities
Below are the Chip-8's specs
    4096 byte RAM
    64 x 32 display area
    16 bit index register (points to memory locations)
    16 bit program counter (points to current instruction)
    16 entry 12 bit stack
    8 bit delay and sound timer
    16 8 bit registers
    4x4 keypad

*/
class Chip8
{
    public:
        uint8_t ram[RAM_SIZE];
        uint8_t display[DISPLAY_SIZE];
        uint16_t indexReg;
        uint16_t pc;
        uint16_t stack[16];
        uint8_t sp;
        uint8_t delayTimer;
        uint8_t soundTimer;
        uint8_t registers[16];
        uint8_t keypad[16];
        uint16_t opcode;

    private:

        
        uint8_t Vx;
        uint8_t Vy;
        uint8_t N;
        uint8_t NN;
        uint16_t NNN;

        uint8_t keyHold; // used to halt until key is released

        // Random number generation
        std::mt19937 randomGen;
        std::uniform_int_distribution<> randomNum;

    public:
        Chip8();
        void loadROM(const std::string fileName);
        void tickTimers();

        void run();

        // Instructions
        // op0NNN (unimplemented; unnecessary)
        void op00E0();
        void op1NNN();
        void op00EE();
        void op2NNN();
        void op3XNN();
        void op4XNN();
        void op5XY0();
        void op9XY0();
        void op6XNN();
        void op7XNN();

        // 8xxN instructions
        void op8XY0();
        void op8XY1();
        void op8XY2();
        void op8XY3();
        void op8XY4();
        void op8XY5();
        void op8XY6();
        void op8XY7();
        void op8XYE();
        
        void opANNN();

        void opBNNN();

        void opCXNN();

        void opDXYN();

        void opEX9E();
        void opEXA1();

        void opFX07();
        void opFX0A();
        void opFX15();
        void opFX18();
        void opFX1E();
        void opFX29();
        void opFX33();
        void opFX55();
        void opFX65();





};

#endif