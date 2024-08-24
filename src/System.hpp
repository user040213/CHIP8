#ifndef SYSTEM_H
#define SYSTEM_H

#include <SDL.h>
#include <chrono>
#include "Chip8.hpp"


class System
{
    public: 
        System(const char* winTitle, int windowWidth, int windowHeight, int texW, int texH);
        ~System();
        void update();
        void refresh(const void* pixels, int pitch);
        void loadSystem(std::string fileName);
        void loop();

    private:
        SDL_Window* windowObj;
        SDL_Texture* texture;
        SDL_Renderer* renderer;
        bool shutDown;
        Chip8 chipEmu;

        float clockTime;
        float delayTime;
        float drawTime;


        

};

#endif