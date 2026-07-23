#include "CPU.h"
#include "../memory/Bus.h"

class CPU {
public:
    explicit CPU(Bus& bus);

    void step();

private:
    Bus& bus;
};
