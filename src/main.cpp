#include <iostream>
#include <string>
#include "core/emulator.h"
#include "core/chip8.h"
#include "frontend/SDLWindow.h"
#include <thread>
#include <chrono>
#include <limits>

namespace DefaultValues
{
    constexpr std::size_t inputCode{ 120 };
}

// converts user's keyboard inputs to the corresponding keypad's hex adress
std::size_t getInputState()
{
    static const bool* pressedButton = SDL_GetKeyboardState(nullptr);
    
    if  (pressedButton[SDL_SCANCODE_1]) return 0x1u;
    else if (pressedButton[SDL_SCANCODE_2]) return 0x2u;
    else if (pressedButton[SDL_SCANCODE_3]) return 0x3u;
    else if (pressedButton[SDL_SCANCODE_4]) return 0xCu;
    else if (pressedButton[SDL_SCANCODE_Q]) return 0x4u;
    else if (pressedButton[SDL_SCANCODE_W]) return 0x5u;
    else if (pressedButton[SDL_SCANCODE_E]) return 0x6u;
    else if (pressedButton[SDL_SCANCODE_R]) return 0xDu;
    else if (pressedButton[SDL_SCANCODE_A]) return 0x7u;
    else if (pressedButton[SDL_SCANCODE_S]) return 0x8u;
    else if (pressedButton[SDL_SCANCODE_D]) return 0x9u;
    else if (pressedButton[SDL_SCANCODE_F]) return 0xEu;
    else if (pressedButton[SDL_SCANCODE_Z]) return 0xAu;
    else if (pressedButton[SDL_SCANCODE_X]) return 0x0u;
    else if (pressedButton[SDL_SCANCODE_C]) return 0xBu;
    else if (pressedButton[SDL_SCANCODE_V]) return 0xFu;
    else return DefaultValues::inputCode;
    
}

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
    SDL_Event sdlEvent;
    bool mainLoopFlag{true};
    // main emulation loop
    
    while (mainLoopFlag)
    {
      
        //Handle events on queue
        while (SDL_PollEvent(&sdlEvent) != 0)
        {
            //User requests quit
            if (sdlEvent.type == SDL_EVENT_QUIT)
            {
                mainLoopFlag = false;
            }
                
        }

            // fetch and decode
            emulator.readOpcode();

            //execute
            emulator.execute(getInputState());
        
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