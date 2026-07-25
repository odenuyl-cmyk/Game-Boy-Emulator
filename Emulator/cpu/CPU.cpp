#include "CPU.h"
#include "../memory/Bus.h"
#include "../debug/Debugger.h"

CPU::CPU(Bus& memoryBus, Debugger& debugger)
    : memoryBus(memoryBus), debugger(debugger)
{
}

void CPU::step()
{
    uint8_t operation = memoryBus.read(PC);
    debugger.logOperation(PC, operation);

    switch (operation) {
        case 0x00: // NOP
            break;
        case 0x06: // LD B,d8
            PC++;
            B = memoryBus.read(PC);
            break;
        case 0x0E: // LD C,d8
            PC++;
            C = memoryBus.read(PC);
            break;
        case 0x16: // LD D,d8
            PC++;
            D = memoryBus.read(PC);
            break;
        case 0x1E: // LD E,d8
            PC++;
            E = memoryBus.read(PC);
            break;
        case 0x26: // LD H,d8
            PC++;
            H = memoryBus.read(PC);
            break;
        case 0x2E: // LD L,d8
            PC++;
            L = memoryBus.read(PC);
            break;
        case 0x3E: // LD A,d8
            PC++;
            A = memoryBus.read(PC);
            break;
        case 0x80: // ADD A,B
            A += B;
            break;
        case 0x81: // ADD A,C
            A += C;
            break;
        case 0x82: // ADD A,D
            A += D;
            break;
        case 0x83: // ADD A,E
            A += E;
            break;
        case 0x84: // ADD A,H
            A += H;
            break;
        case 0x85: // ADD A,L
            A += L;
            break;
        case 0x87: // ADD A,A
            A += A;
            break;
        case 0x90: // SUB A,B
            A -= B;
            break;
        case 0x91: // SUB A,C
            A -= C;
            break;
        case 0x92: // SUB A,D
            A -= D;
            break;
        case 0x93: // SUB A,E
            A -= E;
            break;
        case 0x94: // SUB A,H
            A -= H;
            break;
        case 0x95: // SUB A,L
            A -= L;
            break;
        case 0x97: // SUB A,A
            A -= A;
            break;
        default: // unknown opcode
            break;
    }

    PC++;
}
