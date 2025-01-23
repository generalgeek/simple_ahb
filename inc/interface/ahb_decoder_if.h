#pragma once
#include <systemc.h>

// AHB Decoder 接口层
class AHBDecoderInterface : public sc_interface {
  public:
    virtual sc_uint<SLAVE_COUNT> AddrDecode(sc_uint<32> addr) = 0;
};
