#pragma once

#include <systemc.h>
#include "common.h"
#include "interface/ahb_slave_if.h"

class AHBSlaveShell : public sc_module {
  public:
    SC_HAS_PROCESS(AHBSlaveShell);
    AHBSlaveShell(sc_module_name name) : sc_module(name) {
        SC_METHOD(Process);
        sensitive << HCLK.pos();
        // dont_initialize();
        this->Reset();
    }

  public:
    sc_port<AHBSlaveInterface> port_; // 连接Slave功能模块的端口
    // Input ports
    sc_in<bool> HSEL;          // Slave select signal
    sc_in<sc_uint<BW>> HADDR;  // Address bus
    sc_in<bool> HWRITE;        // Write signal
    sc_in<sc_uint<2>> HTRANS;  // Transfer type
    sc_in<sc_uint<3>> HSIZE;   // Transfer size
    sc_in<sc_uint<3>> HBURST;  // Burst type
    sc_in<sc_uint<BW>> HWDATA; // Write data bus
    sc_in<bool> HRESET;        // Reset signal (active low)
    sc_in<bool> HCLK;          // Clock signal
    sc_in<sc_uint<4>> HMASTER; // Master number
    sc_in<bool> HMASTERLOCK;   // Master lock signal
    // Output ports
    sc_out<bool> HREADY;              // Ready signal
    sc_out<sc_uint<2>> HRESP;         // Response signal
    sc_out<sc_uint<BW>> HRDATA;       // Read data bus
    sc_out<bool> HSPLITx[MASTER_CNT]; // Split response

  private:
    void Process();
    void Reset();
    void ClearHSPLIT();
};
