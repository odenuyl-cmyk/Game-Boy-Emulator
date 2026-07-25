#include "memory/Bus.h"
#include "cpu/CPU.h"
#include "debug/Debugger.h"

int main()
{
    Bus bus;
    Debugger debugger;
    CPU cpu(bus, debugger);

    bus.write(0x0000, 0x3E);
    bus.write(0x0001, 0x42);
    bus.write(0x0002, 0x16);
    bus.write(0x0003, 0x16);
    bus.write(0x0004, 0x00);
    bus.write(0x0005, 0x22);
    bus.write(0x0006, 0x2E);
    bus.write(0x0007, 0xE9);

    cpu.step();
    cpu.step();
    cpu.step();
    for (int i = 0; i < 14; i++) {
        cpu.step();
    }

    debugger.dumpCPU(cpu);

    return 0;
}