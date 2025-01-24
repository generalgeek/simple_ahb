#pragma once
#include <systemc.h>

class AHBMasterInterface : public sc_interface {
  public:
    virtual void Read(sc_uint<BW> addr, sc_uint<BW>& data) = 0;
    virtual void Write(sc_uint<BW> addr, sc_uint<BW> data) = 0;
};
