#pragma once
#include <systemc.h>
#include "common.h"
#include "interface/ahb_arbiter_if.h"

class AHBAribterShell : public sc_module {
  public:
    SC_HAS_PROCESS(AHBAribterShell);
    AHBAribterShell(sc_module_name name) : sc_module(name) {
        SC_METHOD(Process);
        sensitive << HCLK.pos();
        dont_initialize();
    }

  public:
    sc_port<AHBArbiterInterface> port_;
    // 输入信号
    sc_in<sc_uint<MASTER_COUNT>> HBUSREQx; // 每个比特表示一个主设备的总线请求信号,允许多个bit位为1
    sc_in<sc_uint<MASTER_COUNT>> HLOCKx; // 每个比特表示一个主设备的总线锁信号,允许多个bit位为1
    sc_in<sc_uint<32>> HADDR;            // 当前地址总线，辅助判断是否需要拆分或重试
    sc_in<sc_uint<16>> HSPLITx;          // 分裂传输信号，从设备用来释放总线的控制
    sc_in<sc_uint<2>> HTRANS;            // 当前传输类型（IDLE, BUSY, SEQUENTIAL, NONSEQUENTIAL）
    sc_in<sc_uint<3>> HBURST;            // 突发传输类型（SINGLE, INCR, WRAP4, WRAP8, WRAP16 等）
    sc_in<sc_uint<2>> HRESP;             // 响应信号（OKAY, ERROR, RETRY, SPLIT）
    sc_in<bool> HREADY;                  // 从设备是否准备好信号
    sc_in<bool> HRESETn;                 // 复位信号（低电平有效）
    sc_in<bool> HCLK;                    // 时钟信号
    // 输出信号
    sc_out<sc_uint<MASTER_COUNT>> HGRANTx; // 每个bit位表示一个主设备的总线控制权,只允许一个bit位为1
    sc_out<sc_uint<4>> HMASTER;            // 当前拥有总线控制权的主设备编号
    sc_out<bool> HMASTLOCK;                // 当前主设备是否锁定总线

  private:
    void Process();
};
