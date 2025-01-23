#pragma once
#include <systemc.h>

// AHB Decoder 接口层
class AHBDecoderInterface : public sc_interface {
  public:
    virtual void Read(sc_uint<32>& data) = 0;
    virtual void Write(sc_uint<SLAVE_COUNT> data) = 0;
};
