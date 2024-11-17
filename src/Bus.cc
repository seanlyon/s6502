#include <algorithm>
#include "Bus.hh"

Bus::Bus()
{
    std::fill(memory.begin(), memory.end(), 0);
}

uint8_t Bus::read(uint16_t addr) const
{
    return memory.at(addr);
}

void Bus::write(uint16_t addr, uint8_t data)
{
    memory.at(addr) = data;
}
