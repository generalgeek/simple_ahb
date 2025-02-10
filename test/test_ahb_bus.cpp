#include <systemc.h>
#include "ahb_bus.h"

int sc_main(int argc, char* argv[]) {
    Log_Init();
    AHBBus ahb_bus("ahb_bus");

    sc_trace_file* tf = sc_create_vcd_trace_file("trace");
    {
        sc_trace(tf, ahb_bus.HCLK, "HCLK");
        sc_trace(tf, ahb_bus.HRESETn, "HRESETn");
        sc_trace(tf, ahb_bus.HADDR, "HADDR");
        sc_trace(tf, ahb_bus.HTRANS, "HTRANS");
        sc_trace(tf, ahb_bus.HWRITE, "HWRITE");
        sc_trace(tf, ahb_bus.HSIZE, "HSIZE");
        sc_trace(tf, ahb_bus.HBURST, "HBURST");
        sc_trace(tf, ahb_bus.HPORT, "HPORT");
        sc_trace(tf, ahb_bus.HWDATA, "HWDATA");
        for (size_t i = 0; i < SLAVE_CNT; i++)
            sc_trace(tf, ahb_bus.HSELx[i], std::string("HSEL_" + std::to_string(i)));
        sc_trace(tf, ahb_bus.HRDATA, "HRDATA");
        sc_trace(tf, ahb_bus.HREADY, "HREADY");
        sc_trace(tf, ahb_bus.HRESP, "HRESP");
        for (size_t i = 0; i < MASTER_CNT; i++) {
            sc_trace(tf, ahb_bus.HBUSREQx[i], std::string("HBUSREQ_" + std::to_string(i)));
            sc_trace(tf, ahb_bus.HLOCKx[i], std::string("HLOCK_" + std::to_string(i)));
            sc_trace(tf, ahb_bus.HGRANTx[i], std::string("HGRANT_" + std::to_string(i)));
            sc_trace(tf, ahb_bus.HSPLITx[i], std::string("HSPLIT_" + std::to_string(i)));
        }
        sc_trace(tf, ahb_bus.HMASTER, "HMASTER");
        sc_trace(tf, ahb_bus.HMASTLOCK, "HMASTLOCK");
    }

    sc_start(200, SC_NS);
    sc_close_vcd_trace_file(tf);
    return 0;
}
