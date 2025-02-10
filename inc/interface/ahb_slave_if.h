#pragma once
#include <systemc.h>
#include "common.h"

// AHB Slave 接口层
class AHBSlaveInterface : public sc_interface {
  public:
    AHBSlaveInterface(DEVICE_ID id, sc_time resp_time) : id_(id), resp_time_(resp_time) {}
    virtual bool SlaveRead(sc_uint<BW> addr, sc_uint<BW>& data, sc_uint<BW> size) = 0;
    virtual bool SlaveWrite(sc_uint<BW> addr, sc_uint<BW> data, sc_uint<BW> size) = 0;
    virtual sc_time GetRespT() { return resp_time_; }

  private:
    DEVICE_ID id_;
    sc_time resp_time_;
};
