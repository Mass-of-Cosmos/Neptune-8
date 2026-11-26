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


    // Logical and arithmetic instructions

    // VX is set to the value of VY.
    void o_0x8XY0();
    // VX is set to the bitwise/binary logical disjunction (OR) of VX and VY. VY is not affected.
    void o_0x8XY1();
    // VX is set to the bitwise/binary logical conjunction (AND) of VX and VY. VY is not affected.
    void o_0x8XY2();
    // VX is set to the bitwise/binary exclusive OR (XOR) of VX and VY. VY is not affected.
    void o_0x8XY3();
    // VX is set to the value of VX plus the value of VY. VY is not affected.
    void o_0x8XY4();
    // shift value of VX to right; set VF to the 1 or 0 (depending on the shifted bit)
    void o_0x8XY6();
    // shift value of VX to left; set VF to the 1 or 0 (depending on the shifted bit)
    void o_0x8XYE();
    // jump with offset
    void o_0xBNNN();
    // This instruction generates a random number, binary ANDs it with the value NN, and puts the result in VX.
    void o_0xCXNN();
    // will skip one instruction (increment PC by 2) if the key corresponding to the value in VX is pressed.
    void o_0xEX9E();
    // skips if the key corresponding to the value in VX is not pressed.
    void o_0xEXA1();

    // Timer related opcodes
 
    // sets VX to the current value of the delay timer
    void o_0xFX07();
    // sets the delay timer to the value in VX
    void o_0xFX15();
    // sets the sound timer to the value in VX
    void o_0xFX18();
    // The index register I will get the value in VX added to it; set VF to 1 if I “overflows” from 0FFF to above 1000 
    void o_0xFX1E();
    // get key
    void o_0xFX0A(std::size_t input);
    // Fonts related opcode
    void o_0xFX29();
    // the value of each variable register from V0 to VX inclusive (if X is 0, then only V0)
    // will be stored in successive memory addresses, starting with the one that’s stored in I
    void o_0xFX55();
    // this does the opposite; it takes the value stored at the memory addresses and loads them into the variable registers instead.
    void o_0xFX65();
    // it takes the number in VX (which is one byte, so it can be any number from 0 to 255)
    // and converts it to three decimal digits, storing these digits in memory at the address in the index register I.
    void o_0xFX33();

public:
    void readOpcode();
    void execute(std::size_t input);

    // get the m_shouldDraw flag
    bool getShouldDraw() const { return m_shouldDraw; }
    std::vector<uint8_t> getDisplay() const { return m_chip8.getDisplay(); }
    bool readRomPath(std::string& romPath) {return m_chip8.loadRom(romPath); }

};

#endif
