#include "CPU.h"

#include <regex>

#include "../memory/Bus.h"
#include "../debug/Debugger.h"

CPU::CPU(Bus& memoryBus, Debugger& debugger)
    : memoryBus(memoryBus), debugger(debugger)
{
}

void CPU::inc(uint8_t& reg) {
    uint8_t reg_old = reg;
    reg += 1;
    setflag_Z(reg == 0);
    setflag_N(false);
    setflag_H((reg_old & 0x0F) == 0x0F);
}

void CPU::dec(uint8_t& reg) {
    uint8_t reg_old = reg;
    reg -= 1;
    setflag_Z(reg == 0);
    setflag_N(true);
    setflag_H((reg_old & 0x0F) == 0x00);
}

void CPU::add8(uint8_t &reg1, uint8_t &reg2) {
    uint8_t result = reg1 + reg2;
    setflag_Z(result == 0);
    setflag_N(false);
    setflag_H((reg1 & 0x0F) + (reg2 & 0x0F) > 0x0F);
    setflag_C(static_cast<uint16_t>(reg1) + static_cast<uint16_t>(reg2) > 0xFF);
    reg1 = result;
}

uint16_t CPU::add16(uint16_t reg1, uint16_t reg2) {
    setflag_N(false);
    setflag_H((reg1 & 0x0FFF) + (reg2 & 0x0FFF) > 0x0FFF);
    setflag_C(static_cast<uint32_t>(reg1) + static_cast<uint32_t>(reg2) > 0xFFFF);
    return reg1 + reg2;
}

void CPU::sub(uint8_t &reg1, uint8_t &reg2) {
    uint8_t result = reg1 - reg2;
    setflag_Z(result == 0);
    setflag_N(true);
    setflag_H((reg1 & 0x0F) < (reg2 & 0x0F));
    setflag_C(reg1 < reg2);
    reg1 = result;
}

void CPU::adc(uint8_t &reg1, uint8_t &reg2) {
    bool old_C = getflag_C();
    uint8_t result = reg1 + reg2 + old_C;
    setflag_Z(result == 0);
    setflag_N(false);
    setflag_H((reg1 & 0x0F) + (reg2 & 0x0F) + old_C > 0x0F);
    setflag_C(static_cast<uint16_t>(reg1) + static_cast<uint16_t>(reg2) + old_C > 0xFF);
    reg1 = result;
}

void CPU::sbc(uint8_t &reg1, uint8_t &reg2) {
    bool old_C = getflag_C();
    uint8_t result = reg1 - reg2 - old_C;
    setflag_Z(result == 0);
    setflag_N(true);
    setflag_H((reg1 & 0x0F) < (reg2 & 0x0F) + old_C);
    setflag_C(reg1 < reg2 + old_C);
    reg1 = result;
}

void CPU::AND(uint8_t &reg1, uint8_t &reg2) {
    uint8_t result = reg1 & reg2;
    setflag_Z(result == 0);
    setflag_N(false);
    setflag_H(true);
    setflag_C(false);
    reg1 = result;
}

void CPU::XOR(uint8_t &reg1, uint8_t &reg2) {
    uint8_t result = reg1 ^ reg2;
    setflag_Z(result == 0);
    setflag_N(false);
    setflag_H(false);
    setflag_C(false);
    reg1 = result;
}

void CPU::OR(uint8_t &reg1, uint8_t &reg2) {
    uint8_t result = reg1 | reg2;
    setflag_Z(result == 0);
    setflag_N(false);
    setflag_H(false);
    setflag_C(false);
    reg1 = result;
}

void CPU::CP(uint8_t &reg1, uint8_t &reg2) {
    uint8_t result = reg1 - reg2;
    setflag_Z(result == 0);
    setflag_N(true);
    setflag_H((reg1 & 0x0F) < (reg2 & 0x0F));
    setflag_C(reg1 < reg2);
}

void CPU::CB_BIT(uint8_t bitNum, uint8_t &reg) {
    uint8_t bit = reg & (1 << bitNum);
    setflag_Z(bit == 0);
    setflag_N(false);
    setflag_H(true);
}

void CPU::exec_CB(uint8_t operation) {
    switch (operation) {
        case 0x40: // BIT 0,B
            CB_BIT(0, B);
            break;
        case 0x41: // BIT 0,C
            CB_BIT(0, C);
            break;
        case 0x42: // BIT 0,D
            CB_BIT(0, D);
            break;
        case 0x43: // BIT 0,E
            CB_BIT(0, E);
            break;
        case 0x44: // BIT 0,H
            CB_BIT(0, H);
            break;
        case 0x45: // BIT 0,L
            CB_BIT(0, L);
            break;
        case 0x46: { // BIT 0,[HL
            uint8_t value = memoryBus.read(getHL());
            CB_BIT(0, value);
            break;
        }
        case 0x47: // BIT 0,A
            CB_BIT(0, A);
            break;
        case 0x48: // BIT 1,B
            CB_BIT(1, B);
            break;
        case 0x49: // BIT 1,C
            CB_BIT(1, C);
            break;
        case 0x4A: // BIT 1,D
            CB_BIT(1, D);
            break;
        case 0x4B: // BIT 1,E
            CB_BIT(1, E);
            break;
        case 0x4C: // BIT 1,H
            CB_BIT(1, H);
            break;
        case 0x4D: // BIT 1,L
            CB_BIT(1, L);
            break;
        case 0x4E: { // BIT 1,[HL]
            uint8_t value = memoryBus.read(getHL());
            CB_BIT(1, value);
            break;
        }
        case 0x4F: // BIT 1,A
            CB_BIT(1, A);
            break;
        case 0x50: // BIT 2,B
            CB_BIT(2, B);
            break;
        case 0x51: // BIT 2,C
            CB_BIT(2, C);
            break;
        case 0x52: // BIT 2,D
            CB_BIT(2, D);
            break;
        case 0x53: // BIT 2,E
            CB_BIT(2, E);
            break;
        case 0x54: // BIT 2,H
            CB_BIT(2, H);
            break;
        case 0x55: // BIT 2,L
            CB_BIT(2, L);
            break;
        case 0x56: { // BIT 2,[HL]
            uint8_t value = memoryBus.read(getHL());
            CB_BIT(2, value);
            break;
        }
        case 0x57: // BIT 2,A
            CB_BIT(2, A);
            break;
        case 0x58: // BIT 3,B
            CB_BIT(3, B);
            break;
        case 0x59: // BIT 3,C
            CB_BIT(3, C);
            break;
        case 0x5A: // BIT 3,D
            CB_BIT(3, D);
            break;
        case 0x5B: // BIT 3,E
            CB_BIT(3, E);
            break;
        case 0x5C: // BIT 3,H
            CB_BIT(3, H);
            break;
        case 0x5D: // BIT 3,L
            CB_BIT(3, L);
            break;
        case 0x5E: { // BIT 3,[HL]
            uint8_t value = memoryBus.read(getHL());
            CB_BIT(3, value);
            break;
        }
        case 0x5F: // BIT 3,A
            CB_BIT(3, A);
            break;
        case 0x60: // BIT 4,B
            CB_BIT(4, B);
            break;
        case 0x61: // BIT 4,C
            CB_BIT(4, C);
            break;
        case 0x62: // BIT 4,D
            CB_BIT(4, D);
            break;
        case 0x63: // BIT 4,E
            CB_BIT(4, E);
            break;
        case 0x64: // BIT 4,H
            CB_BIT(4, H);
            break;
        case 0x65: // BIT 4,L
            CB_BIT(4, L);
            break;
        case 0x66: { // BIT 4,[HL]
            uint8_t value = memoryBus.read(getHL());
            CB_BIT(4, value);
            break;
        }
        case 0x67: // BIT 4,A
            CB_BIT(4, A);
            break;
        case 0x68: // BIT 5,B
            CB_BIT(5, B);
            break;
        case 0x69: // BIT 5,C
            CB_BIT(5, C);
            break;
        case 0x6A: // BIT 5,D
            CB_BIT(5, D);
            break;
        case 0x6B: // BIT 5,E
            CB_BIT(5, E);
            break;
        case 0x6C: // BIT 5,H
            CB_BIT(5, H);
            break;
        case 0x6D: // BIT 5,L
            CB_BIT(5, L);
            break;
        case 0x6E: { // BIT 5,[HL]
            uint8_t value = memoryBus.read(getHL());
            CB_BIT(5, value);
            break;
        }
        case 0x6F: // BIT 5,A
            CB_BIT(5, A);
            break;
        case 0x70: // BIT 6,B
            CB_BIT(6, B);
            break;
        case 0x71: // BIT 6,C
            CB_BIT(6, C);
            break;
        case 0x72: // BIT 6,D
            CB_BIT(6, D);
            break;
        case 0x73: // BIT 6,E
            CB_BIT(6, E);
            break;
        case 0x74: // BIT 6,H
            CB_BIT(6, H);
            break;
        case 0x75: // BIT 6,L
            CB_BIT(6, L);
            break;
        case 0x76: { // BIT 6,[HL]
            uint8_t value = memoryBus.read(getHL());
            CB_BIT(6, value);
            break;
        }
        case 0x77: // BIT 6,A
            CB_BIT(6, A);
            break;
        case 0x78: // BIT 7,B
            CB_BIT(7, B);
            break;
        case 0x79: // BIT 7,C
            CB_BIT(7, C);
            break;
        case 0x7A: // BIT 7,D
            CB_BIT(7, D);
            break;
        case 0x7B: // BIT 7,E
            CB_BIT(7, E);
            break;
        case 0x7C: // BIT 7,H
            CB_BIT(7, H);
            break;
        case 0x7D: // BIT 7,L
            CB_BIT(7, L);
            break;
        case 0x7E: { // BIT 7,[HL]
            uint8_t value = memoryBus.read(getHL());
            CB_BIT(7, value);
            break;
        }
        case 0x7F: // BIT 7,A
            CB_BIT(7, A);
            break;
        case 0x80: // RES 0,B
            B &= ~(1 << 0);
            break;
        case 0x81: // RES 0,C
            C &= ~(1 << 0);
            break;
        case 0x82: // RES 0,D
            D &= ~(1 << 0);
            break;
        case 0x83: // RES 0,E
            E &= ~(1 << 0);
            break;
        case 0x84: // RES 0,H
            H &= ~(1 << 0);
            break;
        case 0x85: // RES 0,L
            L &= ~(1 << 0);
            break;
        case 0x86: // RES 0,[HL]
            memoryBus.write(getHL(), memoryBus.read(getHL()) & ~(1 << 0));
            break;
        case 0x87: // RES 0,A
            A &= ~(1 << 0);
            break;
        case 0x88: // RES 1,B
            B &= ~(1 << 1);
            break;
        case 0x89: // RES 1,C
            C &= ~(1 << 1);
            break;
        case 0x8A: // RES 1,D
            D &= ~(1 << 1);
            break;
        case 0x8B: // RES 1,E
            E &= ~(1 << 1);
            break;
        case 0x8C: // RES 1,H
            H &= ~(1 << 1);
            break;
        case 0x8D: // RES 1,L
            L &= ~(1 << 1);
            break;
        case 0x8E: // RES 1,[HL]
            memoryBus.write(getHL(), memoryBus.read(getHL()) & ~(1 << 1));
            break;
        case 0x8F: // RES 1,A
            A &= ~(1 << 1);
            break;
        case 0x90: // RES 2,B
            B &= ~(1 << 2);
            break;
        case 0x91: // RES 2,C
            C &= ~(1 << 2);
            break;
        case 0x92: // RES 2,D
            D &= ~(1 << 2);
            break;
        case 0x93: // RES 2,E
            E &= ~(1 << 2);
            break;
        case 0x94: // RES 2,H
            H &= ~(1 << 2);
            break;
        case 0x95: // RES 2,L
            L &= ~(1 << 2);
            break;
        case 0x96: // RES 2,[HL]
            memoryBus.write(getHL(), memoryBus.read(getHL()) & ~(1 << 2));
            break;
        case 0x97: // RES 2,A
            A &= ~(1 << 2);
            break;
        case 0x98: // RES 3,B
            B &= ~(1 << 3);
            break;
        case 0x99: // RES 3,C
            C &= ~(1 << 3);
            break;
        case 0x9A: // RES 3,D
            D &= ~(1 << 3);
            break;
        case 0x9B: // RES 3,E
            E &= ~(1 << 3);
            break;
        case 0x9C: // RES 3,H
            H &= ~(1 << 3);
            break;
        case 0x9D: // RES 3,L
            L &= ~(1 << 3);
            break;
        case 0x9E: // RES 3,[HL]
            memoryBus.write(getHL(), memoryBus.read(getHL()) & ~(1 << 3));
            break;
        case 0x9F: // RES 3,A
            A &= ~(1 << 3);
            break;
        case 0xA0: // RES 4,B
            B &= ~(1 << 4);
            break;
        case 0xA1: // RES 4,C
            C &= ~(1 << 4);
            break;
        case 0xA2: // RES 4,D
            D &= ~(1 << 4);
            break;
        case 0xA3: // RES 4,E
            E &= ~(1 << 4);
            break;
        case 0xA4: // RES 4,H
            H &= ~(1 << 4);
            break;
        case 0xA5: // RES 4,L
            L &= ~(1 << 4);
            break;
        case 0xA6: // RES 4,[HL]
            memoryBus.write(getHL(), memoryBus.read(getHL()) & ~(1 << 4));
            break;
        case 0xA7: // RES 4,A
            A &= ~(1 << 4);
            break;
        case 0xA8: // RES 5,B
            B &= ~(1 << 5);
            break;
        case 0xA9: // RES 5,C
            C &= ~(1 << 5);
            break;
        case 0xAA: // RES 5,D
            D &= ~(1 << 5);
            break;
        case 0xAB: // RES 5,E
            E &= ~(1 << 5);
            break;
        case 0xAC: // RES 5,H
            H &= ~(1 << 5);
            break;
        case 0xAD: // RES 5,L
            L &= ~(1 << 5);
            break;
        case 0xAE: // RES 5,[HL]
            memoryBus.write(getHL(), memoryBus.read(getHL()) & ~(1 << 5));
            break;
        case 0xAF: // RES 5,A
            A &= ~(1 << 5);
            break;
        case 0xB0: // RES 6,B
            B &= ~(1 << 6);
            break;
        case 0xB1: // RES 6,C
            C &= ~(1 << 6);
            break;
        case 0xB2: // RES 6,D
            D &= ~(1 << 6);
            break;
        case 0xB3: // RES 6,E
            E &= ~(1 << 6);
            break;
        case 0xB4: // RES 6,H
            H &= ~(1 << 6);
            break;
        case 0xB5: // RES 6,L
            L &= ~(1 << 6);
            break;
        case 0xB6: // RES 6,[HL]
            memoryBus.write(getHL(), memoryBus.read(getHL()) & ~(1 << 6));
            break;
        case 0xB7: // RES 6,A
            A &= ~(1 << 6);
            break;
        case 0xB8: // RES 7,B
            B &= ~(1 << 7);
            break;
        case 0xB9: // RES 7,C
            C &= ~(1 << 7);
            break;
        case 0xBA: // RES 7,D
            D &= ~(1 << 7);
            break;
        case 0xBB: // RES 7,E
            E &= ~(1 << 7);
            break;
        case 0xBC: // RES 7,H
            H &= ~(1 << 7);
            break;
        case 0xBD: // RES 7,L
            L &= ~(1 << 7);
            break;
        case 0xBE: // RES 7,[HL]
            memoryBus.write(getHL(), memoryBus.read(getHL()) & ~(1 << 7));
            break;
        case 0xBF: // RES 7,A
            A &= ~(1 << 7);
            break;
        case 0xC0: // SET 0,B
            B |= (1 << 0);
            break;
        case 0xC1: // SET 0,C
            C |= (1 << 0);
            break;
        case 0xC2: // SET 0,D
            D |= (1 << 0);
            break;
        case 0xC3: // SET 0,E
            E |= (1 << 0);
            break;
        case 0xC4: // SET 0,H
            H |= (1 << 0);
            break;
        case 0xC5: // SET 0,L
            L |= (1 << 0);
            break;
        case 0xC6: // SET 0,[HL]
            memoryBus.write(getHL(), memoryBus.read(getHL()) | (1 << 0));
            break;
        case 0xC7: // SET 0,A
            A |= (1 << 0);
            break;
        case 0xC8: // SET 1,B
            B |= (1 << 1);
            break;
        case 0xC9: // SET 1,C
            C |= (1 << 1);
            break;
        case 0xCA: // SET 1,D
            D |= (1 << 1);
            break;
        case 0xCB: // SET 1,E
            E |= (1 << 1);
            break;
        case 0xCC: // SET 1,H
            H |= (1 << 1);
            break;
        case 0xCD: // SET 1,L
            L |= (1 << 1);
            break;
        case 0xCE: // SET 1,[HL]
            memoryBus.write(getHL(), memoryBus.read(getHL()) | (1 << 1));
            break;
        case 0xCF: // SET 1,A
            A |= (1 << 1);
            break;
        case 0xD0: // SET 2,B
            B |= (1 << 2);
            break;
        case 0xD1: // SET 2,C
            C |= (1 << 2);
            break;
        case 0xD2: // SET 2,D
            D |= (1 << 2);
            break;
        case 0xD3: // SET 2,E
            E |= (1 << 2);
            break;
        case 0xD4: // SET 2,H
            H |= (1 << 2);
            break;
        case 0xD5: // SET 2,L
            L |= (1 << 2);
            break;
        case 0xD6: // SET 2,[HL]
            memoryBus.write(getHL(), memoryBus.read(getHL()) | (1 << 2));
            break;
        case 0xD7: // SET 2,A
            A |= (1 << 2);
            break;
        case 0xD8: // SET 3,B
            B |= (1 << 3);
            break;
        case 0xD9: // SET 3,C
            C |= (1 << 3);
            break;
        case 0xDA: // SET 3,D
            D |= (1 << 3);
            break;
        case 0xDB: // SET 3,E
            E |= (1 << 3);
            break;
        case 0xDC: // SET 3,H
            H |= (1 << 3);
            break;
        case 0xDD: // SET 3,L
            L |= (1 << 3);
            break;
        case 0xDE: // SET 3,[HL]
            memoryBus.write(getHL(), memoryBus.read(getHL()) | (1 << 3));
            break;
        case 0xDF: // SET 3,A
            A |= (1 << 3);
            break;
        case 0xE0: // SET 4,B
            B |= (1 << 4);
            break;
        case 0xE1: // SET 4,C
            C |= (1 << 4);
            break;
        case 0xE2: // SET 4,D
            D |= (1 << 4);
            break;
        case 0xE3: // SET 4,E
            E |= (1 << 4);
            break;
        case 0xE4: // SET 4,H
            H |= (1 << 4);
            break;
        case 0xE5: // SET 4,L
            L |= (1 << 4);
            break;
        case 0xE6: // SET 4,[HL]
            memoryBus.write(getHL(), memoryBus.read(getHL()) | (1 << 4));
            break;
        case 0xE7: // SET 4,A
            A |= (1 << 4);
            break;
        case 0xE8: // SET 5,B
            B |= (1 << 5);
            break;
        case 0xE9: // SET 5,C
            C |= (1 << 5);
            break;
        case 0xEA: // SET 5,D
            D |= (1 << 5);
            break;
        case 0xEB: // SET 5,E
            E |= (1 << 5);
            break;
        case 0xEC: // SET 5,H
            H |= (1 << 5);
            break;
        case 0xED: // SET 5,L
            L |= (1 << 5);
            break;
        case 0xEE: // SET 5,[HL]
            memoryBus.write(getHL(), memoryBus.read(getHL()) | (1 << 5));
            break;
        case 0xEF: // SET 5,A
            A |= (1 << 5);
            break;
        case 0xF0: // SET 6,B
            B |= (1 << 6);
            break;
        case 0xF1: // SET 6,C
            C |= (1 << 6);
            break;
        case 0xF2: // SET 6,D
            D |= (1 << 6);
            break;
        case 0xF3: // SET 6,E
            E |= (1 << 6);
            break;
        case 0xF4: // SET 6,H
            H |= (1 << 6);
            break;
        case 0xF5: // SET 6,L
            L |= (1 << 6);
            break;
        case 0xF6: // SET 6,[HL]
            memoryBus.write(getHL(), memoryBus.read(getHL()) | (1 << 6));
            break;
        case 0xF7: // SET 6,A
            A |= (1 << 6);
            break;
        case 0xF8: // SET 7,B
            B |= (1 << 7);
            break;
        case 0xF9: // SET 7,C
            C |= (1 << 7);
            break;
        case 0xFA: // SET 7,D
            D |= (1 << 7);
            break;
        case 0xFB: // SET 7,E
            E |= (1 << 7);
            break;
        case 0xFC: // SET 7,H
            H |= (1 << 7);
            break;
        case 0xFD: // SET 7,L
            L |= (1 << 7);
            break;
        case 0xFE: // SET 7,[HL]
            memoryBus.write(getHL(), memoryBus.read(getHL()) | (1 << 7));
            break;
        case 0xFF: // SET 7,A
            A |= (1 << 7);
            break;
        default: // unknown opcode
            break;
    }
}

void CPU::step()
{
    uint8_t operation = memoryBus.read(PC);
    debugger.logOperation(PC, operation);

    switch (operation) {
        case 0x00: // NOP
            PC++;
            break;
        case 0x01: // LD BC,n16
            PC++;
            C = memoryBus.read(PC);
            PC++;
            B = memoryBus.read(PC);
            PC++;
            break;
        case 0x02: // LD [BC],A
            memoryBus.write(getBC(), A);
            PC++;
            break;
        case 0x03: // INC BC
            setBC(getBC() + 1);
            PC++;
            break;
        case 0x04: // INC B
            inc(B);
            PC++;
            break;
        case 0x05: // DEC B
            dec(B);
            PC++;
            break;
        case 0x06: // LD B,n8
            PC++;
            B = memoryBus.read(PC);
            PC++;
            break;
        case 0x07: { // RLCA
            bool old_bit7 = A & 0x80;
            A = (A << 1) | (A >> 7);
            setflag_Z(false);
            setflag_N(false);
            setflag_H(false);
            setflag_C(old_bit7);
            PC++;
            break;
        }
        case 0x08: { // LD [n16],SP
            PC++;
            uint8_t lo = memoryBus.read(PC);
            PC++;
            uint8_t hi = memoryBus.read(PC);
            uint16_t address = (hi << 8) | lo;
            memoryBus.write(address, SP & 0xFF);
            memoryBus.write(address + 1, SP >> 8);
            PC++;
            break;
        }
        case 0x09: // ADD HL,BC
            setHL(add16(getHL(), getBC()));
            PC++;
            break;
        case 0x0A: // LD A,[BC]
            A = memoryBus.read(getBC());
            PC++;
            break;
        case 0x0B:
            setBC(getBC() - 1);
            PC++;
            break;
        case 0x0C: // INC C
            inc(C);
            PC++;
            break;
        case 0x0D: // DEC C
            dec(C);
            PC++;
            break;
        case 0x0E: // LD C,n8
            PC++;
            C = memoryBus.read(PC);
            PC++;
            break;
        case 0x0F: { // RRCA
            bool old_bit0 = A & 0x01;
            A = (A >> 1) | (A << 7);
            setflag_Z(false);
            setflag_N(false);
            setflag_H(false);
            setflag_C(old_bit0);
            PC++;
            break;
        }
        case 0x10: // STOP n8
            PC++;
            // TODO: stop CPU
            PC++;
            break;
        case 0x11: // LD DE,n16
            PC++;
            E = memoryBus.read(PC);
            PC++;
            D = memoryBus.read(PC);
            PC++;
            break;
        case 0x12: // LD [DE],A
            memoryBus.write(getDE(), A);
            PC++;
            break;
        case 0x13:
            setDE(getDE() + 1);
            PC++;
            break;
        case 0x14: // INC D
            inc(D);
            PC++;
            break;
        case 0x15: // DEC D
            dec(D);
            PC++;
            break;
        case 0x16: // LD D,n8
            PC++;
            D = memoryBus.read(PC);
            PC++;
            break;
        case 0x17: { // RLA
            bool old_bit7 = A & 0x80;
            A = (A << 1) | (getflag_C());
            setflag_Z(false);
            setflag_N(false);
            setflag_H(false);
            setflag_C(old_bit7);
            PC++;
            break;
        }
        case 0x18: { // JR n8 (signed)
            PC++;
            auto offset = static_cast<int8_t>(memoryBus.read(PC));
            PC++;
            PC += offset;
            break;
        }
        case 0x19: // ADD HL,DE
            setHL(add16(getHL(), getDE()));
            PC++;
            break;
        case 0x1A: // LD A,[DE]
            A = memoryBus.read(getDE());
            PC++;
            break;
        case 0x1B: // DEC DE
            setDE(getDE() - 1);
            PC++;
            break;
        case 0x1C: // INC E
            inc(E);
            PC++;
            break;
        case 0x1D: // DEC E
            dec(E);
            PC++;
            break;
        case 0x1E: // LD E,n8
            PC++;
            E = memoryBus.read(PC);
            PC++;
            break;
        case 0x1F: { // RRA
            bool old_bit0 = A & 0x01;
            A = (A >> 1) | (getflag_C() << 7);
            setflag_Z(false);
            setflag_N(false);
            setflag_H(false);
            setflag_C(old_bit0);
            PC++;
            break;
        }
        case 0x20: { // JR NZ n8 (signed)
            PC++;
            auto offset = static_cast<int8_t>(memoryBus.read(PC));
            PC++;
            if (!getflag_Z()) {
                PC += offset;
            }
            break;
        }
        case 0x21: // LD HL,n16
            PC++;
            L = memoryBus.read(PC);
            PC++;
            H = memoryBus.read(PC);
            PC++;
            break;
        case 0x22: // LD [HL+],A
            memoryBus.write(getHL(), A);
            setHL(getHL() + 1);
            PC++;
            break;
        case 0x23: // INC HL
            setHL(getHL() + 1);
            PC++;
            break;
        case 0x24: // INC H
            inc(H);
            PC++;
            break;
        case 0x25: // DEC H
            dec(H);
            PC++;
            break;
        case 0x26: // LD H,n8
            PC++;
            H = memoryBus.read(PC);
            PC++;
            break;
        case 0x27: { // DAA
            bool N = getflag_N();
            bool H = getflag_H();
            bool C = getflag_C();
            uint8_t adj = 0;
            if (N) {
                if (H) {
                    adj += 0x06;
                }
                if (C) {
                    adj += 0x60;
                }
                A -= adj;
            } else {
                if (H || ((A & 0x0F) > 0x09)) {
                    adj += 0x06;
                }
                if (C || (A > 0x99)) {
                    adj += 0x60;
                    setflag_C(true);
                }
                A += adj;
            }
            setflag_Z(A == 0);
            setflag_H(false);
            PC++;
            break;
        }
        case 0x28: { // JR Z n8 (signed)
            PC++;
            auto offset = static_cast<int8_t>(memoryBus.read(PC));
            PC++;
            if (getflag_Z()) {
                PC += offset;
            }
            break;
        }
        case 0x29: // ADD HL,HL
            setHL(add16(getHL(), getHL()));
            PC++;
            break;
        case 0x2A: // LD A,[HL+]
            A = memoryBus.read(getHL());
            setHL(getHL() + 1);
            PC++;
            break;
        case 0x2B: // DEC HL
            setHL(getHL() - 1);
            PC++;
            break;
        case 0x2C: // INC L
            inc(L);
            PC++;
            break;
        case 0x2D: // DEC L
            dec(L);
            PC++;
            break;
        case 0x2E: // LD L,n8
            PC++;
            L = memoryBus.read(PC);
            PC++;
            break;
        case 0x2F: // CPL
            A = ~A;
            setflag_N(true);
            setflag_H(true);
            PC++;
            break;
        case 0x30: { // JR NC n8 (signed)
            PC++;
            auto offset = static_cast<int8_t>(memoryBus.read(PC));
            PC++;
            if (!getflag_C()) {
                PC += offset;
            }
            break;
        }
        case 0x31: { // LD SP,n16
            PC++;
            uint8_t lo = memoryBus.read(PC);
            PC++;
            uint8_t hi = memoryBus.read(PC);
            SP = hi << 8 | lo;
            PC++;
            break;
        }
        case 0x32: // LD [HL-],A
            memoryBus.write(getHL(), A);
            setHL(getHL() - 1);
            PC++;
            break;
        case 0x33: // INC SP
            SP++;
            PC++;
            break;
        case 0x34: { // INC [HL]
            uint8_t value = memoryBus.read(getHL());
            inc(value);
            memoryBus.write(getHL(), value);
            PC++;
            break;
        }
        case 0x35: { // DEC [HL]
            uint8_t value = memoryBus.read(getHL());
            dec(value);
            memoryBus.write(getHL(), value);
            PC++;
            break;
        }
        case 0x36: // LD [HL],n8
            PC++;
            memoryBus.write(getHL(), memoryBus.read(PC));
            PC++;
            break;
        case 0x37: // SCF
            setflag_N(false);
            setflag_H(false);
            setflag_C(true);
            PC++;
            break;
        case 0x38: { // JR C n8 (signed)
            PC++;
            auto offset = static_cast<int8_t>(memoryBus.read(PC));
            PC++;
            if (getflag_C()) {
                PC += offset;
            }
            break;
        }
        case 0x39: // ADD HL,SP
            setHL(add16(getHL(), getSP()));
            PC++;
            break;
        case 0x3A: // LD A,[HL-]
            A = memoryBus.read(getHL());
            setHL(getHL() - 1);
            PC++;
            break;
        case 0x3B: // DEC SP
            SP--;
            PC++;
            break;
        case 0x3C: // INC A
            inc(A);
            PC++;
            break;
        case 0x3D: // DEC A
            dec(A);
            PC++;
            break;
        case 0x3E: // LD A,n8
            PC++;
            A = memoryBus.read(PC);
            PC++;
            break;
        case 0x3F: // CCF
            setflag_N(false);
            setflag_H(false);
            setflag_C(!getflag_C());
            PC++;
            break;
        case 0x40: // LD B,B
            B = B;
            PC++;
            break;
        case 0x41: // LD B,C
            B = C;
            PC++;
            break;
        case 0x42: // LD B,D
            B = D;
            PC++;
            break;
        case 0x43: // LD B,E
            B = E;
            PC++;
            break;
        case 0x44: // LD B,H
            B = H;
            PC++;
            break;
        case 0x45: // LD B,L
            B = L;
            PC++;
            break;
        case 0x46: // LD B,[HL]
            B = memoryBus.read(getHL());
            PC++;
            break;
        case 0x47: // LD B,A
            B = A;
            PC++;
            break;
        case 0x48: // LD C,B
            C = B;
            PC++;
            break;
        case 0x49: // LD C,C
            C = C;
            PC++;
            break;
        case 0x4A: // LD C,D
            C = D;
            PC++;
            break;
        case 0x4B: // LD C,E
            C = E;
            PC++;
            break;
        case 0x4C: // LD C,H
            C = H;
            PC++;
            break;
        case 0x4D: // LD C,L
            C = L;
            PC++;
            break;
        case 0x4E: // LD C,[HL]
            C = memoryBus.read(getHL());
            PC++;
            break;
        case 0x4F: // LD C,A
            C = A;
            PC++;
            break;
        case 0x50: // LD D,B
            D = B;
            PC++;
            break;
        case 0x51: // LD D,C
            D = C;
            PC++;
            break;
        case 0x52: // LD D,D
            D = D;
            PC++;
            break;
        case 0x53: // LD D,E
            D = E;
            PC++;
            break;
        case 0x54: // LD D,H
            D = H;
            PC++;
            break;
        case 0x55: // LD D,L
            D = L;
            PC++;
            break;
        case 0x56: // LD D,[HL]
            D = memoryBus.read(getHL());
            PC++;
            break;
        case 0x57: // LD D,A
            D = A;
            PC++;
            break;
        case 0x58: // LD E,B
            E = B;
            PC++;
            break;
        case 0x59: // LD E,C
            E = C;
            PC++;
            break;
        case 0x5A: // LD E,D
            E = D;
            PC++;
            break;
        case 0x5B: // LD E,E
            E = E;
            PC++;
            break;
        case 0x5C: // LD E,H
            E = H;
            PC++;
            break;
        case 0x5D: // LD E,L
            E = L;
            PC++;
            break;
        case 0x5E: // LD E,[HL]
            E = memoryBus.read(getHL());
            PC++;
            break;
        case 0x5F: // LD E,A
            E = A;
            PC++;
            break;
        case 0x60: // LD H,B
            H = B;
            PC++;
            break;
        case 0x61: // LD H,C
            H = C;
            PC++;
            break;
        case 0x62: // LD H,D
            H = D;
            PC++;
            break;
        case 0x63: // LD H,E
            H = E;
            PC++;
            break;
        case 0x64: // LD H,H
            H = H;
            PC++;
            break;
        case 0x65: // LD H,L
            H = L;
            PC++;
            break;
        case 0x66: // LD H,[HL]
            H = memoryBus.read(getHL());
            PC++;
            break;
        case 0x67: // LD H,A
            H = A;
            PC++;
            break;
        case 0x68: // LD L,B
            L = B;
            PC++;
            break;
        case 0x69: // LD L,C
            L = C;
            PC++;
            break;
        case 0x6A: // LD L,D
            L = D;
            PC++;
            break;
        case 0x6B: // LD L,E
            L = E;
            PC++;
            break;
        case 0x6C: // LD L,H
            L = H;
            PC++;
            break;
        case 0x6D: // LD L,L
            L = L;
            PC++;
            break;
        case 0x6E: // LD L,[HL]
            L = memoryBus.read(getHL());
            PC++;
            break;
        case 0x6F: // LD L,A
            L = A;
            PC++;
            break;
        case 0x70: // LD [HL],B
            memoryBus.write(getHL(), B);
            PC++;
            break;
        case 0x71: // LD [HL],C
            memoryBus.write(getHL(), C);
            PC++;
            break;
        case 0x72: // LD [HL],D
            memoryBus.write(getHL(), D);
            PC++;
            break;
        case 0x73: // LD [HL],E
            memoryBus.write(getHL(), E);
            PC++;
            break;
        case 0x74: // LD [HL],H
            memoryBus.write(getHL(), H);
            PC++;
            break;
        case 0x75: // LD [HL],L
            memoryBus.write(getHL(), L);
            PC++;
            break;
        case 0x76: // HALT
            // TODO: implement halting
            PC++;
            break;
        case 0x77: // LD [HL],A
            memoryBus.write(getHL(), A);
            PC++;
            break;
        case 0x78: // LD A,B
            A = B;
            PC++;
            break;
        case 0x79: // LD A,C
            A = C;
            PC++;
            break;
        case 0x7A: // LD A,D
            A = D;
            PC++;
            break;
        case 0x7B: // LD A,E
            A = E;
            PC++;
            break;
        case 0x7C: // LD A,H
            A = H;
            PC++;
            break;
        case 0x7D: // LD A,L
            A = L;
            PC++;
            break;
        case 0x7E: // LD A,[HL]
            A = memoryBus.read(getHL());
            PC++;
            break;
        case 0x7F: // LD A,A
            A = A;
            PC++;
            break;
        case 0x80: // ADD A,B
            add8(A, B);
            PC++;
            break;
        case 0x81: // ADD A,C
            add8(A, C);
            PC++;
            break;
        case 0x82: // ADD A,D
            add8(A, D);
            PC++;
            break;
        case 0x83: // ADD A,E
            add8(A, E);
            PC++;
            break;
        case 0x84: // ADD A,H
            add8(A, H);
            PC++;
            break;
        case 0x85: // ADD A,L
            add8(A, L);
            PC++;
            break;
        case 0x86: { // ADD A,[HL]
            uint8_t value = memoryBus.read(getHL());
            add8(A, value);
            PC++;
            break;
        }
        case 0x87: // ADD A,A
            add8(A, A);
            PC++;
            break;
        case 0x88: // ADC A,B
            adc(A, B);
            PC++;
            break;
        case 0x89: // ADC A,C
            adc(A, C);
            PC++;
            break;
        case 0x8A: // ADC A,D
            adc(A, D);
            PC++;
            break;
        case 0x8B: // ADC A,E
            adc(A, E);
            PC++;
            break;
        case 0x8C: // ADC A,H
            adc(A, H);
            PC++;
            break;
        case 0x8D: // ADC A,L
            adc(A, L);
            PC++;
            break;
        case 0x8E: { // ADC A,[HL]
            uint8_t value = memoryBus.read(getHL());
            adc(A, value);
            PC++;
            break;
        }
        case 0x8F: // ADC A,A
            adc(A, A);
            PC++;
            break;
        case 0x90: // SUB A,B
            sub(A, B);
            PC++;
            break;
        case 0x91: // SUB A,C
            sub(A, C);
            PC++;
            break;
        case 0x92: // SUB A,D
            sub(A, D);
            PC++;
            break;
        case 0x93: // SUB A,E
            sub(A, E);
            PC++;
            break;
        case 0x94: // SUB A,H
            sub(A, H);
            PC++;
            break;
        case 0x95: // SUB A,L
            sub(A, L);
            PC++;
            break;
        case 0x96: { // SUB A,[HL]
            uint8_t value = memoryBus.read(getHL());
            sub(A, value);
            PC++;
            break;
        }
        case 0x97: // SUB A,A
            sub(A, A);
            PC++;
            break;
        case 0x98: // SBC A,B
            sbc(A, B);
            PC++;
            break;
        case 0x99: // SBC A,C
            sbc(A, C);
            PC++;
            break;
        case 0x9A: // SBC A,D
            sbc(A, D);
            PC++;
            break;
        case 0x9B: // SBC A,E
            sbc(A, E);
            PC++;
            break;
        case 0x9C: // SBC A,H
            sbc(A, H);
            PC++;
            break;
        case 0x9D: // SBC A,L
            sbc(A, L);
            PC++;
            break;
        case 0x9E: { // SBC A,[HL]
            uint8_t value = memoryBus.read(getHL());
            sbc(A, value);
            PC++;
            break;
        }
        case 0x9F: // SBC A,A
            sbc(A, A);
            PC++;
            break;
        case 0xA0: // AND A,B
            AND(A, B);
            PC++;
            break;
        case 0xA1: // AND A,C
            AND(A, C);
            PC++;
            break;
        case 0xA2: // AND A,D
            AND(A, D);
            PC++;
            break;
        case 0xA3: // AND A,E
            AND(A, E);
            PC++;
            break;
        case 0xA4: // AND A,H
            AND(A, H);
            PC++;
            break;
        case 0xA5: // AND A,L
            AND(A, L);
            PC++;
            break;
        case 0xA6: { // AND A,[HL]
            uint8_t value = memoryBus.read(getHL());
            AND(A, value);
            PC++;
            break;
        }
        case 0xA7: // AND A,A
            AND(A, A);
            PC++;
            break;
        case 0xA8: // XOR A,B
            XOR(A, B);
            PC++;
            break;
        case 0xA9: // XOR A,C
            XOR(A, C);
            PC++;
            break;
        case 0xAA: // XOR A,D
            XOR(A, D);
            PC++;
            break;
        case 0xAB: // XOR A,E
            XOR(A, E);
            PC++;
            break;
        case 0xAC: // XOR A,H
            XOR(A, H);
            PC++;
            break;
        case 0xAD: // XOR A,L
            XOR(A, L);
            PC++;
            break;
        case 0xAE: { // XOR A,[HL]
            uint8_t value = memoryBus.read(getHL());
            XOR(A, value);
            PC++;
            break;
        }
        case 0xAF: // XOR A,A
            XOR(A, A);
            PC++;
            break;
        case 0xB0: // OR A,B
            OR(A, B);
            PC++;
            break;
        case 0xB1: // OR A,C
            OR(A, C);
            PC++;
            break;
        case 0xB2: // OR A,D
            OR(A, D);
            PC++;
            break;
        case 0xB3: // OR A,E
            OR(A, E);
            PC++;
            break;
        case 0xB4: // OR A,H
            OR(A, H);
            PC++;
            break;
        case 0xB5: // OR A,L
            OR(A, L);
            PC++;
            break;
        case 0xB6: { // OR A,[HL]
            uint8_t value = memoryBus.read(getHL());
            OR(A, value);
            PC++;
            break;
        }
        case 0xB7: // OR A,A
            OR(A, A);
            PC++;
            break;
        case 0xB8: // CP A,B
            CP(A, B);
            PC++;
            break;
        case 0xB9: // CP A,C
            CP(A, C);
            PC++;
            break;
        case 0xBA: // CP A,D
            CP(A, D);
            PC++;
            break;
        case 0xBB: // CP A,E
            CP(A, E);
            PC++;
            break;
        case 0xBC: // CP A,H
            CP(A, H);
            PC++;
            break;
        case 0xBD: // CP A,L
            CP(A, L);
            PC++;
            break;
        case 0xBE: { // CP A,[HL]
            uint8_t value = memoryBus.read(getHL());
            CP(A, value);
            PC++;
            break;
        }
        case 0xBF: // CP A,A
            CP(A, A);
            PC++;
            break;
        case 0xC0: { // RET NZ
            if (!getflag_Z()) {
                uint8_t lo = memoryBus.read(SP);
                SP++;
                uint8_t hi = memoryBus.read(SP);
                SP++;
                PC = (hi << 8) | lo;
            } else {
                PC++;
            }
            break;
        }
        case 0xC1: { // POP BC
            uint8_t lo = memoryBus.read(SP);
            SP++;
            uint8_t hi = memoryBus.read(SP);
            SP++;
            setBC((hi << 8) | lo);
            PC++;
            break;
        }
        case 0xC2: { // JP NZ n16
            PC++;
            uint8_t lo = memoryBus.read(PC);
            PC++;
            uint8_t hi = memoryBus.read(PC);
            uint16_t address = hi << 8 | lo;
            if (!getflag_Z()) {
                PC = address;
            } else {
                PC++;
            }
            break;
        }
        case 0xC3: { // JP n16
            PC++;
            uint8_t lo = memoryBus.read(PC);
            PC++;
            uint8_t hi = memoryBus.read(PC);
            uint16_t address = hi << 8 | lo;
            PC = address;
            break;
        }
        case 0xC4: { // CALL NZ n16
            PC++;
            uint8_t lo = memoryBus.read(PC);
            PC++;
            uint8_t hi = memoryBus.read(PC);
            uint16_t address = (hi << 8) | lo;
            if (!getflag_Z()) {
                uint16_t return_address = PC + 1;
                SP--;
                memoryBus.write(SP, return_address >> 8);
                SP--;
                memoryBus.write(SP, return_address & 0xFF);
                PC = address;
            } else {
                PC++;
            }
            break;
        }
        case 0xC5: // PUSH BC
            SP--;
            memoryBus.write(SP, getBC() >> 8);
            SP--;
            memoryBus.write(SP, getBC() & 0xFF);
            PC++;
            break;
        case 0xC6: { // ADD A,n8
            PC++;
            uint8_t value = memoryBus.read(PC);
            add8(A,value);
            PC++;
            break;
        }
        case 0xC7: { // RST $00
            uint16_t return_address = PC + 1;
            SP--;
            memoryBus.write(SP,return_address >> 8);
            SP--;
            memoryBus.write(SP, return_address & 0xFF);
            PC = 0;
            break;
        }
        case 0xC8: { // RET Z
            if (getflag_Z()) {
                uint8_t lo = memoryBus.read(SP);
                SP++;
                uint8_t hi = memoryBus.read(SP);
                SP++;
                PC = (hi << 8) | lo;
            } else {
                PC++;
            }
            break;
        }
        case 0xC9: { // RET
            uint8_t lo = memoryBus.read(SP);
            SP++;
            uint8_t hi = memoryBus.read(SP);
            SP++;
            PC = (hi << 8) | lo;
            break;
        }
        case 0xCA: { // JP Z n16
            PC++;
            uint8_t lo = memoryBus.read(PC);
            PC++;
            uint8_t hi = memoryBus.read(PC);
            uint16_t address = (hi << 8) | lo;
            if (getflag_Z()) {
                PC = address;
            } else {
                PC++;
            }
            break;
        }
        case 0xCB: // PREFIX
            PC++;
            exec_CB(memoryBus.read(PC));
            PC++;
            break;
        case 0xCC: { // CALL Z n16
            PC++;
            uint8_t lo = memoryBus.read(PC);
            PC++;
            uint8_t hi = memoryBus.read(PC);
            uint16_t address = (hi << 8) | lo;
            if (getflag_Z()) {
                uint16_t return_address = PC + 1;
                SP--;
                memoryBus.write(SP, return_address >> 8);
                SP--;
                memoryBus.write(SP, return_address & 0xFF);
                PC = address;
            } else {
                PC++;
            }
            break;
        }
        case 0xCD: { // CALL n16
            PC++;
            uint8_t lo = memoryBus.read(PC);
            PC++;
            uint8_t hi = memoryBus.read(PC);
            uint16_t address = (hi << 8) | lo;
            uint16_t return_address = PC + 1;
            SP--;
            memoryBus.write(SP, return_address >> 8);
            SP--;
            memoryBus.write(SP, return_address & 0xFF);
            PC = address;
            break;
        }
        case 0xCE: { // ADC A,n8
            PC++;
            uint8_t value = memoryBus.read(PC);
            adc(A, value);
            PC++;
            break;
        }
        case 0xCF: { // RST $08
            uint16_t return_address = PC + 1;
            SP--;
            memoryBus.write(SP,return_address >> 8);
            SP--;
            memoryBus.write(SP, return_address & 0xFF);
            PC = 8;
            break;
        }
        case 0xD0: { // RET NC
            if (!getflag_C()) {
                uint8_t lo = memoryBus.read(SP);
                SP++;
                uint8_t hi = memoryBus.read(SP);
                SP++;
                PC = (hi << 8) | lo;
            } else {
                PC++;
            }
            break;
        }
        case 0xD1: { // POP DE
            uint8_t lo = memoryBus.read(SP);
            SP++;
            uint8_t hi = memoryBus.read(SP);
            SP++;
            setDE((hi << 8) | lo);
            PC++;
            break;
        }
        case 0xD2: { // JP NC n16
            PC++;
            uint8_t lo = memoryBus.read(PC);
            PC++;
            uint8_t hi = memoryBus.read(PC);
            uint16_t address = (hi << 8) | lo;
            if (!getflag_C()) {
                PC = address;
            } else {
                PC++;
            }
            break;
        }
        case 0xD3: { // no operation listed
            PC++;
            break;
        }
        case 0xD4: { // CALL NC n16
            PC++;
            uint8_t lo = memoryBus.read(PC);
            PC++;
            uint8_t hi = memoryBus.read(PC);
            uint16_t address = (hi << 8) | lo;
            if (!getflag_C()) {
                uint16_t return_address = PC + 1;
                SP--;
                memoryBus.write(SP, return_address >> 8);
                SP--;
                memoryBus.write(SP, return_address & 0xFF);
                PC = address;
            } else {
                PC++;
            }
            break;
        }
        case 0xD5: // PUSH DE
            SP--;
            memoryBus.write(SP, getDE() >> 8);
            SP--;
            memoryBus.write(SP, getDE() & 0xFF);
            PC++;
            break;
        case 0xD6: { // SUB A,n8
            PC++;
            uint8_t value = memoryBus.read(PC);
            sub(A,value);
            PC++;
            break;
        }
        case 0xD7: { // RST $10
            uint16_t return_address = PC + 1;
            SP--;
            memoryBus.write(SP,return_address >> 8);
            SP--;
            memoryBus.write(SP, return_address & 0xFF);
            PC = 16;
            break;
        }
        case 0xD8: { // RET C
            if (getflag_C()) {
                uint8_t lo = memoryBus.read(SP);
                SP++;
                uint8_t hi = memoryBus.read(SP);
                SP++;
                PC = (hi << 8) | lo;
            } else {
                PC++;
            }
            break;
        }
        case 0xD9: // RETI
            // TODO: implement interrupts
            break;
        case 0xDA: { // JP C n16
            PC++;
            uint8_t lo = memoryBus.read(PC);
            PC++;
            uint8_t hi = memoryBus.read(PC);
            uint16_t address = (hi << 8) | lo;
            if (getflag_C()) {
                PC = address;
            } else {
                PC++;
            }
            break;
        }
        case 0xDB: // no operation listed
            PC++;
            break;
        case 0xDC: { // CALL C n16
            PC++;
            uint8_t lo = memoryBus.read(PC);
            PC++;
            uint8_t hi = memoryBus.read(PC);
            uint16_t address = (hi << 8) | lo;
            if (getflag_C()) {
                uint16_t return_address = PC + 1;
                SP--;
                memoryBus.write(SP, return_address >> 8);
                SP--;
                memoryBus.write(SP, return_address & 0xFF);
                PC = address;
            } else {
                PC++;
            }
            break;
        }
        case 0xDD: // no operation listed
            PC++;
            break;
        case 0xDE: { // SBC A,n8
            PC++;
            uint8_t value = memoryBus.read(PC);
            sbc(A, value);
            PC++;
            break;
        }
        case 0xDF: { // RST $18
            uint16_t return_address = PC + 1;
            SP--;
            memoryBus.write(SP,return_address >> 8);
            SP--;
            memoryBus.write(SP, return_address & 0xFF);
            PC = 24;
            break;
        }
        case 0xE0: { // LDH [n8],A
            PC++;
            uint8_t adj = memoryBus.read(PC);
            memoryBus.write(0xFF00 + adj, A);
            PC++;
            break;
        }
        case 0xE1: { // POP HL
            uint8_t lo = memoryBus.read(SP);
            SP++;
            uint8_t hi = memoryBus.read(SP);
            SP++;
            setHL((hi << 8) | lo);
            PC++;
            break;
        }
        case 0xE2: // LDH [C],A
            memoryBus.write(0xFF00 + C, A);
            PC++;
            break;
        case 0xE3: // no operation listed
            PC++;
            break;
        case 0xE4: // no operation listed
            PC++;
            break;
        case 0xE5: // PUSH HL
            SP--;
            memoryBus.write(SP, getHL() >> 8);
            SP--;
            memoryBus.write(SP, getHL() & 0xFF);
            PC++;
            break;
        case 0xE6: { // AND A,n8
            PC++;
            uint8_t value = memoryBus.read(PC);
            AND(A,value);
            PC++;
            break;
        }
        case 0xE7: { // RST $20
            uint16_t return_address = PC + 1;
            SP--;
            memoryBus.write(SP,return_address >> 8);
            SP--;
            memoryBus.write(SP, return_address & 0xFF);
            PC = 32;
            break;
        }
        case 0xE8: { // ADD SP,n8
            PC++;
            auto offset = static_cast<int8_t>(memoryBus.read(PC));
            setflag_Z(false);
            setflag_N(false);
            setflag_H((SP & 0x0F) + (offset & 0x0F) > 0x0F);
            setflag_C(static_cast<uint16_t>(SP & 0xFF) + static_cast<uint16_t>(offset & 0xFF) > 0x00FF);
            SP += offset;
            PC++;
            break;
        }
        case 0xE9: // JP HL
            PC = getHL();
            break;
        case 0xEA: { // LD [n16],A
            PC++;
            uint8_t lo = memoryBus.read(PC);
            PC++;
            uint8_t hi = memoryBus.read(PC);
            uint16_t address = (hi << 8) | lo;
            memoryBus.write(address, A);
            PC++;
            break;
        }
        case 0xEB: // no operation listed
            PC++;
            break;
        case 0xEC: // no operation listed
            PC++;
            break;
        case 0xED: // no operation listed
            PC++;
            break;
        case 0xEE: { // XOR A,n8
            PC++;
            uint8_t value = memoryBus.read(PC);
            XOR(A,value);
            PC++;
            break;
        }
        case 0xEF: { // RST $28
            uint16_t return_address = PC + 1;
            SP--;
            memoryBus.write(SP,return_address >> 8);
            SP--;
            memoryBus.write(SP, return_address & 0xFF);
            PC = 40;
            break;
        }
        case 0xF0: { // LDH A,[n8]
            PC++;
            uint8_t adj = memoryBus.read(PC);
            A = memoryBus.read(0xFF00 + adj);
            PC++;
            break;
        }
        case 0xF1: { // POP AF
            uint8_t lo = memoryBus.read(SP);
            SP++;
            uint8_t hi = memoryBus.read(SP);
            SP++;
            setAF((hi << 8) | lo);
            PC++;
            break;
        }
        case 0xF2: // LDH A,[C]
            A = memoryBus.read(0xFF00 + C);
            PC++;
            break;
        case 0xF3: { // DI
            // TODO: implement interrupts
            PC++;
            break;
        }
        case 0xF4: // no operation listed
            PC++;
            break;
        case 0xF5: // PUSH AF
            SP--;
            memoryBus.write(SP, getAF() >> 8);
            SP--;
            memoryBus.write(SP, getAF() & 0xFF);
            PC++;
            break;
        case 0xF6: { // OR A,n8
            PC++;
            uint8_t value = memoryBus.read(PC);
            OR(A,value);
            PC++;
            break;
        }
        case 0xF7: { // RST $30
            uint16_t return_address = PC + 1;
            SP--;
            memoryBus.write(SP,return_address >> 8);
            SP--;
            memoryBus.write(SP, return_address & 0xFF);
            PC = 48;
            break;
        }
        case 0xF8: {
            PC++;
            auto offset = static_cast<int8_t>(memoryBus.read(PC));
            setflag_Z(false);
            setflag_N(false);
            setflag_H((SP & 0x0F) + (offset & 0x0F) > 0x0F);
            setflag_C(static_cast<uint16_t>(SP & 0xFF) + static_cast<uint16_t>(offset & 0xFF) > 0x00FF);
            setHL(SP + offset);
            PC++;
            break;
        }
        case 0xF9: // LD SP,HL
            SP = getHL();
            PC++;
            break;
        case 0xFA: { // LD A,[n16]
            PC++;
            uint8_t lo = memoryBus.read(PC);
            PC++;
            uint8_t hi = memoryBus.read(PC);
            uint16_t address = (hi << 8) | lo;
            A = memoryBus.read(address);
            PC++;
            break;
        }
        case 0xFB: // EI
            // TODO: implement interrupts
            break;
        case 0xFC: // no operation listed
            PC++;
            break;
        case 0xFD: // no operation listed
            PC++;
            break;
        case 0xFE: { // CP A,n8
            PC++;
            uint8_t value = memoryBus.read(PC);
            CP(A, value);
            PC++;
            break;
        }
        case 0xFF: { // RST $38
            uint16_t return_address = PC + 1;
            SP--;
            memoryBus.write(SP,return_address >> 8);
            SP--;
            memoryBus.write(SP, return_address & 0xFF);
            PC = 56;
            break;
        }
        default: // unknown opcode
            break;
    }
}