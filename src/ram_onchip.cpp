#include "ram_onchip.h"

RAMOnChip::RAMOnChip(DEVICE_ID id, sc_time resp_time) : AHBSlaveInterface(id, resp_time) {
    for (size_t i = 0; i < RAM_SIZE; i++) {
        ram_[i] = (i + 1) % BW; // 1~32
    }
}

bool RAMOnChip::SlaveRead(sc_uint<BW> addr, sc_uint<BW>& data, sc_uint<BW> size) {
    addr -= RAM_ADDR_START;
    if (!AddrValidCheck(addr, size))
        return false;
    int offset = addr / 4;
    int bit_s = (addr % 4) * 8;
    int bit_end = bit_s + size * 8 - 1;
    data = ram_[offset].range(bit_end, bit_s);
    LOG_DEBUG(logger, "{}  SlaveRead(addr={:#010x}, data=[{}], size={})", sc_time_stamp().to_string(), addr.to_uint(),
              data.to_uint(), size.to_uint());
    return true;
}

bool RAMOnChip::SlaveWrite(sc_uint<BW> addr, sc_uint<BW> data, sc_uint<BW> size) {
    addr -= RAM_ADDR_START;
    if (!AddrValidCheck(addr, size))
        return false;
    int offset = addr / 4;
    int bit_s = (addr % 4) * 8;
    int bit_end = bit_s + size * 8 - 1;
    ram_[offset].range(bit_end, bit_s) = data.range(size * 8 - 1, 0);
    LOG_DEBUG(logger, "{}  SlaveWrite(addr={:#010x}, data=[{}], size={})", sc_time_stamp().to_string(), addr.to_uint(),
              data.to_uint(), size.to_uint());
    return true;
}

inline bool RAMOnChip::AddrValidCheck(sc_uint<BW> addr, sc_uint<BW> size) {
    sc_uint<BW> offset = addr / 4;
    if (addr / 4 > RAM_SIZE) { // 传输地址不能超出范围
        LOG_ERROR(logger, "{}  the addr={:#010x} is out of the RAM range [0, {}].", sc_time_stamp().to_string(),
                  offset.to_uint(), RAM_SIZE);
        return false;
    }
    if (size != 1 && size != 2 && size != 4) { // 传输大小只能是1B 2B 4B
        LOG_ERROR(logger, "{}  not support trans_size=.", sc_time_stamp().to_string(), size.to_uint());
        return false;
    }
    if (addr % size) { // 传输地址要与HSIZE对齐
        LOG_ERROR(logger, "{}  the addr={:#010x} not align with size={}.", sc_time_stamp().to_string(), addr.to_uint(),
                  size.to_uint());
        return false;
    }
    return true;
}
