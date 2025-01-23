#pragma once
#include <systemc.h>

class AHBMasterInterface : public sc_interface {
  public:
    virtual void Read(sc_uint<32> addr, sc_uint<32>& data) = 0;
    virtual void Write(sc_uint<32> addr, sc_uint<32> data) = 0;
};
