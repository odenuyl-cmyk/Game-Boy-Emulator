#include "Debugger.h"
#include <iostream>
#include <iomanip>

void Debugger::logOperation(uint16_t pc, uint8_t opcode) {
    std::cout << "PC: 0x"
              << std::hex << std::setw(4) << std::setfill('0') << pc
              << " Opcode: 0x"
              << std::setw(2) << static_cast<int>(opcode)
              << '\n';
}

void Debugger::dumpCPU(const CPU& cpu)
{
    std::cout << std::hex << std::setfill('0');

    std::cout
        << "A:"  << "0x" << std::setw(2) << static_cast<int>(cpu.getA()) << "  "
        << "B:"  << "0x" << std::setw(2) << static_cast<int>(cpu.getB()) << "  "
        << "C:"  << "0x" << std::setw(2) << static_cast<int>(cpu.getC()) << "  "
        << "D:"  << "0x" << std::setw(2) << static_cast<int>(cpu.getD()) << '\n';

    std::cout
        << "E:"  << "0x" << std::setw(2) << static_cast<int>(cpu.getE()) << "  "
        << "F:"  << "0x" << std::setw(2) << static_cast<int>(cpu.getF()) << "  "
        << "H:"  << "0x" << std::setw(2) << static_cast<int>(cpu.getH()) << "  "
        << "L:"  << "0x" << std::setw(2) << static_cast<int>(cpu.getL()) << '\n';

    std::cout
        << "PC:" << "0x" << std::setw(4) << cpu.getPC() << "  "
        << "SP:" << "0x" << std::setw(4) << cpu.getSP() << '\n';
}
