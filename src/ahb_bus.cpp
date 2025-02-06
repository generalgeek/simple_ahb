#include "ahb_bus.h"

AHBBus::AHBBus(sc_module_name name) : sc_module(name) {
    // init simple_cpu(Master)
    simple_cpu_ = new SimpleCPU(MASTER_CPU_ID);
    simple_cpu_shell_ = new AHBMasterShell("simple_cpu_shell");
    simple_cpu_shell_->port_(*simple_cpu_);
    // bind input signal
    simple_cpu_shell_->HGRANT(HGRANTx[MASTER_CPU_ID]);
    simple_cpu_shell_->HREADY(HREADY);
    simple_cpu_shell_->HRESP(HRESP);
    simple_cpu_shell_->HRESET(HRESETn);
    simple_cpu_shell_->HCLK(HCLK);
    simple_cpu_shell_->HRDATA(HRDATA);
    // bind output signal
    simple_cpu_shell_->HBUSREQ(HBUSREQx[MASTER_CPU_ID]);
    simple_cpu_shell_->HLOCK(HLOCKx[MASTER_CPU_ID]);
    simple_cpu_shell_->HTRANS(HTRANS);
    simple_cpu_shell_->HADDR(HADDR);
    simple_cpu_shell_->HWRITE(HWRITE);
    simple_cpu_shell_->HSIZE(HSIZE);
    simple_cpu_shell_->HBURST(HBURST);
    simple_cpu_shell_->HPROT(HPORT);
    simple_cpu_shell_->HWDATA(HWDATA);

    // init ram_onchip(Slave)
    ram_onchip_ = new RAMOnChip(SLAVE_RAM_ID);
    ram_shell_ = new AHBSlaveShell("ram_onchip_shell");
    ram_shell_->port_(*ram_onchip_);
    // bind input signal
    ram_shell_->HSEL(HSELx[SLAVE_RAM_ID]);
    ram_shell_->HADDR(HADDR);
    ram_shell_->HWRITE(HWRITE);
    ram_shell_->HTRANS(HTRANS);
    ram_shell_->HSIZE(HSIZE);
    ram_shell_->HBURST(HBURST);
    ram_shell_->HWDATA(HWDATA);
    ram_shell_->HRESET(HRESETn);
    ram_shell_->HCLK(HCLK);
    ram_shell_->HMASTER(HMASTER);
    ram_shell_->HMASTERLOCK(HMASTERLOCK);
    // bind output signal
    ram_shell_->HREADY(HREADY);
    ram_shell_->HRESP(HRESP);
    ram_shell_->HRDATA(HRDATA);
    for (size_t i = 0; i < MASTER_CNT; i++)
        ram_shell_->HSPLITx[i](HSPLITx[i]);

    // init arbiter
    arbiter_ = new AHBArbiter("arbiter");
    // bind input signal
    for (size_t i = 0; i < MASTER_CNT; i++) {
        arbiter_->HBUSREQx[i](HBUSREQx[i]);
        arbiter_->HLOCKx[i](HLOCKx[i]);
        arbiter_->HSPLITx[i](HSPLITx[i]);
    }
    arbiter_->HADDR(HADDR);
    arbiter_->HTRANS(HTRANS);
    arbiter_->HBURST(HBURST);
    arbiter_->HRESP(HRESP);
    arbiter_->HREADY(HREADY);
    arbiter_->HRESETn(HRESETn);
    arbiter_->HCLK(HCLK);
    // bind output signal
    for (size_t i = 0; i < MASTER_CNT; i++)
        arbiter_->HGRANTx[i](HGRANTx[i]);
    arbiter_->HMASTER(HMASTER);
    arbiter_->HMASTLOCK(HMASTERLOCK);

    // init decoder
    decoder_ = new AHBDecoder("decoder");
    decoder_->HADDR(HADDR);
    for (size_t i = 0; i < SLAVE_CNT; i++)
        decoder_->HSELx[i](HSELx[i]);
}
