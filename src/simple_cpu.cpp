#include "simple_cpu.h"

SimpleCPU::SimpleCPU(DEVICE_ID id) : AHBMasterInterface(id) {
}

bool SimpleCPU::MasterRead(sc_uint<BW> addr, sc_uint<BW>& data, sc_uint<BW> size) {
    if (!AddrValidCheck(addr, size))
        return false;
    int offset = addr / 4;
    int bit_s = (addr % 4) * 8;
    int bit_end = bit_s + size * 8 - 1;
    data = cache_[offset].range(bit_end, bit_s);
    LOG_DEBUG(logger, "{}  MasterRead(addr={:#010x}, data=[{}], size={})", sc_time_stamp().to_string(), addr.to_uint(),
              data.to_uint(), size.to_uint());
    return true;
}

bool SimpleCPU::MasterWrite(sc_uint<BW> addr, sc_uint<BW> data, sc_uint<BW> size) {
    if (!AddrValidCheck(addr, size))
        return false;
    int offset = addr / 4;
    int bit_s = (addr % 4) * 8;
    int bit_end = bit_s + size * 8 - 1;
    cache_[offset].range(bit_end, bit_s) = data.range(size * 8 - 1, 0);
    LOG_DEBUG(logger, "{}  MasterWrite(addr={:#010x}, data=[{}], size={})", sc_time_stamp().to_string(), addr.to_uint(),
              data.to_uint(), size.to_uint());
    return true;
}

MasterTask SimpleCPU::GetTask() {
    static sc_uint<BW> inner_addr = 0;
    static sc_uint<BW> slave_addr = RAM_ADDR_START;
    // 模拟发起读任务,可以任意自定义
    MasterTask task;
    task.write = false;                   // 读
    task.burst_type = BURST_TYPE::INCR4;  // BURST-INCR4类型
    task.trans_size = TRANS_SIZE::Word;   // 32bit
    task.slave_addr = slave_addr;         // 操作Slave的地址
    task.inner_addr = inner_addr;         // 操作内部cache的地址
    slave_addr += (1 << task.trans_size); // 注意地址可能超出范围
    slave_addr += (1 << task.trans_size); // 注意地址可能超出范围
    return task;
}

inline bool SimpleCPU::AddrValidCheck(sc_uint<BW> addr, sc_uint<BW> size) {
    sc_uint<BW> offset = addr / 4;
    if (addr / 4 > CACHE_SIZE) { // 传输地址不能超出范围
        LOG_ERROR(logger, "{}  the addr={:#010x} is out of the Cache range [0, {}].", sc_time_stamp().to_string(),
                  offset.to_uint(), CACHE_SIZE);
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
