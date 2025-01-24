#include "ram_onchip.h"

RAMOnChip::RAMOnChip(DEVICE_ID id) : id_(id) {
    memset(ram_, 0, RAM_SIZE * sizeof(uint32_t));
}

bool RAMOnChip::Read(sc_uint<BW> addr, sc_uint<BW>& data, sc_uint<BW> size) {
    if (!AddrValidCheck(addr, size))
        return false;
    sc_uint<BW> offset = addr / 4;      // 计算 4 字节对齐的基址索引
    sc_uint<BW> byte_offset = addr % 4; // 计算字节内的偏移(0 ~ 3)
    if (size == 1) {
        data = (ram_[offset] >> (byte_offset * 8)) & 0xFF;
    } else if (size == 2) {
        data = (ram_[offset] >> (byte_offset * 8)) & 0xFFFF;
    } else if (size == 4) {
        data = ram_[offset];
    } else {
        LOG_ERROR(logger, "Not support trans_size=.", size.to_uint());
        return false;
    }
    return true;
}

bool RAMOnChip::Write(sc_uint<BW> addr, sc_uint<BW> data, sc_uint<BW> size) {
    if (!AddrValidCheck(addr, size))
        return false;
    sc_uint<BW> offset = addr / 4;      // 计算 4 字节对齐的基址索引
    sc_uint<BW> byte_offset = addr % 4; // 计算字节内的偏移(0 ~ 3)
    if (size == 1) {
        sc_uint<32> mask = 0xFF << (byte_offset * 8); // 生成目标字节的掩码
        ram_[offset] = (ram_[offset] & ~mask) | ((data & 0xFF) << (byte_offset * 8));
    } else if (size == 2) {
        sc_uint<32> mask = 0xFFFF << (byte_offset * 8); // 生成目标半字的掩码
        ram_[offset] = (ram_[offset] & ~mask) | ((data & 0xFFFF) << (byte_offset * 8));
    } else if (size == 4) {
        ram_[offset] = data.to_uint();
    } else {
        LOG_ERROR(logger, "Not support trans_size=.", size.to_uint());
        return false;
    }
    return true;
}

inline bool RAMOnChip::AddrValidCheck(sc_uint<BW> addr, sc_uint<BW> size) {
    sc_uint<BW> offset = addr / 4;
    if (addr / 4 > RAM_SIZE) { // 传输地址不能超出范围
        LOG_ERROR(logger, "The addr={:#010x} is out of the RAM range [0, {}].", offset.to_uint(), RAM_SIZE);
        return false;
    }
    if (addr % size) { // 传输地址要与HSIZE对齐
        LOG_ERROR(logger, "The addr={:#010x} not align with size={}.", addr.to_uint(), size.to_uint());
        return false;
    }
    return true;
}
