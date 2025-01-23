#pragma once
#include <systemc.h>

// AHB Slave 接口层
class AHBSlaveInterface : public sc_interface {
  public:
    virtual void Read(sc_uint<32> addr, sc_uint<32>& data) = 0;
    virtual void Write(sc_uint<32> addr, sc_uint<32> data) = 0;
};
