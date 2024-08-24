#include "Chip8.hpp"

Chip8::Chip8()
{
    // Random number generator
    std::srand(std::time(nullptr));
    randomGen.seed(std::chrono::system_clock::now().time_since_epoch().count());
    randomNum = std::uniform_int_distribution<>(0,255);
    // Initialize program counter to start value
    // 0x0 to 0x1FF is reserved
    pc = RAM_START;
    soundTimer = 0;
    delayTimer = 0;
    indexReg = 0;
    sp = 0;
    keyHold = 16;

    // Font typically stored between 0x050 to 0x09F
    // A character is represented using 4x5 binary
    uint8_t fonts[80] = 
    {
        0b11110000, 0b10010000, 0b10010000, 0b10010000, 0b11110000, //0
        0b00100000, 0b01100000, 0b00100000, 0b00100000, 0b01110000, //1
        0b11110000, 0b00010000, 0b11110000, 0b10000000, 0b11110000, //2
        0b11110000, 0b00010000, 0b11110000, 0b00010000, 0b11110000, //3
        0b10010000, 0b10010000, 0b11110000, 0b00010000, 0b00010000, //4
        0b11110000, 0b10000000, 0b11110000, 0b00010000, 0b11110000, //5
        0b11110000, 0b10000000, 0b11110000, 0b10010000, 0b11110000, //6
        0b11110000, 0b00010000, 0b00100000, 0b01000000, 0b01000000, //7
        0b11110000, 0b10010000, 0b11110000, 0b10010000, 0b11110000, //8
        0b11110000, 0b10010000, 0b11110000, 0b00010000, 0b11110000, //9
        0b11110000, 0b10010000, 0b11110000, 0b10010000, 0b10010000, //A
        0b11100000, 0b10010000, 0b11100000, 0b10010000, 0b11100000, //B
        0b11110000, 0b10000000, 0b10000000, 0b10000000, 0b11110000, //C
        0b11100000, 0b10010000, 0b10010000, 0b10010000, 0b11100000, //D
        0b11110000, 0b10000000, 0b11110000, 0b00010000, 0b11110000, //E
        0b11110000, 0b10000000, 0b11110000, 0b10000000, 0b10000000  //F
    };

    // store font into ram
    for (unsigned int i{0}; i < 80; i++)
    {
        ram[0x50 + i] = fonts[i];
    }

    // init arrays
    memset(display, 0, DISPLAY_SIZE);
    memset(keypad, 0, 16);
    memset(registers, 0, 16);
    memset(stack, 0, 16);


}

void Chip8::loadROM(const std::string fileName)
{
    std::ifstream inputFile(fileName, std::ios::binary); 
    if(inputFile.is_open())
    {
        uintmax_t fileSize{std::filesystem::file_size(fileName)};
        if(fileSize > (RAM_SIZE - RAM_START))
        {
            std::cout << "File will not fit";
            return;
        }
        char* fBuffer{new char[fileSize]};
        inputFile.read(fBuffer, fileSize);

        inputFile.close();
        for(uintmax_t i{0}; i < fileSize; i++)
        {
            ram[RAM_START + i] = fBuffer[i]; 
        }

        delete[] fBuffer;
    }
    else
    {
        std::cout << "Could not open file";
    }
    
}

void Chip8::tickTimers()
{
    if (soundTimer > 0)
    {
        soundTimer--;
    }
    if (delayTimer > 0)
    {
        delayTimer--;
    }
}

void Chip8::run()
{

    // Fetch
    if(keyHold == 16)
    {
        opcode = (ram[pc] << 8) | (ram[pc + 1]);
        pc += 2;

        // Decode
        Vx = (opcode & 0x0F00) >> 8;
        Vy = (opcode & 0x00F0) >> 4;
        N = (opcode & 0x000F);
        NN = (opcode & 0x00FF);
        NNN = (opcode & 0x0FFF);

        // Execute
        switch((opcode & 0xF000) >> 12)
        {
            case 0x0:
                switch(NNN)
                {
                    case 0x0E0:
                        op00E0();
                        break;
                    case 0x0EE:
                        op00EE();
                        break;
                    // unimplemented: op0NNN();
                }
                break;
            case 0x1: // only one instr.
                op1NNN();
                break;
            case 0x2: // only one instr.
                op2NNN();
                break;
            case 0x3: // only one instr.
                op3XNN();
                break;
            case 0x4: // only one instr.
                op4XNN();
                break;
            case 0x5: // only one instr.
                op5XY0();
                break;
            case 0x6: // only one instr.
                op6XNN();
                break;
            case 0x7: // only one instr.
                op7XNN();
                break;
            case 0x8:
                switch(N)
                {
                    case 0x0:
                        op8XY0();
                        break;
                    case 0x1:
                        op8XY1();
                        break;
                    case 0x2:
                        op8XY2();
                        break;
                    case 0x3:
                        op8XY3();
                        break;
                    case 0x4:
                        op8XY4();
                        break;
                    case 0x5:
                        op8XY5();
                        break;
                    case 0x6:
                        op8XY6();
                        break;
                    case 0x7:
                        op8XY7();
                        break;
                    case 0xE:
                        op8XYE();
                        break;
                }
                break;
            case 0x9: // only one instr.
                op9XY0();
                break;
            case 0xA: // only one instr.
                opANNN();
                break;
            case 0xB: // only one instr.
                opBNNN();
                break;
            case 0xC: // only one instr.
                opCXNN();
                break;
            case 0xD: // only one instr.
                opDXYN();
                break;
            case 0xE:
                switch(NN)
                {
                    case 0xA1:
                        opEXA1();
                        break;
                    case 0x9E:
                        opEX9E();

                }
                break;
            case 0xF:
                switch(NN)
                {
                    case 0x07:
                        opFX07();
                        break;
                    case 0x0A:
                        opFX0A();
                        break;
                    case 0x15:
                        opFX15();
                        break;
                    case 0x18:
                        opFX18();
                        break;
                    case 0x1E:
                        opFX1E();
                        break;
                    case 0x29:
                        opFX29();
                        break;
                    case 0x33:
                        opFX33();
                        break;
                    case 0x55:
                        opFX55();
                        break;
                    case 0x65:
                        opFX65();
                        break;
                }
                break;

        }
    }
    else // check to remove keyHold so we can continue
    {
        if(!keypad[keyHold])
        {
            keyHold = 16;
        }
    }
}

void Chip8::op00E0()
{
    memset(display, 0, DISPLAY_SIZE);
}

void Chip8::op1NNN()
{
    // sets pc to 0x0NNN
    pc = NNN;
}

void Chip8::op00EE()
{
    sp--;
    pc = stack[sp];
}

void Chip8::op2NNN()
{
    stack[sp] = pc;
    sp++;
    pc = NNN;
}

void Chip8::op3XNN()
{
    if (registers[Vx] == NN)
    {
        pc += 2;
    }
}

void Chip8::op4XNN()
{
    if (registers[Vx] != NN)
    {
        pc += 2;
    }
}

void Chip8::op5XY0()
{
    if (registers[Vx] == registers[Vy])
    {
        pc += 2;
    }
}

void Chip8::op9XY0()
{
    if (registers[Vx] != registers[Vy])
    {
        pc += 2;
    }
}

void Chip8::op6XNN()
{
    registers[Vx] = NN;
}

void Chip8::op7XNN()
{
    registers[Vx] += NN;
}

void Chip8::op8XY0()
{
    registers[Vx] = registers[Vy];
}

void Chip8::op8XY1()
{
    registers[Vx] |= registers[Vy];
    registers[0xF] = 0;
}

void Chip8::op8XY2()
{
    registers[Vx] &= registers[Vy];
    registers[0xF] = 0;
}

void Chip8::op8XY3()
{
    registers[Vx] ^= registers[Vy];
    registers[0xF] = 0;
}

void Chip8::op8XY4()
{
    int16_t sum = registers[Vx] + registers[Vy];
    
    // truncate upper 8 bits
    registers[Vx] = sum & 0x00FF;

    if (sum > 0xFF)
    {
        registers[0xF] = 1;
    }
    else
    {
        registers[0xF] = 0;
    }
}

void Chip8::op8XY5()
{
    bool setFlag{registers[Vx] >= registers[Vy]};

    registers[Vx] -= registers[Vy];

    if (setFlag)
    {
        registers[0xF] = 1;
    }
    else
    {
        registers[0xF] = 0;
    }
}

void Chip8::op8XY6()
{
    // This instruction is sometimes different
    // carry flag is LSB of register Vy
    uint8_t shiftedBit = registers[Vy] & 0b1;
    
    registers[Vx] = registers[Vy] >> 1;
    registers[0xF] = shiftedBit;
    
}

void Chip8::op8XY7()
{
    bool setFlag{registers[Vy] >= registers[Vx]};

    registers[Vx] = registers[Vy] - registers[Vx];

    if (setFlag)
    {
        registers[0xF] = 1;
    }
    else
    {
        registers[0xF] = 0;
    }
}

void Chip8::op8XYE()
{
    uint8_t shiftedBit = (registers[Vy] & 0b10000000) >> 7;
    
    registers[Vx] = registers[Vy] << 1;
    registers[0xF] = shiftedBit;
    
}

void Chip8::opANNN()
{
    indexReg = NNN;
}

void Chip8::opBNNN()
{
    pc = registers[0x0] + NNN;
}

void Chip8::opCXNN()
{
    registers[Vx] = randomNum(randomGen) & NN;
}

void Chip8::opDXYN()
{
    // Get X and Y coordinates of sprite
    // Note: X = (0, 63), Y = (0, 31)
    // Ex. if Vx = 64, X = 0
    int8_t xCoord = registers[Vx] % DISPLAY_COLUMNS;
    int8_t yCoord = registers[Vy] % DISPLAY_ROWS;

    uint8_t rowData{0};
    uint16_t pixelIndex{0};
    uint8_t pixelBit{0};
    registers[0xF] = 0;
    
    for(uint8_t row{0}; row < N; row++)
    {
        // Read first from address pointed by index reg.
        rowData = ram[indexReg + row];
        
        // Getting pixel index
        pixelIndex = xCoord + (yCoord + row)*DISPLAY_COLUMNS;

        if(yCoord + row > 31)
        {
            break;
        }
        for(uint8_t col{0}; col < 8; col++)
        {
            if(xCoord + col > 63)
            {
                break;
            }
            pixelBit = (rowData >> (7 - col)) & 0b1;
            if (pixelBit) // flip pixel state
            {
                // pixel will be turned off
                if (display[pixelIndex + col] != 0)
                {
                    registers[0xF] = 1;
                }

                display[pixelIndex + col] ^= 0xFF;
            }
            
        }

    }
}

void Chip8::opEX9E()
{
    if(keypad[registers[Vx]])
    {
        pc += 2;
    }
}

void Chip8::opEXA1()
{
    if(!keypad[registers[Vx]])
    {
        pc += 2;
    }
}

void Chip8::opFX07()
{
    registers[Vx] = delayTimer;
}

void Chip8::opFX0A()
{
    if(keypad[0x0])
    {
        registers[Vx] = 0x0;
        keyHold = 0;
    }
    else if(keypad[0x1])
    {
        registers[Vx] = 0x1;
        keyHold = 1;
    }
    else if(keypad[0x2])
    {
        registers[Vx] = 0x2;
        keyHold = 2;
    }
    else if(keypad[0x3])
    {
        registers[Vx] = 0x3;
        keyHold = 3;
    }
    else if(keypad[0x4])
    {
        registers[Vx] = 0x4;
        keyHold = 4;
    }
    else if(keypad[0x5])
    {
        registers[Vx] = 0x5;
        keyHold = 5;
    }
    else if(keypad[0x6])
    {
        registers[Vx] = 0x6;
        keyHold = 6;
    }
    else if(keypad[0x7])
    {
        registers[Vx] = 0x7;
        keyHold = 7;
    }
    else if(keypad[0x8])
    {
        registers[Vx] = 0x8;
        keyHold = 8;
    }
    else if(keypad[0x9])
    {
        registers[Vx] = 0x9;
        keyHold = 9;
    }
    else if(keypad[0xA])
    {
        registers[Vx] = 0xA;
        keyHold = 10;
    }
    else if(keypad[0xB])
    {
        registers[Vx] = 0xB;
        keyHold = 11;
    }
    else if(keypad[0xC])
    {
        registers[Vx] = 0xC;
        keyHold = 12;
    }
    else if(keypad[0xD])
    {
        registers[Vx] = 0xD;
        keyHold = 13;
    }
    else if(keypad[0xE])
    {
        registers[Vx] = 0xE;
        keyHold = 14;
    }
    else if(keypad[0xF])
    {
        registers[Vx] = 0xF;
        keyHold = 15;
    }
    else // no key press, loop
    {
        pc -= 2;
    }
    
}

void Chip8::opFX15()
{
    delayTimer = registers[Vx];
}

void Chip8::opFX18()
{
    soundTimer = registers[Vx];
}

void Chip8::opFX1E()
{
    indexReg += registers[Vx];
}

void Chip8::opFX29()
{
    // character is either 0, 1, 2, 3, ..., D (13), E (14), F (15)
    // each character is represented by 5 bytes
    uint16_t character = registers[Vx];
    indexReg = 0x50 + (character * 5);
}

void Chip8::opFX33()
{
    uint16_t divisor{1000};
    for(uint8_t i{0}; i < 3; i++)
    {
        ram[indexReg + i] = (registers[Vx] % divisor) / (divisor/10);
        divisor /= 10;
    }
}

void Chip8::opFX55()
{
    for(uint8_t i{0}; i <= Vx; i++)
    {
        ram[indexReg + i] = registers[i];
    }

    indexReg += Vx + 1;
}

void Chip8::opFX65()
{
    for(uint8_t i{0}; i <= Vx; i++)
    {
        registers[i] = ram[indexReg + i];
    }

    indexReg += Vx + 1;
}
