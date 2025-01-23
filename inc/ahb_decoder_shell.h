#pragma once

#include <systemc.h>
#include "common.h"
#include "interface/ahb_decoder_if.h"

class AHBDecoderShell : public sc_module {
  public:
    SC_HAS_PROCESS(AHBDecoderShell);
    AHBDecoderShell(const sc_module_name& name) : sc_module(name) {
        SC_METHOD(Process);
        sensitive << HADDR;
        dont_initialize();
    }

  public:
    sc_port<AHBDecoderInterface> decoder_port_;
    // Input ports
    sc_in<sc_uint<32>> HADDR; // 输入地址信号
    // Output ports
    sc_out<sc_uint<SLAVE_COUNT>> HSELx; // slave选择信号,每个bit位可以控制一个slave
  private:
    void Process();
};
