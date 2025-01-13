#include <iostream>
#include "S6502.hh"

S6502::S6502(Bus& bus)
    : bus(bus), dataBus(0), addrBus(0), processStatus(32), xIndex(0), yIndex(0), stackPointer(0), accumulator(0), programCounter(0), opcode(0)
{
    opcodes = {
        {0xA9, {"LDA", &S6502::IMM, &S6502::LDA, 2}},
        {0xA5, {"LDA", &S6502::ZP, &S6502::LDA, 3}},
        {0xB5, {"LDA", &S6502::ZPX, &S6502::LDA, 4}},
        {0xAD, {"LDA", &S6502::ABS, &S6502::LDA, 4}},
        {0xBD, {"LDA", &S6502::ABSX, &S6502::LDA, 4}},
        {0xB9, {"LDA", &S6502::ABSY, &S6502::LDA, 4}},
        {0xA1, {"LDA", &S6502::INDX, &S6502::LDA, 6}},
        {0xB1, {"LDA", &S6502::INDY, &S6502::LDA, 5}}
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
    return 0;
}

uint8_t S6502::ZP()
{
    addrBus = bus.read(programCounter++);
    return 0;
}

uint8_t S6502::ZPX()
{
    addrBus = (bus.read(programCounter++) + xIndex) & 0x00FF;
    return 0;
}

uint8_t S6502::ZPY()
{
    addrBus = (bus.read(programCounter++) + yIndex) & 0x0FF;
    return 0;
}

uint8_t S6502::ABS()
{
    uint8_t lowByte = bus.read(programCounter++);
    uint8_t highByte = bus.read(programCounter++);
    addrBus = (highByte << 8) | lowByte;
    return 0;
}

uint8_t S6502::ABSX()
{
    uint8_t lowByte = bus.read(programCounter++);
    uint8_t highByte = bus.read(programCounter++);
    addrBus = ((highByte << 8) | lowByte) + xIndex;
    // todo: return 1 if page boundary crossed
    return 0;
}

uint8_t S6502::ABSY()
{
    uint8_t lowByte = bus.read(programCounter++);
    uint8_t highByte = bus.read(programCounter++);
    addrBus = ((highByte << 8) | lowByte) + yIndex;
    // todo: return 1 if page boundary crossed
    return 0;
}

uint8_t S6502::INDX()
{
    uint16_t addr = (bus.read(programCounter++) + xIndex) & 0x00FF;
    uint8_t lowByte = bus.read(addr++);
    uint8_t highByte = bus.read(addr);
    addrBus = (highByte << 8) | lowByte;
    return 0;
}

uint8_t S6502::INDY()
{
    uint16_t zpAddr = bus.read(programCounter++);
    uint16_t sum = bus.read(zpAddr++) + yIndex;
    uint8_t lowByte = sum & 0x00FF;
    uint8_t highByte = bus.read(zpAddr) + (sum & 0xFF00);
    addrBus = (highByte << 8) | lowByte;
    // todo: return 1 if page boundary crossed
    return 0;
}

uint8_t S6502::IND()
{
    uint8_t lowByte = bus.read(programCounter++);
    uint8_t highByte = bus.read(programCounter++);
    addrBus = (highByte << 8) | lowByte;
    return 0;
}

uint8_t S6502::ACC()
{
    return 0;
}

uint8_t S6502::IMP()
{
    return 0;
}

uint8_t S6502::REL()
{
    addrBus = programCounter++;
    return 0;
}

//
// Operations
//

uint8_t S6502::LDA()
{
    dataBus = bus.read(addrBus);
    accumulator = dataBus;
    setFlag(Zero, accumulator == 0x00);
    setFlag(Negative, accumulator & 0x80);
    return 0;
}
