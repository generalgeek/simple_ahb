#include <systemc.h>
#include "ahb_arbiter.h"
#include "ahb_decoder.h"
#include "ahb_master_shell.h"
#include "ahb_slave_shell.h"
#include "common.h"
#include "log.h"
#include "ram_onchip.h"
#include "simple_cpu.h"

class AHBBus : public sc_module {
  public:
    // SC_HAS_PROCESS(AHBBus);
    AHBBus(sc_module_name name);

  public:
    SimpleCPU* simple_cpu_;
    AHBMasterShell* simple_cpu_shell_;
    RAMOnChip* ram_onchip_;
    AHBSlaveShell* ram_shell_;
    AHBDecoder* decoder_;
    AHBArbiter* arbiter_;

  public: // AHB Signal
    sc_signal<bool> HCLK;
    sc_signal<bool> HRESETn;
    sc_signal<sc_uint<BW>> HADDR;
    sc_signal<sc_uint<2>> HTRANS;
    sc_signal<bool> HWRITE;
    sc_signal<sc_uint<3>> HSIZE;
    sc_signal<sc_uint<3>> HBURST;
    sc_signal<sc_uint<4>> HPORT;
    sc_signal<sc_uint<BW>> HWDATA;
    sc_signal<bool> HSELx[SLAVE_CNT];
    sc_signal<sc_uint<BW>> HRDATA;
    sc_signal<bool> HREADY;
    sc_signal<sc_uint<2>> HRESP;
    sc_signal<bool> HBUSREQx[MASTER_CNT];
    sc_signal<bool> HLOCKx[MASTER_CNT];
    sc_signal<bool> HGRANTx[MASTER_CNT];
    sc_signal<sc_uint<4>> HMASTER;
    sc_signal<bool> HMASTERLOCK;
    sc_signal<bool> HSPLITx[MASTER_CNT];
};
