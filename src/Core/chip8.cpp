#include "Chip 8.h"
#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>

bool Chip8::loadRom(const std::string& path)
{
    std::error_code ec{};
    const auto theRomSize {std::filesystem::file_size(path, ec)};
    if (ec)
    {
        std::cerr << "Error finding ROM file: " << ec.message() << '\n';
        return false;
    }
    if (theRomSize > m_memory.size() - 512)
    {
        std::cerr<< "file too big\n";
        return false;
    }

    std::ifstream theRom{path, std::ios::binary};

    if (!theRom)
    {
        std::cerr << "Error loading rom file\n";
        return false;
    }

    std::cerr << "reading rom...\n";
    theRom.read(reinterpret_cast<char*>(&m_memory[512]), static_cast<std::streamsize>(theRomSize));
    return true;
}

void Chip8::clearScreen()
{
    for (auto& e : m_display)
        e = 0;
}