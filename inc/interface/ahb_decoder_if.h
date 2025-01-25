#pragma once
#include <systemc.h>
#include "common.h"

// AHB Decoder 接口层
class AHBDecoderInterface : public sc_interface {
  public:
    virtual sc_uint<SLAVE_COUNT> AddrDecode(sc_uint<BW> addr) = 0;
};
