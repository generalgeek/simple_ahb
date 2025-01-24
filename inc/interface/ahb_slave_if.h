#pragma once
#include <systemc.h>

// AHB Slave 接口层
class AHBSlaveInterface : public sc_interface {
  public:
    virtual bool Read(sc_uint<BW> addr, sc_uint<BW>& data, sc_uint<BW> size) = 0;
    virtual bool Write(sc_uint<BW> addr, sc_uint<BW> data, sc_uint<BW> size) = 0;
};
