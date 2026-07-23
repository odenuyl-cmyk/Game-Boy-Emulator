#pragma once
#include "../memory/Bus.h"


class CPU {
public:
    explicit CPU(Bus& memoryBus);

    void step();

private:
    Bus& memoryBus;
};
