/* #pragma once
#include <cstring>
#include <systemc.h>
#include "interface/ahb_master_if.h"
#include "log.h"

constexpr size_t CACHE_SIZE = 256; // 256*4=1KB

// simulate a simple cpu/pe
class SimpleCPU : public AHBMasterIF {
  public:
    SimpleCPU(sc_module_name name, DEVICE_ID master_id) : AHBMasterIF(name, master_id) {
        std::memset(cache_, 0, sizeof(uint32_t) * CACHE_SIZE);
        cache_ptr_ = 0;
        trans_beat_size_ = 0;
    }

  private:
    void Process() override;
    void Reset() override;
    void WriteOut() override;
    void ReadIn() override;
    void Prepare();

  private:
    uint32_t cache_[CACHE_SIZE]; // 1KB L1缓存,访问无延迟
    uint32_t cache_ptr_; // cache指针
    size_t trans_beat_size_;
}; */
