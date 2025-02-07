#pragma once
#include <systemc.h>
#include "interface/ahb_master_if.h"

class AHBMasterShell : public sc_module {
  public:
    SC_HAS_PROCESS(AHBMasterShell);
    AHBMasterShell(sc_module_name name) : sc_module(name) {
        SC_THREAD(Process);
        sensitive << HCLK.pos();
        // dont_initialize();
    }

  public:
    sc_port<AHBMasterInterface> port_;
    // Input ports
    sc_in<bool> HGRANT;        // Master grant signal
    sc_in<bool> HREADY;        // Transfer ready signal
    sc_in<sc_uint<2>> HRESP;   // Transfer response
    sc_in<bool> HRESET;        // Reset signal (active low)
    sc_in<bool> HCLK;          // Clock signal
    sc_in<sc_uint<BW>> HRDATA; // Read data bus
    // Output ports
    sc_out<bool> HBUSREQ;       // Bus request signal
    sc_out<bool> HLOCK;         // Lock signal
    sc_out<sc_uint<2>> HTRANS;  // Transfer type
    sc_out<sc_uint<BW>> HADDR;  // Address bus
    sc_out<bool> HWRITE;        // Write signal
    sc_out<sc_uint<3>> HSIZE;   // Transfer size
    sc_out<sc_uint<3>> HBURST;  // Burst type
    sc_out<sc_uint<4>> HPROT;   // Protection control
    sc_out<sc_uint<BW>> HWDATA; // Write data bus

  private:
    void Process();
    void Reset();
};
