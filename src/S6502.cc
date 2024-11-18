#include <iostream>
#include "S6502.hh"

S6502::S6502(Bus& bus)
    : bus(bus), dataBus(0), addrBus(0), processStatus(32), xIndex(0), yIndex(0), stackPointer(0), accumulator(0), programCounter(0), opcode(0)
{
    opcodes = {
        {0xA9, {"LDA", &S6502::IMM, &S6502::LDA, 2}},
        {0xA5, {"LDA", &S6502::ZP, &S6502::LDA, 3}},
    };
}

//
// Control Interface
//

void S6502::reset()
{
    dataBus = 0;
    addrBus = 0;
    processStatus = 32;
    xIndex = 0;
    yIndex = 0;
    stackPointer = 0;
    accumulator = 0;
    programCounter = 0;
    opcode = 0;
}

void S6502::clock()
{
    fetch();
    decode();
    execute();
    log();
}

void S6502::fetch()
{
    addrBus = programCounter++;
    opcode = bus.read(addrBus);
}

void S6502::decode()
{
    instruction = opcodes.at(opcode);
}

void S6502::execute()
{
    instruction.addrMode(*this);
    instruction.operation(*this);
}

void S6502::log()
{
    std::cout << "Opcode:\t\t" << std::hex << std::uppercase << static_cast<int>(opcode) << "\t\t"
              << "Mnemonic:\t  " << instruction.mnemonic << '\n'
              << "Data Bus:\t" << static_cast<int>(dataBus) << "\t\t"
              << "Address Bus:\t  " << static_cast<int>(addrBus) << '\n'
              << "Index X:\t" << static_cast<int>(xIndex) << "\t\t"
              << "Index Y:\t  " << static_cast<int>(yIndex) << '\n'
              << "Stack Pointer:\t" << static_cast<int>(stackPointer) << "\t\t"
              << "Program Counter:  " << static_cast<int>(programCounter) << '\n'
              << "Accumulator:\t" << static_cast<int>(accumulator) << "\t\t"
              << "Process Status:\t  " << std::bitset<8>(processStatus) << "\n\n";
}

//
// Process Status Mutator
//

void S6502::setFlag(StatusFlag flag, bool condition)
{
    if (condition)
        processStatus |= flag;
    else
        processStatus &= ~flag;
}

//
// Addressing Modes
//

uint8_t S6502::IMM()
{
    addrBus = programCounter++;
    dataBus = bus.read(addrBus);
    return 0;
}

uint8_t S6502::ZP()
{
    addrBus = bus.read(programCounter++);
    dataBus = bus.read(addrBus);
    return 0;
}

//
// Operations
//

uint8_t S6502::LDA()
{
    accumulator = dataBus;
    setFlag(Zero, accumulator == 0x00);
    setFlag(Negative, accumulator & 0x80);
    return 0;
}
