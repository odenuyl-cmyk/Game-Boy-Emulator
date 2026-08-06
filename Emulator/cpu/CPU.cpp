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
        case 0x11: // LD DE,n16
            PC++;
            E = memoryBus.read(PC);
            PC++;
            D = memoryBus.read(PC);
            break;
        case 0x12: // LD [DE],A
            memoryBus.write(getDE(), A);
            break;
        case 0x13:
            setDE(getDE() + 1);
            break;
        case 0x16: // LD D,n8
            PC++;
            D = memoryBus.read(PC);
            break;
        case 0x18: { // JR n8 (signed)
            PC++;
            auto offset = static_cast<int8_t>(memoryBus.read(PC));
            PC += offset;
            break;
        }
        case 0x1A: // LD A,[DE]
            A = memoryBus.read(getDE());
            break;
        case 0x1B: // DEC DE
            setDE(getDE() - 1);
            break;
        case 0x1E: // LD E,n8
            PC++;
            E = memoryBus.read(PC);
            break;
        case 0x21: // LD HL,n16
            PC++;
            L = memoryBus.read(PC);
            PC++;
            H = memoryBus.read(PC);
            break;
        case 0x22: // LD [HL+],A
            memoryBus.write(getHL(), A);
            setHL(getHL() + 1);
            break;
        case 0x23: // INC HL
            setHL(getHL() + 1);
            break;
        case 0x26: // LD H,n8
            PC++;
            H = memoryBus.read(PC);
            break;
        case 0x2A: // LD A,[HL+]
            A = memoryBus.read(getHL());
            setHL(getHL() + 1);
            break;
        case 0x2B: // DEC HL
            setHL(getHL() - 1);
            break;
        case 0x2E: // LD L,n8
            PC++;
            L = memoryBus.read(PC);
            break;
        case 0x31: { // LD SP,n16
            PC++;
            uint8_t lo = memoryBus.read(PC);
            PC++;
            uint8_t hi = memoryBus.read(PC);
            SP = hi << 8 | lo;
            break;
        }
        case 0x32: // LD [HL-],A
            memoryBus.write(getHL(), A);
            setHL(getHL() - 1);
            break;
        case 0x33: // INC SP
            SP++;
            break;
        case 0x36: // LD [HL],n8
            PC++;
            memoryBus.write(getHL(), memoryBus.read(PC));
            break;
        case 0x3A: // LD A,[HL-]
            A = memoryBus.read(getHL());
            setHL(getHL() - 1);
            break;
        case 0x3B: // DEC SP
            SP--;
            PC++;
        case 0x3E: // LD A,n8
            PC++;
            A = memoryBus.read(PC);
            break;
        case 0x40: // LD B,B
            B = B;
            break;
        case 0x41: // LD B,C
            B = C;
            break;
        case 0x42: // LD B,D
            B = D;
            break;
        case 0x43: // LD B,E
            B = E;
            break;
        case 0x44: // LD B,H
            B = H;
            break;
        case 0x45: // LD B,L
            B = L;
            break;
        case 0x46: // LD B,[HL]
            B = memoryBus.read(getHL());
            break;
        case 0x47: // LD B,A
            B = A;
            break;
        case 0x48: // LD C,B
            C = B;
            break;
        case 0x49: // LD C,C
            C = C;
            break;
        case 0x4A: // LD C,D
            C = D;
            break;
        case 0x4B: // LD C,E
            C = E;
            break;
        case 0x4C: // LD C,H
            C = H;
            break;
        case 0x4D: // LD C,L
            C = L;
            break;
        case 0x4E: // LD C,[HL]
            C = memoryBus.read(getHL());
            break;
        case 0x4F: // LD C,A
            C = A;
            break;
        case 0x50: // LD D,B
            D = B;
            break;
        case 0x51: // LD D,C
            D = C;
            break;
        case 0x52: // LD D,D
            D = D;
            break;
        case 0x53: // LD D,E
            D = E;
            break;
        case 0x54: // LD D,H
            D = H;
            break;
        case 0x55: // LD D,L
            D = L;
            break;
        case 0x56: // LD D,[HL]
            D = memoryBus.read(getHL());
            break;
        case 0x57: // LD D,A
            D = A;
            break;
        case 0x58: // LD E,B
            E = B;
            break;
        case 0x59: // LD E,C
            E = C;
            break;
        case 0x5A: // LD E,D
            E = D;
            break;
        case 0x5B: // LD E,E
            C = E;
            break;
        case 0x5C: // LD E,H
            E = H;
            break;
        case 0x5D: // LD E,L
            E = L;
            break;
        case 0x5E: // LD C,[HL]
            E = memoryBus.read(getHL());
            break;
        case 0x5F: // LD C,A
            E = A;
            break;
        case 0x60: // LD H,B
            H = B;
            break;
        case 0x61: // LD H,C
            H = C;
            break;
        case 0x62: // LD H,D
            H = D;
            break;
        case 0x63: // LD H,E
            H = E;
            break;
        case 0x64: // LD H,H
            H = H;
            break;
        case 0x65: // LD H,L
            H = L;
            break;
        case 0x66: // LD H,[HL]
            H = memoryBus.read(getHL());
            break;
        case 0x67: // LD H,A
            H = A;
            break;
        case 0x68: // LD L,B
            L = B;
            break;
        case 0x69: // LD L,C
            L = C;
            break;
        case 0x6A: // LD L,D
            L = D;
            break;
        case 0x6B: // LD L,E
            L = E;
            break;
        case 0x6C: // LD L,H
            L = H;
            break;
        case 0x6D: // LD L,L
            L = L;
            break;
        case 0x6E: // LD L,[HL]
            L = memoryBus.read(getHL());
            break;
        case 0x6F: // LD L,A
            L = A;
            break;
        case 0x70: // LD [HL],B
            memoryBus.write(getHL(), B);
            break;
        case 0x71: // LD [HL],C
            memoryBus.write(getHL(), C);
            break;
        case 0x72: // LD [HL],D
            memoryBus.write(getHL(), D);
            break;
        case 0x73: // LD [HL],E
            memoryBus.write(getHL(), E);
            break;
        case 0x74: // LD [HL],H
            memoryBus.write(getHL(), H);
            break;
        case 0x75: // LD [HL],L
            memoryBus.write(getHL(), L);
            break;
        case 0x76: // HALT
            // TODO: implement halting
            break;
        case 0x77: // LD [HL],A
            memoryBus.write(getHL(), A);
            break;
        case 0x78: // LD A,B
            A = B;
            break;
        case 0x79: // LD A,C
            A = C;
            break;
        case 0x7A: // LD A,D
            A = D;
            break;
        case 0x7B: // LD A,E
            A = E;
            break;
        case 0x7C: // LD A,H
            A = H;
            break;
        case 0x7D: // LD A,L
            A = L;
            break;
        case 0x7E: // LD A,[HL]
            A = memoryBus.read(getHL());
            break;
        case 0x7F: // LD A,A
            A = A;
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