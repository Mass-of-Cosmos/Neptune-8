#ifndef EMULATOR_H
#define EMULATOR_H
#include <cstdint>
#include "chip8.h"

class Emulator
{
    uint16_t m_opcode{};
    Chip8 m_chip8{};
    uint8_t m_firstNibble{};
    uint8_t m_X{};
    uint8_t m_Y{};
    uint8_t m_N{};
    uint8_t m_NN{};
    uint16_t m_NNN{};
    bool m_shouldDraw{false};

    // opcodes are prefixed with o_

    // clear screen
    void o_0x00E0();
    // sets PC to NNN
    void o_0x1NNN() { m_chip8.setPC(m_NNN);}
    // remove the last address from the stack and set the PC to it.
    void o_0x00EE();
    // push the current PC to the stack. then set PC to NNN.
    void o_0x2NNN();
    // will skip one instruction if the value in VX is equal to NN
    void o_0x3XNN();
    //   will skip one instruction if the value in VX is NOT equal to NN
    void o_0x4XNN();
    // skips if the values in VX and VY are equal
    void o_0x5XY0();
    // skips if the values in VX and VY are NOT equal
    void o_0x9XY0();
    // set VX to NN
    void o_0x6XNN();
    // Add the value NN to VX
    void o_0x7XNN();
    // sets the index register I to the value NNN
    void o_0xANNN();
    // the display opcode
    void o_0xDXYN();

public:
    void readOpcode();
    void execute();

};

#endif
