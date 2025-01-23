#pragma once
#include <cstddef>
#include <systemc.h>
#include "channel/ahb_slave_channel.h"
#include "interface/ahb_slave_if.h"
#include "log.h"

constexpr size_t RAM_SIZE = 4096; // 4096*4=16KB

// AHB Slave 模块层
class RAMOnChip final : public sc_module, public AHBSlaveInterface {
  public:
    // SC_HAS_PROCESS(RAMOnChip);
    RAMOnChip(sc_module_name name, DEVICE_ID slave_id) : sc_module(name) {}
    virtual void Read(sc_uint<32> addr, sc_uint<32>& data) override;
    virtual void Write(sc_uint<32> addr, sc_uint<32> data) override;

  private:
    uint32_t ram_[RAM_SIZE]; // 16KB
};
