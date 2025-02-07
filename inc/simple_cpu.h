#pragma once
#include <cstring>
#include <systemc.h>
#include "common.h"
#include "interface/ahb_master_if.h"
#include "log.h"

constexpr size_t CACHE_SIZE = 256; // 256*4=1KB

// simulate a simple cpu/pe
class SimpleCPU : public AHBMasterInterface {
  public:
    SimpleCPU(DEVICE_ID id);
    bool MasterRead(sc_uint<BW> addr, sc_uint<BW>& data, sc_uint<BW> size) override;
    bool MasterWrite(sc_uint<BW> addr, sc_uint<BW> data, sc_uint<BW> size) override;
    MasterTask GetTask() override;

  private:
    bool AddrValidCheck(sc_uint<BW> addr, sc_uint<BW> size);

  private:
    DEVICE_ID id_;
    sc_uint<BW> cache_[CACHE_SIZE]; // 1KB
};
