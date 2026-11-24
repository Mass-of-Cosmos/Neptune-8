#include <iostream>
#include <string>
#include "core/emulator.h"
#include "core/chip8.h"
#include "frontend/SDLWindow.h"
#include <thread>
#include <chrono>
#include <limits>

int main( int argc, char* argv[] )
{
    // create the emulator
    Emulator emulator{};
    
    std::string romPath{};
    
    // get rom path
    while (true)
    {
        std::cout << "Enter the full path of a chip8 rom in your PC (e.g. 'D:\\Roms\\IBM Logo.ch8'):\n";
        std::getline(std::cin >> std::ws, romPath);

        // handling unextracted user input:
        if (!std::cin)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        // break if the user provided a succesfull path
        if (emulator.readRomPath(romPath))
            break;
    }

    // create SDL3 Window
    SDLWindow window{};

    

    // we aim for abot 700 instructions per second to control the speed of our emulator
    // 1 / 700 is 0.001428 seconds.
    // 1,428 microseconds is ~0.001428 seconds (~700 Hz)
    constexpr std::chrono::microseconds cycleDelay{ 1428 };
    bool shouldDrawFlag{};
    bool mainLoopFlag{true};
    SDL_Event sdlEvent;
    // main emulation loop
    while (mainLoopFlag)
    {
        //Handle events on queue
        while( SDL_PollEvent( &sdlEvent ) != 0 )
        {
            //User requests quit
            if( sdlEvent.type == SDL_EVENT_QUIT )
            {
                mainLoopFlag = false;
            }
        }

        // fetch and decode
        emulator.readOpcode();

        //execute
        emulator.execute();
        shouldDrawFlag = emulator.getShouldDraw();
        if (shouldDrawFlag)
        {
            window.updateAndRenderTexture(emulator.getDisplay());
        }

        // delay between each instrcution
        std::this_thread::sleep_for(cycleDelay);
    }

    return 0;
}