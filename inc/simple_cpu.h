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
    bool Read(sc_uint<BW> addr, sc_uint<BW>& data, sc_uint<BW> size) override;
    bool Write(sc_uint<BW> addr, sc_uint<BW> data, sc_uint<BW> size) override;

  private:
    DEVICE_ID id_;
    sc_uint<BW> cache_[CACHE_SIZE]; // 1KB
};
