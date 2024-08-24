#include "System.hpp"

System::System(const char *winTitle, int windowWidth, int windowHeight, int texW, int texH)
{
    chipEmu = Chip8();
    SDL_InitSubSystem(SDL_INIT_VIDEO);
    
    windowObj = SDL_CreateWindow(winTitle, 50, 50, windowWidth, windowHeight, 0);
    renderer = SDL_CreateRenderer(windowObj, -1, 0);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB332, SDL_TEXTUREACCESS_STREAMING, texW, texH);

    shutDown = false;

    clockTime = (1/(float)CLOCKHZ)*1000; // ms clock period
    delayTime = (1/(float)DELAYHZ)*1000;
    drawTime = (1/(float)DRAWHZ)*1000;
}

System::~System()
{
    SDL_DestroyWindow(windowObj);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(texture);
    SDL_Quit();
}

void System::update()
{
    SDL_Event event{0};
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
            {
                shutDown = true;
                break;
            }

            case SDL_KEYDOWN:
            {
                // use SDL scancodes for multiple keyboard layout compatibility
                switch(event.key.keysym.scancode)
                {
                    case SDL_SCANCODE_1:
                        chipEmu.keypad[0x1] = 1;
                        break;
                    case SDL_SCANCODE_2:
                        chipEmu.keypad[0x2] = 1;
                        break;
                    case SDL_SCANCODE_3:
                        chipEmu.keypad[0x3] = 1;
                        break;
                    case SDL_SCANCODE_4:
                        chipEmu.keypad[0xC] = 1;
                        break;
                    case SDL_SCANCODE_Q:
                        chipEmu.keypad[0x4] = 1;
                        break;
                    case SDL_SCANCODE_W:
                        chipEmu.keypad[0x5] = 1;
                        break;
                    case SDL_SCANCODE_E:
                        chipEmu.keypad[0x6] = 1;
                        break;
                    case SDL_SCANCODE_R:
                        chipEmu.keypad[0xD] = 1;
                        break;
                    case SDL_SCANCODE_A:
                        chipEmu.keypad[0x7] = 1;
                        break;
                    case SDL_SCANCODE_S:
                        chipEmu.keypad[0x8] = 1;
                        break;
                    case SDL_SCANCODE_D:
                        chipEmu.keypad[0x9] = 1;
                        break;
                    case SDL_SCANCODE_F:
                        chipEmu.keypad[0xE] = 1;
                        break;
                    case SDL_SCANCODE_Z:
                        chipEmu.keypad[0xA] = 1;
                        break;
                    case SDL_SCANCODE_X:
                        chipEmu.keypad[0x0] = 1;
                        break;
                    case SDL_SCANCODE_C:
                        chipEmu.keypad[0xB] = 1;
                        break;
                    case SDL_SCANCODE_V:
                        chipEmu.keypad[0xF] = 1;
                        break;
                }
                break;
            }

            case SDL_KEYUP:
                switch(event.key.keysym.scancode)
                {
                    case SDL_SCANCODE_1:
                        chipEmu.keypad[0x1] = 0;
                        break;
                    case SDL_SCANCODE_2:
                        chipEmu.keypad[0x2] = 0;
                        break;
                    case SDL_SCANCODE_3:
                        chipEmu.keypad[0x3] = 0;
                        break;
                    case SDL_SCANCODE_4:
                        chipEmu.keypad[0xC] = 0;
                        break;
                    case SDL_SCANCODE_Q:
                        chipEmu.keypad[0x4] = 0;
                        break;
                    case SDL_SCANCODE_W:
                        chipEmu.keypad[0x5] = 0;
                        break;
                    case SDL_SCANCODE_E:
                        chipEmu.keypad[0x6] = 0;
                        break;
                    case SDL_SCANCODE_R:
                        chipEmu.keypad[0xD] = 0;
                        break;
                    case SDL_SCANCODE_A:
                        chipEmu.keypad[0x7] = 0;
                        break;
                    case SDL_SCANCODE_S:
                        chipEmu.keypad[0x8] = 0;
                        break;
                    case SDL_SCANCODE_D:
                        chipEmu.keypad[0x9] = 0;
                        break;
                    case SDL_SCANCODE_F:
                        chipEmu.keypad[0xE] = 0;
                        break;
                    case SDL_SCANCODE_Z:
                        chipEmu.keypad[0xA] = 0;
                        break;
                    case SDL_SCANCODE_X:
                        chipEmu.keypad[0x0] = 0;
                        break;
                    case SDL_SCANCODE_C:
                        chipEmu.keypad[0xB] = 0;
                        break;
                    case SDL_SCANCODE_V:
                        chipEmu.keypad[0xF] = 0;
                        break;
                }
                break;
        }
    }
}

void System::refresh(const void* pixels, int pitch)
{
    SDL_UpdateTexture(texture, NULL, pixels, pitch);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void System::loadSystem(std::string fileName)
{
    chipEmu.loadROM(fileName);
}

void System::loop()
{

    auto lastCycle = std::chrono::high_resolution_clock::now();
    auto lastTimer = std::chrono::high_resolution_clock::now();
    auto lastDraw = std::chrono::high_resolution_clock::now();

    while(!shutDown)
    {

        update();

        auto curTime = std::chrono::high_resolution_clock::now();
        auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(curTime - lastCycle).count(); 
        auto deltaTime2 = std::chrono::duration_cast<std::chrono::milliseconds>(curTime - lastTimer).count();
        auto deltaTime3 = std::chrono::duration_cast<std::chrono::milliseconds>(curTime - lastDraw).count();

        // CPU frequency
        if(deltaTime >= clockTime)
        {
            lastCycle = curTime;
            chipEmu.run();
        }

        // Delay Frequency
        if(deltaTime2 >= delayTime)
        {
            lastTimer = curTime;
            chipEmu.tickTimers();

        }

        // Draw Frequency
        if(deltaTime3 >= drawTime)
        {
            lastDraw = curTime;
            refresh(chipEmu.display, sizeof(chipEmu.display[0]) * DISPLAY_COLUMNS);
        }

    }
}
