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
    void Read(sc_uint<BW> addr, sc_uint<BW>& data) override;
    void Write(sc_uint<BW> addr, sc_uint<BW> data) override;

  private:
    DEVICE_ID id_;
    uint32_t cache_[CACHE_SIZE]; // 1KB L1缓存,访问无延迟
    uint32_t cache_ptr_;         // cache指针
    size_t trans_beat_size_;
};
