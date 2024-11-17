#ifndef BUS_HH
#define BUS_HH

#include <array>
#include <cstdint>

class Bus {
    public:
        Bus();
        void write(uint16_t addr, uint8_t data);
        uint8_t read(uint16_t addr) const;

    private:
        std::array<uint8_t, 64 * 1024> memory;
};

#endif
