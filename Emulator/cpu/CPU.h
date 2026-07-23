#pragma once
#include "../memory/Bus.h"


class CPU {
public:
    explicit CPU(Bus& memoryBus);

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

private:
    Bus& memoryBus;

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
