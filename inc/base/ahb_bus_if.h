#pragma once
#include <systemc.h>

class AHBBusIF : public sc_module {
  public:
    virtual bool burst_read() = 0;
    virtual bool burst_write() = 0;
    virtual bool single_read() = 0;
    virtual bool single_write() = 0;
};
