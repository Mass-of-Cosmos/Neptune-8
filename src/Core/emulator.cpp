#include "emulator.h"
#include "chip8.h"


void Emulator::readOpcode()
{
    m_opcode = m_chip8.fetch();
    m_firstNibble = static_cast<uint8_t>((m_opcode & 0xF000u) >> 12);
    m_X = static_cast<uint8_t>((m_opcode & 0x0F00u) >> 8);
    m_Y = static_cast<uint8_t>((m_opcode & 0x00F0u) >> 4);
    m_N = static_cast<uint8_t>((m_opcode & 0x000Fu));
    m_NN = static_cast<uint8_t>((m_opcode & 0x00FFu));
    m_NNN = static_cast<uint16_t>((m_opcode & 0x0FFFu));
}

void Emulator::o_0x00E0()
{
    m_chip8.clearScreen();
    m_shouldDraw = true;
}

void Emulator::o_0x2NNN()
{
    m_chip8.pushPCToStack(m_chip8.getPC());
    m_chip8.setPC(m_NNN);
}

void Emulator::o_0x00EE()
{
    m_chip8.setPC(m_chip8.popStack());
}

void Emulator::o_0x3XNN()
{
    if (m_chip8.getGPRegister(m_X) == m_NN)
        m_chip8.movePCNext();
}

void Emulator::o_0x4XNN()
{
    if (m_chip8.getGPRegister(m_X) != m_NN)
        m_chip8.movePCNext();
}

void Emulator::o_0x5XY0()
{
    if (m_chip8.getGPRegister(m_X) == m_chip8.getGPRegister(m_Y))
        m_chip8.movePCNext();
}

void Emulator::o_0x9XY0()
{
    if (m_chip8.getGPRegister(m_X) != m_chip8.getGPRegister(m_Y))
        m_chip8.movePCNext();
}

void Emulator::o_0x6XNN()
{
    m_chip8.setGPRegister(m_X, m_NN);
}

void Emulator::o_0x7XNN()
{
    m_chip8.setGPRegister(m_X, m_NN);
}

void Emulator::o_0xANNN()
{
    m_chip8.setIRegister(m_NNN);
}

void Emulator::o_0xDXYN()
{
    uint8_t xCoord{static_cast<uint8_t>(m_chip8.getGPRegister(m_X) & 63)};
    uint8_t xCoordInitial{ xCoord };
    uint8_t yCoord{static_cast<uint8_t>(m_chip8.getGPRegister(m_Y) & 31)};

    // set VF to 0
    m_chip8.setGPRegister(0xF, 0);

    uint8_t totalRows{ m_N };

    for (std::size_t row{0}; row < totalRows; ++row)
    {
        xCoord = xCoordInitial;
        // get Nth byte of sprite data
        uint8_t nthByteSprite{static_cast<uint8_t>(m_chip8.getMemmoryLocation(m_chip8.getIRegister() + row))};
        for (int i{0}; i < 8; ++i)
        {
            uint8_t currentPixelInSprite{};
            currentPixelInSprite = static_cast<uint8_t>(nthByteSprite & 0b10000000);
            //If the current pixel in the sprite row is on and the pixel at coordinates X,Y on the screen is also on
            if (currentPixelInSprite && m_chip8.getPixelAtCoord(xCoord, yCoord))
            {
                m_chip8.setPixelAtCoord(xCoord, yCoord, 0); // turn off the pixel
                m_chip8.setGPRegister(0xF, 1); // set VF to 1
            }
            // if the current pixel in the sprite row is on and the screen pixel is not
            else if (currentPixelInSprite && (m_chip8.getPixelAtCoord(xCoord, yCoord) == 0))
            {
                m_chip8.setPixelAtCoord(xCoord, yCoord, 1); // Draw the Pixel
            }
            nthByteSprite <<= 1; // move to the next bit for the next iteration (should be in the end of our for loop)

            ++xCoord;
            if (xCoord >= 64)
            {
                break;
            }
        }
        ++yCoord;
        if (yCoord >= 32)
        {
            return;
        }
    }
}
