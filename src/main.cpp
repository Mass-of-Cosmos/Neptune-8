#include <iostream>
#include <string>
#include "core/emulator.h"
#include "core/chip8.h"
#include "frontend/SDLWindow.h"

int main( int argc, char* argv[] )
{
    std::string romPath{};
    std::cout << "Enter the path of a chip8 rom:\n";
    std::cin >> romPath;

    Emulator emulator{};
    SDLWindow window{};

    while (true)
    {

    }

    return 0;
}