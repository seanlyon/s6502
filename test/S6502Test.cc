#include <gtest/gtest.h>
#include "S6502.hh"

class S6502Test : public testing::Test {
    protected:
    Bus bus;
    S6502 cpu;

    S6502Test() : cpu(bus) {}

    void SetUp() override {
        cpu.reset();
    }

    uint8_t getDataBus() const { return cpu.dataBus; }
    uint16_t getAddrBus() const { return cpu.addrBus; }

    uint8_t getProcessStatus() const { return cpu.processStatus; }
    uint8_t getXIndex() const { return cpu.xIndex; }
    uint8_t getYIndex() const { return cpu.yIndex; }
    uint8_t getStackPointer() const { return cpu.stackPointer; }
    uint8_t getAccumulator() const { return cpu.accumulator; }
    uint16_t getProgramCounter() const { return cpu.programCounter; }

    uint8_t getOpcode() const { return cpu.opcode; }
};

TEST_F(S6502Test, InitialState) {
    EXPECT_EQ(getDataBus(), 0);
    EXPECT_EQ(getAddrBus(), 0);
    EXPECT_EQ(getProcessStatus(), 32);
    EXPECT_EQ(getXIndex(), 0);
    EXPECT_EQ(getYIndex(), 0);
    EXPECT_EQ(getStackPointer(), 0);
    EXPECT_EQ(getAccumulator(), 0);
    EXPECT_EQ(getProgramCounter(), 0);
    EXPECT_EQ(getOpcode(), 0);
}
