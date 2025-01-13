#ifndef S6502_HH
#define S6502_HH

#include <functional>
#include <unordered_map>
#include <string>
#include "Bus.hh"

class S6502
{
public:
    S6502(Bus& bus);
    void reset();
    void clock();
    void log();

    enum StatusFlag
    {
        Carry = (1 << 0),
        Zero = (1 << 1),
        IRQ = (1 << 2),
        Decimal = (1 << 3),
        BRK = (1 << 4),
        Unused = (1 << 5),
        Overflow = (1 << 6),
        Negative = (1 << 7)
    };

private:
    void fetch();
    void decode();
    void execute();

    Bus bus;
    uint8_t dataBus;
    uint16_t addrBus;

    void setFlag(StatusFlag flag, bool condition);

    uint8_t processStatus;
    uint8_t xIndex;
    uint8_t yIndex;
    uint8_t stackPointer;
    uint8_t accumulator;
    uint16_t programCounter;

    struct Instruction
    {   
        std::string mnemonic;
        std::function<uint8_t(S6502&)> addrMode;
        std::function<uint8_t(S6502&)> operation;
        int cycles;
    };

    uint8_t opcode;
    Instruction instruction;
    std::unordered_map<uint8_t, Instruction> opcodes;

//
// Addressing Modes
//

    uint8_t IMM();
    uint8_t ZP();
    uint8_t ZPX();
    uint8_t ZPY();
    uint8_t ABS();
    uint8_t ABSX();
    uint8_t ABSY();
    uint8_t INDX();

//
// Operations
//
    uint8_t LDA();

    friend class S6502Test;
};

#endif
