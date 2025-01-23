#pragma once
#include <systemc.h>

class AHBArbiterInterface : public sc_interface {
  public:
    virtual void Arbitrate() = 0;
};
