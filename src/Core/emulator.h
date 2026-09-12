#ifndef EMULATOR_H
#define EMULATOR_H
#include <cstdint>
#include "chip 8.h"

class Emulator
{
    uint16_t m_opcode{};
    Chip8 m_chip8{};
    uint8_t m_firstNibble{};
    uint8_t m_X{};
    uint8_t m_Y{};
    uint8_t m_N{};
    uint8_t m_NN{};
    uint8_t m_NNN{};
    bool m_shouldDraw{false};

    // opcodes are prefixed with o_

    // clear screen
    void o_0x00E0u();

public:
    void readOpcode();

};

#endif
