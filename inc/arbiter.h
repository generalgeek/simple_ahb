#pragma once
#include <systemc.h>
#include "interface/ahb_arbiter_if.h"

class Arbiter : public AHBArbiterInterface {
  public:
    void Arbitrate() override;
};
