#include "emulator.h"
#include "chip8.h"
#include "../Random.h"
#include <cassert>
#include <iostream>

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
    std::cerr << "opcode: o_0x00E0  is running...\n";

    m_chip8.clearScreen();
    m_shouldDraw = true;
}

void Emulator::o_0x2NNN()
{
    std::cerr << "opcode: o_0x2NNN  is running...\n";
    m_chip8.pushPCToStack(m_chip8.getPC());
    m_chip8.setPC(m_NNN);
}

void Emulator::o_0x00EE()
{
    std::cerr << "opcode: o_0x00EE  is running...\n";
    m_chip8.setPC(m_chip8.popStack());
}

void Emulator::o_0x3XNN()
{
    std::cerr << "opcode: o_0x3XNN  is running...\n";
    if (m_chip8.getGPRegister(m_X) == m_NN)
        m_chip8.movePCNext();
}

void Emulator::o_0x4XNN()
{
    std::cerr << "opcode: o_0x4XNN  is running...\n";
    if (m_chip8.getGPRegister(m_X) != m_NN)
        m_chip8.movePCNext();
}

void Emulator::o_0x5XY0()
{
    std::cerr << "opcode: o_0x5XY0  is running...\n";
    if (m_chip8.getGPRegister(m_X) == m_chip8.getGPRegister(m_Y))
        m_chip8.movePCNext();
}

void Emulator::o_0x9XY0()
{
    std::cerr << "opcode: o_0x9XY0  is running...\n";
    if (m_chip8.getGPRegister(m_X) != m_chip8.getGPRegister(m_Y))
        m_chip8.movePCNext();
}

void Emulator::o_0x6XNN()
{
    std::cerr << "opcode: o_0x6XNN  is running...\n";
    m_chip8.setGPRegister(m_X, m_NN);
}

void Emulator::o_0x7XNN()
{
    std::cerr << "opcode: o_0x7XNN  is running...\n";
    m_chip8.addToGPRigester(m_X, m_NN);
}

void Emulator::o_0xANNN()
{
    std::cerr << "opcode: o_0xANNN  is running...\n";
    m_chip8.setIRegister(m_NNN);
}

void Emulator::o_0x8XY0()
{
    std::cerr << "opcode: o_0x8XY0  is running...\n";
    m_chip8.setGPRegister(m_X, m_chip8.getGPRegister(m_Y));
}

void Emulator::o_0x8XY1()
{
    std::cerr << "opcode: o_0x8XY1  is running...\n";
    uint8_t VXAndVYOR{ static_cast<uint8_t>(m_chip8.getGPRegister(m_X) | m_chip8.getGPRegister(m_Y)) };
    m_chip8.setGPRegister(m_X, VXAndVYOR);
}

void Emulator::o_0x8XY2()
{
    std::cerr << "opcode: o_0x8XY2  is running...\n";
    uint8_t VXAndVYAND{ static_cast<uint8_t>(m_chip8.getGPRegister(m_X) & m_chip8.getGPRegister(m_Y)) };
    m_chip8.setGPRegister(m_X, VXAndVYAND);
}

void Emulator::o_0x8XY3()
{
    std::cerr << "opcode: o_0x8XY3  is running...\n";
    uint8_t VXAndVYXOR{ static_cast<uint8_t>(m_chip8.getGPRegister(m_X) ^ m_chip8.getGPRegister(m_Y)) };
    m_chip8.setGPRegister(m_X, VXAndVYXOR);
}

void Emulator::o_0x8XY4()
{
    std::cerr << "opcode: o_0x8XY4  is running...\n";
    //Unlike 7XNN, this addition will affect the carry flag.
    //If the result is larger than 255 (and thus overflows the 8-bit register VX),
    //the flag register VF is set to 1. If it doesn’t overflow, VF is set to 0.

    int theAddition{ m_chip8.getGPRegister(m_X) + m_chip8.getGPRegister(m_Y) };

    m_chip8.setGPRegister(m_X, theAddition);

if (theAddition > 255)
m_chip8.setGPRegister(0xF, 1);
else
m_chip8.setGPRegister(0xF, 0);

}

void Emulator::o_0x8XY6()
{
    std::cerr << "opcode: o_0x8XY6  is running...\n";
    // get the bit via bit mask
    uint8_t theShiftedBit{ (m_chip8.getGPRegister(m_X)) && 0b00000001 };

    // get and set the new (shifted) bit to the register
    uint8_t shiftedToRight{ static_cast<uint8_t>((m_chip8.getGPRegister(m_X)) >> 1) };
    m_chip8.setGPRegister(m_X, shiftedToRight);

    // set VF to theShiftedBit
    m_chip8.setGPRegister(0xF, theShiftedBit == 1 ? 1 : 0);
}

void Emulator::o_0x8XYE()
{
    std::cerr << "opcode: o_0x8XYE  is running...\n";
    // get the bit via bit mask
    uint8_t theShiftedBit{ (m_chip8.getGPRegister(m_X)) && 0b00000001 };

    // get and set the new (shifted) bit to the register
    uint8_t shiftedToLeft{ static_cast<uint8_t>((m_chip8.getGPRegister(m_X)) << 1) };
    m_chip8.setGPRegister(m_X, shiftedToLeft);

    // set VF to theShiftedBit
    m_chip8.setGPRegister(0xF, theShiftedBit == 1 ? 1 : 0);
}

void Emulator::o_0xBNNN()
{
    std::cerr << "opcode: o_0xBNNN  is running...\n";

    // In the original COSMAC VIP interpreter, this instruction jumped to the address NNN plus the value in the register V0
    // so we set PC to NNN + value in V0

    m_chip8.setPC(m_NNN + m_chip8.getGPRegister(0x0));
}

void Emulator::o_0xCXNN()
{
    std::cerr << "opcode: o_0xCXNN  is running...\n";
    // the number binary ANDed with NN
    uint8_t theNumber{ static_cast<uint8_t>((Random::get(0, 50)) & m_NN) };

    // put the result in VX
    m_chip8.setGPRegister(m_X, theNumber);
}

void Emulator::o_0xEX9E()
{
    std::cerr << "opcode: o_0xEX9E  is running...\n";
    std::size_t key{ static_cast<std::size_t>(m_chip8.getGPRegister(m_X)) & 0x0F };

    if (m_chip8.getKeys(key))
    {
        m_chip8.movePCNext();
    }
}

void Emulator::o_0xEXA1()
{
    std::cerr << "opcode: o_0xEXA1  is running...\n";
    std::size_t key{ static_cast<std::size_t>(m_chip8.getGPRegister(m_X)) & 0x0F };

    if (!(m_chip8.getKeys(key)))
    {
        m_chip8.movePCNext();
    }
}

void Emulator::o_0xFX07()
{
    std::cerr << "opcode: o_0xFX07  is running...\n";
    m_chip8.setGPRegister(m_X, m_chip8.getDelayTimer());
}

void Emulator::o_0xFX15()
{
    std::cerr << "opcode: o_0xFX15  is running...\n";
    m_chip8.setDealyTimer(m_chip8.getGPRegister(m_X));
}

void Emulator::o_0xFX18()
{
    std::cerr << "opcode: o_0xFX18  is running...\n";
    m_chip8.setSoundTimer(m_chip8.getGPRegister(m_X));
}

void Emulator::o_0xFX1E()
{
    std::cerr << "opcode: o_0xFX1E  is running...\n";
    m_chip8.setIRegister(m_chip8.getIRegister() + m_chip8.getGPRegister(m_X));
    if (m_chip8.getIRegister() > 0x1000)
        m_chip8.setGPRegister(0xF, 1);
}

void Emulator::o_0xFX0A(std::size_t input)
{
    std::cerr << "opcode: o_0xFX0A  is running...\n";
    while (true)
    {
        // if input is valid
        if ((input == 0x0) || (input == 0x1) || (input == 0x2) || (input == 0x3) ||
            (input == 0x4) || (input == 0x5) || (input == 0x6) || (input == 0x7) ||
            (input == 0x8) || (input == 0x9) || (input == 0xA) || (input == 0xB) ||
            (input == 0xC) || (input == 0xD) || (input == 0xE) || (input == 0xF))
        {
            m_chip8.setGPRegister(m_X, input);
            break;
        }
        else
            continue;
    }
}

void Emulator::o_0xFX29()
{
    std::cerr << "opcode: o_0xFX29  is running...\n";
    uint8_t fontRelatedNibble{ static_cast<uint8_t>(m_chip8.getGPRegister(m_X) & 0x000Fu) };

    // index register I is set to the address of the hexadecimal character in VX (its last nibble)
    const uint8_t startOfFontsInMemory { 0x050 };
    const uint8_t eachFontsLength{ 5 };
    m_chip8.setIRegister(startOfFontsInMemory + fontRelatedNibble * eachFontsLength);

    // Register I should point to the start of where the font is for the associated bytes that hold the font in the memory.
    // each 5 continious bytes represent 1 font; so if VX gave 0 --> 0x050 + (0 * 5)
    //                                              if VX gave 1 --> 0x050 + (1 * 5)
    //                                              Etc...
}

void Emulator::o_0xFX55()
{
    std::cerr << "opcode: o_0xFX55  is running...\n";

    uint16_t IRegVal{ m_chip8.getIRegister()};

    for (uint16_t i{ 0 }; i <= m_X; ++i)
    {
        uint16_t GPRegister{ m_chip8.getGPRegister(i)};

        m_chip8.loadToMemory(IRegVal + i, GPRegister);
    }
}

void Emulator::o_0xFX65()
{
    std::cerr << "opcode: o_0xFX65  is running...\n";
    uint16_t IRegVal{ m_chip8.getIRegister() };

    for (uint16_t i{ 0 }; i <= m_X; ++i)
    {
        uint8_t valueInMemory{ m_chip8.readFromMemory(IRegVal + i)};
        m_chip8.setGPRegister(i, valueInMemory);
    }
}

void Emulator::o_0xFX33()
{
    std::cerr << "opcode: o_0xFX33  is running...\n";

    std::array<uint8_t, 3> digits{};

    // get the digits (right to left)
    std::size_t i{ 0 };
    std::size_t j{ 0 };
    uint8_t VXGPRCopy{ m_chip8.getGPRegister(m_X) };
    while (j < 3)
    {
        if ((VXGPRCopy - i) % 10 != 0)
        {
            ++i;
            continue;
        }
        else if ((VXGPRCopy - i) % 10 == 0)
        {
            assert(j < digits.size() && " the array in o_0xFX33 is out of range!");
            digits[j] = i;
            VXGPRCopy /= 10;
            i = 0;
            ++j;
        }
    }

    int order{ 0 };
    // add the digits (left to right) to memory at address of I + n 
    // n is the order of digit from left to right
    for (int index{ 2 }; index >= 0; --index)
    {
        assert( index < digits.size() && " the array in o_0xFX33 is out of range!");
        m_chip8.loadToMemory(m_chip8.getIRegister() + static_cast<std::uint16_t>(order), digits[static_cast<std::size_t>(index)]);
        ++order;
    }
}


void Emulator::o_0xDXYN()
{
    std::cerr << "opcode: o_0xDXYN  is running...\n";
    uint8_t xCoord{static_cast<uint8_t>(m_chip8.getGPRegister(m_X) & 63)};
    uint8_t xCoordInitial{xCoord};
    uint8_t yCoord{static_cast<uint8_t>(m_chip8.getGPRegister(m_Y) & 31)};
    m_shouldDraw = true;

    // set VF to 0
    m_chip8.setGPRegister(0xF, 0);

    uint8_t totalRows{m_N};

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

void Emulator::execute(std::size_t input)
{
    if (m_opcode == 0x00E0)
    {
        o_0x00E0();
        return;
    }
    if (m_opcode == 0x00EE)
    {
        o_0x00EE();
        return;
    }

    switch (m_firstNibble)
    {
        case 1: o_0x1NNN(); break;
        case 2: o_0x2NNN(); break;
        case 3: o_0x3XNN(); break;
        case 4: o_0x4XNN(); break;
        case 5:
            switch (m_N)
            {
                case 0: o_0x5XY0(); break;
            } break;
        case 9:
            switch (m_N)
            {
                case 0: o_0x9XY0(); break;
            } break;
        case 6: o_0x6XNN(); break;
        case 7: o_0x7XNN(); break;
        case 8:
            switch (m_N)
            {
            case 0: o_0x8XY0(); break;
            case 1: o_0x8XY1(); break;
            case 2: o_0x8XY2(); break;
            case 3: o_0x8XY3(); break;
            case 4: o_0x8XY4(); break;
            case 5: o_0x8XY5(); break;
            case 6: o_0x8XY6(); break;
            case 7: o_0x8XY7(); break;
            case 0xE: o_0x8XYE(); break;
            } break;
        case 0xA: o_0xANNN(); break;
        case 0xB: o_0xBNNN(); break;
        case 0xC: o_0xCXNN(); break;
        case 0xD: o_0xDXYN(); break;
        case 0xE: 
            switch (m_Y)
            {
            case 9: o_0xEX9E(); break;
            case 0xA: if (m_N == 1) { o_0xEXA1(); } break;
            } break;
        case 0xF: switch (m_Y)
            {
        case 0: if (m_N == 7) { o_0xFX07(); } 
              else if (m_N == 0xA) { o_0xFX0A(input); } break;
        case 1: if (m_N == 5) { o_0xFX15(); }
              else if (m_N == 8) { o_0xFX18(); }
              else if (m_N == 0xE) { o_0xFX1E(); } break;
        case 2: if (m_N == 9) { o_0xFX29(); } break;
        case 3: if (m_N == 3) { o_0xFX33(); } break;
        case 5: if (m_N == 5) { o_0xFX55(); } break;
        case 6: if (m_N == 5) { o_0xFX65(); } break;

            } break;
    }
}