#include "emulator.h"
#include "chip 8.h"


void Emulator::readOpcode()
{
    m_opcode = m_chip8.fetch();
    m_firstNibble = static_cast<uint8_t>( (m_opcode & 0xF000u) >> 12);
    m_X = static_cast<uint8_t>( (m_opcode & 0x0F00u) >> 8);
    m_Y = static_cast<uint8_t>( (m_opcode & 0x00F0u) >> 4);
    m_N = static_cast<uint8_t>( (m_opcode & 0x000Fu) );
    m_NN = static_cast<uint8_t>( (m_opcode & 0x00FFu));
    m_NNN = static_cast<uint16_t>( (m_opcode & 0x0FFFu) );
}

void Emulator::o_0x00E0u()
{
    m_chip8.clearScreen();
    m_shouldDraw = true;
}



/*
namespace Emulator {

    uint16_t opcode{};
    bool drawFlag { false };

    // Read the opcode that PC is currently pointing at from memory
    void fetch(Chip8& chip8)
    {
        opcode = chip8.getPC();
        chip8.movePCNext();

    }

    void decodeAndExecute(Chip8& chip8)
    {
        uint8_t firstNibble { static_cast<uint8_t>( (opcode & 0xF000u) >> 12) };
        uint8_t X { static_cast<uint8_t>( (opcode & 0x0F00u) >> 8) };
        uint8_t Y { static_cast<uint8_t>( (opcode & 0x00F0u) >> 4) };
        uint8_t N { static_cast<uint8_t>( (opcode & 0x000Fu) ) };
        uint8_t NN { static_cast<uint8_t>( (opcode & 0x00FFu)) };
        uint16_t NNN { static_cast<uint16_t>( (opcode & 0x0FFFu) ) };

        // clear screen
        if (opcode == 0x00E0u)
        {
            for ( auto& e : chip8.m_display.pixels)
                e = 0;
            drawFlag = true;
        }
        if (opcode == 0x00EEu)
        {
            if (!chip8.m_stack.lifoStack.empty())
                chip8.m_programCounter = chip8.m_stack.lifoStack.back();
            if (!chip8.m_stack.lifoStack.empty())
                chip8.m_stack.lifoStack.pop_back();
        }

        switch (firstNibble)
        {
            case 1:
                chip8.m_programCounter = NNN;
                return;
            case 2:
                chip8.m_stack.lifoStack.push_back(chip8.getPC());
                chip8.m_programCounter = NNN;
                return;
            case 3:
                if (chip8.m_GPRegisters[X] == NN)
                    chip8.movePCNext();
                return;
            case 4:
                if (chip8.m_GPRegisters[X] != NN)
                    chip8.movePCNext();
                return;
            case 5:
                switch (N)
                {
                    case 0:
                        if (chip8.m_GPRegisters[X] == chip8.m_GPRegisters[Y])
                            chip8.movePCNext();
                        return;
                }
                return;
            case 9:
                switch (N)
                {
                    case 0:
                        if (chip8.m_GPRegisters[X] != chip8.m_GPRegisters[Y])
                            chip8.movePCNext();
                        return;
                }
                return;
            case 6:
                chip8.m_GPRegisters[X] = NN;
                return;
            case 7:
                chip8.m_GPRegisters[X] += NN;
                return;
            case 0xA:
                chip8.m_Iregister = NNN;
                return;
            case 0xD:
                uint16_t xCoord { static_cast<uint16_t>(chip8.m_GPRegisters[X] & 63) };
                uint16_t yCoord { static_cast<uint16_t>(chip8.m_GPRegisters[Y] & 31) };
                chip8.m_GPRegisters[0xFu] = 0;
                uint8_t sprite { static_cast<uint8_t>(chip8.m_Iregister + N) };
                // if the pixel is ON
                if (sprite & 0b10000000)
                {
                    for (int i{0}; i <= 7; ++i)
                    {
                        uint8_t currentBit{0b10000000};
                        if ( (sprite & currentBit) && (chip8.m_display.getPixelAtCoord(xCoord, yCoord)))
                        {
                            chip8.m_display.setPixelAtCoord(xCoord, yCoord, 0);
                            chip8.m_GPRegisters[0xFu] =1;
                        }
                        else if ((sprite & currentBit) && !(chip8.m_display.getPixelAtCoord(xCoord, yCoord)))
                            {
                                chip8.m_GPRegisters[0xFu] = 1;
                                ++X;
                                if (xCoord == 63)
                                    continue;
                                chip8.m_display.setPixelAtCoord(xCoord, yCoord, 1);
                            }
                         ++Y;
                        if (yCoord == 32)
                            break;
                        currentBit >> 1;
                    }
                }
                drawFlag = true;
                break;
        }

    }

}
*/