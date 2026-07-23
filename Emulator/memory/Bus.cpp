#include "Bus.h"

uint8_t Bus::read(uint16_t address) {
    return memory[address];
}

void Bus::write(uint16_t address, uint8_t value) {
    memory[address] = value;
}

MemoryType Bus::getMemoryType(uint16_t address) {
    if (address <= 0x3FFF) {
        return MemoryType::ROM0;
    }

    if (address <= 0x7FFF) {
        return MemoryType::ROMX;
    }

    if (address <= 0x9FFF) {
        return MemoryType::VRAM;
    }

    if (address <= 0xBFFF) {
        return MemoryType::EXT_RAM;
    }

    if (address <= 0xCFFF) {
        return MemoryType::WRAM0;
    }

    if (address <= 0xDFFF) {
        return MemoryType::WRAMX;
    }

    if (address <= 0xFDFF) {
        return MemoryType::ECHO_RAM;
    }

    if (address <= 0xFE9F) {
        return MemoryType::OAM;
    }

    if (address <= 0xFEFF) {
        return MemoryType::UNUSABLE;
    }

    if (address <= 0xFF7F) {
        return MemoryType::IO_REGISTER;
    }

    if (address <= 0xFFFE) {
        return MemoryType::HRAM;
    }

    return MemoryType::IE;
}
