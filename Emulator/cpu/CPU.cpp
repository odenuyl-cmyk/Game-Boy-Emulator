#include "CPU.h"
#include "../memory/Bus.h"
#include "../debug/Debugger.h"

CPU::CPU(Bus& memoryBus, Debugger& debugger)
    : memoryBus(memoryBus), debugger(debugger)
{
}

uint16_t CPU::getBC() {

}

void CPU::step()
{
    uint8_t operation = memoryBus.read(PC);
    debugger.logOperation(PC, operation);

    switch (operation) {
        case 0x00: // NOP
            break;
        case 0x01: // LD BC,n16
            PC++;
            C = memoryBus.read(PC);
            PC++;
            B = memoryBus.read(PC);
            break;
        case 0x02: // LD [BC],A
            memoryBus.write(getBC(), A);
            break;
        case 0x03: // INC BC
            setBC(getBC() + 1);
            break;
        case 0x06: // LD B,n8
            PC++;
            B = memoryBus.read(PC);
            break;
        case 0x08: { // LD [n16],SP
            PC++;
            uint8_t al = memoryBus.read(PC);
            PC++;
            uint8_t ah = memoryBus.read(PC);
            uint16_t address = (ah << 8) | al;
            memoryBus.write(address, SP & 0xFF);
            memoryBus.write(address + 1, SP >> 8);
            break;
        }
        case 0x0A: // LD A,[BC]
            A = memoryBus.read(getBC());
            break;
        case 0x0B:
            setBC(getBC() - 1);
            break;
        case 0x0E: // LD C,n8
            PC++;
            C = memoryBus.read(PC);
            break;
        case 0x10: // STOP n8
            PC++;
            // TODO: stop CPU
            break;
        case 0x16: // LD D,n8
            PC++;
            D = memoryBus.read(PC);
            break;
        case 0x1E: // LD E,n8
            PC++;
            E = memoryBus.read(PC);
            break;
        case 0x26: // LD H,n8
            PC++;
            H = memoryBus.read(PC);
            break;
        case 0x2E: // LD L,n8
            PC++;
            L = memoryBus.read(PC);
            break;
        case 0x3E: // LD A,n8
            PC++;
            A = memoryBus.read(PC);
            break;
        case 0x80: // ADD A,B
            // TODO: set flags
            A += B;
            break;
        case 0x81: // ADD A,C
            // TODO: set flags
            A += C;
            break;
        case 0x82: // ADD A,D
            // TODO: set flags
            A += D;
            break;
        case 0x83: // ADD A,E
            // TODO: set flags
            A += E;
            break;
        case 0x84: // ADD A,H
            // TODO: set flags
            A += H;
            break;
        case 0x85: // ADD A,L
            // TODO: set flags
            A += L;
            break;
        case 0x87: // ADD A,A
            // TODO: set flags
            A += A;
            break;
        case 0x90: // SUB A,B
            // TODO: set flags
            A -= B;
            break;
        case 0x91: // SUB A,C
            // TODO: set flags
            A -= C;
            break;
        case 0x92: // SUB A,D
            // TODO: set flags
            A -= D;
            break;
        case 0x93: // SUB A,E
            // TODO: set flags
            A -= E;
            break;
        case 0x94: // SUB A,H
            // TODO: set flags
            A -= H;
            break;
        case 0x95: // SUB A,L
            // TODO: set flags
            A -= L;
            break;
        case 0x97: // SUB A,A
            // TODO: set flags
            A -= A;
            break;
        default: // unknown opcode
            break;
    }

    PC++;
}