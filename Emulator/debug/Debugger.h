#pragma once

#include "../cpu/CPU.h"
#include <cstdint>

class Debugger {
public:
    void logOperation(uint16_t pc, uint8_t opcode);
    void dumpCPU(const CPU& cpu);
};