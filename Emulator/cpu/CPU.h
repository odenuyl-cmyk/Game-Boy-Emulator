#pragma once
#include "../memory/Bus.h"

class Debugger;

class CPU {
public:
    explicit CPU(Bus& memoryBus, Debugger& debugger);

    void step();
    void exec_CB(uint8_t operation);

    // default register functions
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

    // dual-register functions
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

    // flag-related functions
    void setflag_Z(bool value) {
        F = value ? (F | (1 << 7)) : (F & ~(1 << 7));
    }
    void setflag_N(bool value) {
        F = value ? (F | (1 << 6)) : (F & ~(1 << 6));
    }
    void setflag_H(bool value) {
        F = value ? (F | (1 << 5)) : (F & ~(1 << 5));
    }
    void setflag_C(bool value) {
        F = value ? (F | (1 << 4)) : (F & ~(1 << 4));
    }

    bool getflag_Z() const {
        return F & (1 << 7);
    }
    bool getflag_N() const {
        return F & (1 << 6);
    }
    bool getflag_H() const {
        return F & (1 << 5);
    }
    bool getflag_C() const {
        return F & (1 << 4);
    }

    // instruction functions
    void inc(uint8_t& reg);

    void dec(uint8_t& reg);

    void add8(uint8_t& reg1, uint8_t& reg2);

    uint16_t add16(uint16_t reg1, uint16_t reg2);

    void sub(uint8_t& reg1, uint8_t& reg2);

    void adc(uint8_t& reg1, uint8_t& reg2);

    void sbc(uint8_t& reg1, uint8_t& reg2);

    void AND(uint8_t& reg1, uint8_t& reg2);

    void XOR(uint8_t& reg1, uint8_t& reg2);

    void OR(uint8_t& reg1, uint8_t& reg2);

    void CP(uint8_t& reg1, uint8_t& reg2);

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