#pragma once
#include <array>
#include <cstdint>

enum class MemoryType : uint8_t {
    ROM0 = 0,
    ROMX = 1,
    VRAM = 2,
    EXT_RAM = 3,
    WRAM0 = 4,
    WRAMX = 5,
    ECHO_RAM = 6,
    OAM = 7,
    UNUSABLE = 8,
    IO_REGISTER = 9,
    HRAM = 10,
    IE = 11
};

class Bus {
public:
    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t value);

private:
    static MemoryType getMemoryType(uint16_t address);
    std::array<uint8_t, 0x10000> memory{};
};
