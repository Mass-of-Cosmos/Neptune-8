#include "emulator.h"
#include "chip8.h"


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
    // The first thing to do is to get the X and Y coordinates from VX and VY.
    // Set the X coordinate to the value in VX modulo 64
    uint16_t XCoord{ m_chip8.getGPRegister(m_X & 63) };
    // Set the Y coordinate to the value in VY modulo 32
    uint16_t YCoord{ m_chip8.getGPRegister(m_Y & 31) };

    // set VF to 0
    m_chip8.setGPRegister(0xF, 0);

    // for N Rows:
    // Get the Nth byte of sprite data, counting from the memory address in the I register (I is not incremented)
    uint8_t NthByteOfSpriteData{ static_cast<uint8_t>(m_chip8.getMemmoryLocation(m_chip8.getIRegister() + static_cast<uint16_t>(m_N) )) };

    // For each of the 8 pixels/bits in this sprite row (from left to right, ie. from most to least significant bit):
    for (uint8_t i{0b10000000}; i != 0; i >> 1)
    {
        //If the current pixel in the sprite row is on and the pixel at coordinates X,Y on the screen is also on,
        //turn off the pixel and set VF to 1
        if ( (i & NthByteOfSpriteData) && (m_chip8.getPixelAtCoord(XCoord, YCoord)) )
        {
            m_chip8.setPixelAtCoord(XCoord, YCoord, 0);
            m_chip8.setGPRegister(0xF, 1);
        }
        // Or if the current pixel in the sprite row is on and the screen pixel is not,
        // draw the pixel at the X and Y coordinates
        else if ( (i & NthByteOfSpriteData) && !(m_chip8.getPixelAtCoord(XCoord, YCoord)) )
        {
            m_chip8.setPixelAtCoord(XCoord, YCoord, 1);
        }
        // If you reach the right edge of the screen, stop drawing this row
        if (m_X == 63)
            break;
        // Increment X (VX is not incremented)
        ++m_X;

        // Increment Y (VY is not incremented)
        ++m_Y;
        // Stop if you reach the bottom edge of the screen
        if (m_Y == 31)
            return;
    }
}
