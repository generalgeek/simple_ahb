#pragma once
#include <systemc.h>
#include "common.h"

// ahb master interface
class AHBMasterIF : public sc_module {
  public:
    DEVICE_ID master_id_;
    SC_HAS_PROCESS(AHBMasterIF);
    AHBMasterIF(sc_module_name name, DEVICE_ID master_id) : sc_module(name), master_id_(master_id) {
        SC_THREAD(Process);
        sensitive << HCLK.pos();
        dont_initialize();
    }

  public:
    // Input ports
    sc_in<bool> HGRANT;        // Master grant signal
    sc_in<bool> HREADY;        // Transfer ready signal
    sc_in<sc_uint<2>> HRESP;   // Transfer response
    sc_in<bool> HRESETn;       // Reset signal (active low)
    sc_in<bool> HCLK;          // Clock signal
    sc_in<sc_uint<32>> HRDATA; // Read data bus

    // Output ports
    sc_out<bool> HBUSREQ;       // Bus request signal
    sc_out<bool> HLOCK;         // Lock signal
    sc_out<sc_uint<2>> HTRANS;  // Transfer type
    sc_out<sc_uint<32>> HADDR;  // Address bus
    sc_out<bool> HWRITE;        // Write signal
    sc_out<sc_uint<3>> HSIZE;   // Transfer size
    sc_out<sc_uint<3>> HBURST;  // Burst type
    sc_out<sc_uint<4>> HPROT;   // Protection control
    sc_out<sc_uint<32>> HWDATA; // Write data bus

  private:
    virtual void Process() = 0;
    virtual void Reset() = 0;
    virtual void WriteOut() = 0;
    virtual void ReadIn() = 0;
};
