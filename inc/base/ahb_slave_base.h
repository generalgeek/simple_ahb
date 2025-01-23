#pragma once
#include <systemc.h>
#include "common.h"

// ahb slave base
class AHBSlaveBase {
  public:
    DEVICE_ID slave_id_;
    AHBSlaveBase(DEVICE_ID slave_id) : slave_id_(slave_id) {}

  public:
    // Input ports
    sc_in<bool> HSEL;          // Slave select signal
    sc_in<sc_uint<32>> HADDR;  // Address bus
    sc_in<bool> HWRITE;        // Write signal
    sc_in<sc_uint<2>> HTRANS;  // Transfer type
    sc_in<sc_uint<2>> HSIZE;   // Transfer size
    sc_in<sc_uint<3>> HBURST;  // Burst type
    sc_in<sc_uint<32>> HWDATA; // Write data bus
    sc_in<bool> HRESETn;       // Reset signal (active low)
    sc_in<bool> HCLK;          // Clock signal
    sc_in<sc_uint<4>> HMASTER; // Master number
    sc_in<bool> HMASTERLOCK;   // Master lock signal

    // Output ports
    sc_out<bool> HREADY;         // Ready signal
    sc_out<sc_uint<2>> HRESP;    // Response signal
    sc_out<sc_uint<32>> HRDATA;  // Read data bus
    sc_out<sc_uint<16>> HSPLITx; // Split response
};
