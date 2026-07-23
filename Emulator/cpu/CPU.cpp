#include "CPU.h"
#include "../memory/Bus.h"
#include "../debug/Debugger.h"

Debugger debugger;

CPU::CPU(Bus& memoryBus)
    : memoryBus(memoryBus)
{
}

void CPU::step()
{
    uint8_t operation = memoryBus.read(PC);
    debugger.logOperation(PC, operation);

    PC++;
}