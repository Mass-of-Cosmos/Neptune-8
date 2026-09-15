#ifndef CHIP_8_H
#define CHIP_8_H
#include <vector>
#include <cstdint>
#include <array>
#include <string>

class Chip8
{
private:
    std::vector<std::uint8_t> m_memory = std::vector<std::uint8_t>(4096);
    std::vector<uint8_t> m_fonts{
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
    std::vector<std::uint8_t> m_display = std::vector<std::uint8_t>(64 * 32);
    std::vector<uint16_t> m_stack{};
    uint8_t m_soundTimer{};
    uint8_t m_delayTimer{};
    // chip 8's layout
    // 1	2	3	C
    // 4	5	6	D
    // 7	8	9	E
    // A	0	B	F
    // the key layout is arranged in this order in the array: left to right, up to down in HEX
    std::array<int, 16> m_keys {
        0x1, 0x2, 0x3, 0xC,
        0x4, 0x5, 0x6, 0xD,
        0x7, 0x8, 0x9, 0xE,
        0xA, 0x0, 0xB, 0xF
    };
    uint16_t m_programCounter{0x200}; //aka PC
    uint16_t m_Iregister{};
    std::array<uint16_t, static_cast<std::size_t>(16)> m_GPRegisters{};

public:
   Chip8()
   {
       // load the fonts in memory starting at 0x050
       uint8_t startingMemoryIndex{0x050};
        for (uint8_t index {0}; index < m_fonts.size(); ++index )
        {
            m_memory[startingMemoryIndex] = m_fonts[index];
            ++startingMemoryIndex;
        }
   }
    void movePCNext() {m_programCounter += 2;}
    uint16_t getPC() const {return m_programCounter;}
    void setPC(uint16_t vlaue) {m_programCounter = vlaue;}
    bool loadRom(const std::string& path);

    // get the pixel at any specific coordinate
    std::size_t getPixelAtCoord(std::size_t x, std::size_t y) {return m_display[ (y * 64) + x ];}
    void setPixelAtCoord(std::size_t x, std::size_t y, std::size_t value) {m_display[ (y * 64) + x ] = value;}
    std::vector<uint8_t> getDisplay() const {return m_display; }
    void clearScreen();
    // return the opcode that PC is currently pointing at from memory | also it should move PC to the next instruction
    uint16_t fetch() { movePCNext(); return m_memory[ m_programCounter ];  }
    void pushPCToStack(uint16_t pc) {m_stack.push_back(pc);}
    // also returns the popped stack
    uint16_t popStack() { uint16_t poppedStack{m_stack.back()}; m_stack.pop_back(); return poppedStack; }
    uint16_t getGPRegister(std::size_t index) {return m_GPRegisters[index];}
    void setGPRegister(std::size_t indexOfRegister, uint16_t newValue) {m_GPRegisters[indexOfRegister] = newValue;}
    void setIRegister(uint16_t value) {m_Iregister = value;}
    uint16_t getIRegister() const {return m_Iregister;}
    uint16_t getMemmoryLocation( uint16_t locationIndex) {return m_memory[locationIndex];}
};

#endif