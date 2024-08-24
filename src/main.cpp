#include "System.hpp"
#include "Chip8.hpp"

int main(int argc, char* argv[])
{
    System mainSys("CHIP-8", DISPLAY_COLUMNS*10, DISPLAY_ROWS*10, DISPLAY_COLUMNS, DISPLAY_ROWS);
    
    mainSys.loadSystem(argv[1]);
    mainSys.loop();

    return 0;
}