#pragma once
#include <cstddef>
#include <cstring>
#include <systemc.h>
#include "common.h"
#include "interface/ahb_slave_if.h"
#include "log.h"

constexpr size_t RAM_SIZE = 4096; // 4096*4=16KB

// AHB Slave 功能层
class RAMOnChip : public AHBSlaveInterface {
  public:
    RAMOnChip(DEVICE_ID id);
    bool Read(sc_uint<BW> addr, sc_uint<BW>& data, sc_uint<BW> size) override;
    bool Write(sc_uint<BW> addr, sc_uint<BW> data, sc_uint<BW> size) override;
  private:
    bool AddrValidCheck(sc_uint<BW> addr, sc_uint<BW> size);

  private:
    DEVICE_ID id_;
    uint32_t ram_[RAM_SIZE]; // 16KB
};
