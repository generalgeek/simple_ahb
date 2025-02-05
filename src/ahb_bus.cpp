#include "ahb_bus.h"

AHBBus::AHBBus(sc_module_name name) : sc_module(name) {
    // init simple_cpu(Master)
    simple_cpu_ = new SimpleCPU(MASTER_PE_ID);
    simple_cpu_shell_ = new AHBMasterShell("simple_cpu_shell");
    simple_cpu_shell_->port_(*simple_cpu_);
    simple_cpu_shell_->HGRANT(HGRANTx[0]);

    // init decoder
    decoder_ = new AHBDecoder("decoder");
    decoder_->HADDR(HADDR);
    for (size_t i = 0; i < SLAVE_CNT; i++)
        decoder_->HSELx[i](HSELx[i]);
}
