/* #pragma once
#include <systemc.h>
#include "interface/ahb_arbiter_if.h"

// simulate a simple arbiter
class AHBArbiter : public AHBArbiterIF {
  public:
    AHBArbiter(sc_module_name name) : AHBArbiterIF(name) {}

  private:
    void Process() override;
    void Reset() override;
};
 */