#pragma once
#include <systemc.h>
#include "channel/ahb_decoder_channel.h"
#include "common.h"
#include "interface/ahb_decoder_if.h"
#include "log.h"

// simulate a simple decoder
class Decoder : public sc_module {
  public:
    SC_HAS_PROCESS(Decoder);
    Decoder(sc_module_name name) : sc_module(name) {
        SC_METHOD(Process);
        sensitive << HCLK.pos(); // 只对上升沿敏感
        dont_initialize();
    }

  public:
    sc_in<bool> HCLK;
    sc_port<AHBDecoderInterface> port_;

  private:
    void Process();
    sc_uint<SLAVE_COUNT> AddrDecode(sc_uint<32> addr);
};
