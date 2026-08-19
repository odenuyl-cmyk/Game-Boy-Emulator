#include "Bus.h"

#include <iostream>
#include <ostream>

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

uint8_t Bus::read(uint16_t address) {
    switch (getMemoryType(address)) {
        case MemoryType::ROM0:
            return ROM0[address];
        case MemoryType::ROMX:
            return ROMX[address - 0x4000];
        case MemoryType::VRAM:
            return VRAM[address - 0x8000];
        case MemoryType::EXT_RAM:
            return EXT_RAM[address - 0xA000];
        case MemoryType::WRAM0:
            return WRAM0[address - 0xC000];
        case MemoryType::WRAMX:
            return WRAMX[address - 0xD000];
        case MemoryType::ECHO_RAM:
            return read(address - 0x2000);
        case MemoryType::OAM:
            return OAM[address - 0xFE00];
        case MemoryType::UNUSABLE:
            return 0xFF;
        case MemoryType::IO_REGISTER:
            return IO[address - 0xFF00];
        case MemoryType::HRAM:
            return HRAM[address - 0xFF80];
        case MemoryType::IE:
            return IE;
        default:
            std::cerr << "Unknown memory type" << std::endl;
            std::exit(1);
    }
}

void Bus::write(uint16_t address, uint8_t value) {
    switch (getMemoryType(address)) {
        case MemoryType::ROM0:
            ROM0[address] = value;
            break;
        case MemoryType::ROMX:
            ROMX[address - 0x4000] = value;
            break;
        case MemoryType::VRAM:
            VRAM[address - 0x8000] = value;
            break;
        case MemoryType::EXT_RAM:
            EXT_RAM[address - 0xA000] = value;
            break;
        case MemoryType::WRAM0:
            WRAM0[address - 0xC000] = value;
            break;
        case MemoryType::WRAMX:
            WRAMX[address - 0xD000] = value;
            break;
        case MemoryType::ECHO_RAM:
            write(address - 0x2000, value);
            break;
        case MemoryType::OAM:
            OAM[address - 0xFE00] = value;
            break;
        case MemoryType::UNUSABLE:
            break;
        case MemoryType::IO_REGISTER:
            IO[address - 0xFF00] = value;
            break;
        case MemoryType::HRAM:
            HRAM[address - 0xFF80] = value;
            break;
        case MemoryType::IE:
            IE = value;
            break;
        default:
            std::cerr << "Unknown memory type" << std::endl;
            std::exit(1);
    }
}
