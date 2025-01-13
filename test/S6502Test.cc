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

    void loadMemory(const std::vector<uint8_t>& data, uint16_t startAddr = 0x0000) {
        for (size_t i = 0; i < data.size(); ++i) {
            cpu.bus.write(startAddr + i, data.at(i));
        }
    }
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

TEST_F(S6502Test, LdaImmediate) {
    loadMemory({0xA9, 0x00});
    cpu.clock();

    EXPECT_EQ(getAccumulator(), 0);
    EXPECT_EQ(getProgramCounter(), 2);

    EXPECT_TRUE(getProcessStatus() & S6502::Zero);
    EXPECT_FALSE(getProcessStatus() & S6502::Negative);
}

TEST_F(S6502Test, LdaZeroPage) {
    loadMemory({0xA5, 0x04, 0x00, 0x00, 0x13});
    cpu.clock();

    EXPECT_EQ(getAccumulator(), 0x13);
    EXPECT_EQ(getProgramCounter(), 2);

    EXPECT_FALSE(getProcessStatus() & S6502::Zero);
    EXPECT_FALSE(getProcessStatus() & S6502::Negative);
}

TEST_F(S6502Test, LdaZeroPageX) {
    loadMemory({0xB5, 0x04, 0x00, 0x00, 0x13});
    cpu.clock();

    EXPECT_EQ(getAccumulator(), 0x13);
    EXPECT_EQ(getProgramCounter(), 2);

    EXPECT_FALSE(getProcessStatus() & S6502::Zero);
    EXPECT_FALSE(getProcessStatus() & S6502::Negative);
}

TEST_F(S6502Test, LdaAbsolute) {
    loadMemory({0xAD, 0x04, 0x00, 0x00, 0x13});
    cpu.clock();

    EXPECT_EQ(getAccumulator(), 0x13);
    EXPECT_EQ(getProgramCounter(), 3);

    EXPECT_FALSE(getProcessStatus() & S6502::Zero);
    EXPECT_FALSE(getProcessStatus() & S6502::Negative);
}

TEST_F(S6502Test, LdaAbsoluteX) {
    loadMemory({0xBD, 0x04, 0x00, 0x00, 0x13});
    cpu.clock();

    EXPECT_EQ(getAccumulator(), 0x13);
    EXPECT_EQ(getProgramCounter(), 3);

    EXPECT_FALSE(getProcessStatus() & S6502::Zero);
    EXPECT_FALSE(getProcessStatus() & S6502::Negative);
}

TEST_F(S6502Test, LdaAbsoluteY) {
    loadMemory({0xB9, 0x04, 0x00, 0x00, 0x13});
    cpu.clock();

    EXPECT_EQ(getAccumulator(), 0x13);
    EXPECT_EQ(getProgramCounter(), 3);

    EXPECT_FALSE(getProcessStatus() & S6502::Zero);
    EXPECT_FALSE(getProcessStatus() & S6502::Negative);
}

TEST_F(S6502Test, LdaIndexedIndirect) {
    loadMemory({0xA1, 0x04, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00 ,0x13});
    cpu.clock();

    EXPECT_EQ(getAccumulator(), 0x13);
    EXPECT_EQ(getProgramCounter(), 2);

    EXPECT_FALSE(getProcessStatus() & S6502::Zero);
    EXPECT_FALSE(getProcessStatus() & S6502::Negative);
}

TEST_F(S6502Test, LdaIndirectIndexed) {
    loadMemory({0xB1, 0x04, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00 ,0x13});
    cpu.clock();

    EXPECT_EQ(getAccumulator(), 0x13);
    EXPECT_EQ(getProgramCounter(), 2);

    EXPECT_FALSE(getProcessStatus() & S6502::Zero);
    EXPECT_FALSE(getProcessStatus() & S6502::Negative);
}
