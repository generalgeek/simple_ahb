#pragma once
#include <systemc.h>
#include "common.h"
#include "interface/ahb_decoder_if.h"
#include "log.h"

class AHBDecoderChannel : public sc_prim_channel, public AHBDecoderInterface {
  public:
    // Input ports
    sc_in<sc_uint<32>> HADDR; // 输入地址信号

    // Output ports
    sc_out<sc_uint<SLAVE_COUNT>> HSELx; // slave选择信号,每个bit位可以控制一个slave
  public:
    virtual void Read(sc_uint<32>& data) override;
    virtual void Write(sc_uint<SLAVE_COUNT> data) override;
};
