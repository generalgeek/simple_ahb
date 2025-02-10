#pragma once

#include <systemc.h>
#include "common.h"
#include "log.h"

class AHBDecoder : public sc_module {
  public:
    SC_HAS_PROCESS(AHBDecoder);
    AHBDecoder(const sc_module_name& name) : sc_module(name) {
        SC_METHOD(Process);
        sensitive << HCLK.pos()/*  << HADDR */;
        // dont_initialize();
    }

  public:
    // Input ports
    sc_in<bool> HCLK;         // 时钟信号
    sc_in<bool> HRESETn;      // 复位信号（低电平有效）
    sc_in<sc_uint<BW>> HADDR; // 输入地址信号
    // Output ports
    sc_out<bool> HSELx[SLAVE_CNT]; // slave选择信号,每个bit位可以控制一个slave

  private:
    void Process();
    void Reset();
    void ClearHSEL();
    DEVICE_ID AddrDecode(sc_uint<BW> addr);
};
