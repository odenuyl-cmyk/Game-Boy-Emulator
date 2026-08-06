#pragma once
#include "../memory/Bus.h"

class Debugger;

class CPU {
public:
    explicit CPU(Bus& memoryBus, Debugger& debugger);

    void step();

    uint8_t getA() const {
        return A;
    }
    uint8_t getB() const {
        return B;
    }
    uint8_t getC() const {
        return C;
    }
    uint8_t getD() const {
        return D;
    }
    uint8_t getE() const {
        return E;
    }
    uint8_t getF() const {
        return F;
    }
    uint8_t getH() const {
        return H;
    }
    uint8_t getL() const {
        return L;
    }
    uint16_t getPC() const {
        return PC;
    }
    uint16_t getSP() const {
        return SP;
    }
    uint16_t getBC() const {
        return (B << 8) | C;
    }
    void setBC(uint16_t value) {
        B = value >> 8;
        C = value & 0xFF;
    }
    uint16_t getDE() const {
        return (D << 8) | E;
    }
    void setDE(uint16_t value) {
        D = value >> 8;
        E = value & 0xFF;
    }
    uint16_t getHL() const {
        return (H << 8) | L;
    }
    void setHL(uint16_t value) {
        H = value >> 8;
        L = value & 0xFF;
    }

private:
    Bus& memoryBus;
    Debugger& debugger;

    uint8_t A = 0;
    uint8_t B = 0;
    uint8_t C = 0;
    uint8_t D = 0;
    uint8_t E = 0;
    uint8_t F = 0;
    uint8_t H = 0;
    uint8_t L = 0;

    uint16_t PC = 0;
    uint16_t SP = 0;
};
