#include "memory/Bus.h"
#include "cpu/CPU.h"
#include "debug/Debugger.h"

int main()
{
    Bus bus;
    CPU cpu(bus);
    Debugger debugger;

    bus.write(0x0000, 0x55);
    bus.write(0x0001, 0x32);
    bus.write(0x0002, 0x11);

    cpu.step();
    cpu.step();
    cpu.step();
    for (int i = 0; i < 14; i++) {
        cpu.step();
    }

    debugger.dumpCPU(cpu);

    return 0;
}