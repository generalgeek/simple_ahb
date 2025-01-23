#pragma once
#include <systemc.h>
#include "base/ahb_slave_base.h"
#include "interface/ahb_slave_if.h"

// AHB Slave 通道层
class AHBSlaveChannel : public AHBSlaveBase, public sc_module {
  public:
    SC_HAS_PROCESS(AHBSlaveChannel);
    AHBSlaveChannel(sc_module_name name, DEVICE_ID slave_id) : sc_module(name), AHBSlaveBase(slave_id) {
        SC_THREAD(Process);
        sensitive << HCLK.pos();
        dont_initialize();
    }

  private:
    void Process();

  private:
    sc_port<AHBSlaveInterface>* slave_interface_;
};
