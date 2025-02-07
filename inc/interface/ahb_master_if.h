#pragma once
#include <systemc.h>
#include "common.h"

struct MasterTask {
    bool write;
    sc_uint<BW> inner_addr;
    sc_uint<BW> slave_addr;
    BURST_TYPE burst_type;
    TRANS_SIZE trans_size;
};

class AHBMasterInterface : public sc_interface {
  public:
    virtual bool MasterRead(sc_uint<BW> addr, sc_uint<BW>& data, sc_uint<BW> size) = 0;
    virtual bool MasterWrite(sc_uint<BW> addr, sc_uint<BW> data, sc_uint<BW> size) = 0;
    virtual MasterTask GetTask() = 0;
};
