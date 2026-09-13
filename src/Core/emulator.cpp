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