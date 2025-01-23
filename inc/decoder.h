#pragma once
#include <systemc.h>
#include "common.h"
#include "interface/ahb_decoder_if.h"
#include "log.h"

// simulate a simple decoder
class Decoder : public AHBDecoderInterface {
  public:
    Decoder();
    sc_uint<SLAVE_COUNT> AddrDecode(sc_uint<32> addr) override;
};
