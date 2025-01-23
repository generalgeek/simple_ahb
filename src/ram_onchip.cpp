#include "ram_onchip.h"

RAMOnChip::RAMOnChip(DEVICE_ID id) : id_(id) {
    memset(ram_, 0, RAM_SIZE * sizeof(uint32_t));
}

void RAMOnChip::Read(sc_uint<32> addr, sc_uint<32>& data) {
    sc_uint<32> offset = addr / 4;
    if (addr >= RAM_SIZE) {
        LOG_ERROR(logger, "The addr={:#010x} is out of the RAM range [0, {}].", offset.to_uint(), RAM_SIZE);
        return;
    }
    data = ram_[offset];
}

void RAMOnChip::Write(sc_uint<32> addr, sc_uint<32> data) {
    sc_uint<32> offset = addr / 4;
    if (addr >= RAM_SIZE) {
        LOG_ERROR(logger, "The addr={:#010x} is out of the RAM range [0, {}].", offset.to_uint(), RAM_SIZE);
        return;
    }
    ram_[offset] = data.to_uint();
}
