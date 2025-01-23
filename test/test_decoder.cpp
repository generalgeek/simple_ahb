#include <systemc.h>
#include "channel/ahb_decoder_channel.h"
#include "decoder.h"

class TestDecoder : public sc_module {
  public:
    SC_HAS_PROCESS(TestDecoder);
    TestDecoder(sc_module_name name) : sc_module(name), clk_("g_clk", 10, SC_NS, 0.5, 0, SC_NS, false) {
        // 创建组件
        decoder_ = new Decoder("decoder");
        ahb_decoder_channel_ = new AHBDecoderChannel();
        // 连接绑定
        ahb_decoder_channel_->HADDR(addr_signal_);
        ahb_decoder_channel_->HSELx(sel_signal_);
        decoder_->port_(*ahb_decoder_channel_);
        decoder_->HCLK(clk_);
        SC_THREAD(Process);
    }

  public:
    sc_clock clk_; // 时钟周期10ns,高低电平各占一半,起始时刻为0,起始为低电平
    Decoder* decoder_;
    AHBDecoderChannel* ahb_decoder_channel_;
    sc_signal<sc_uint<32>> addr_signal_;
    sc_signal<sc_uint<SLAVE_COUNT>> sel_signal_;

  private:
    void Process() {
        sc_uint<32> addr = 0;
        sc_trace_file* tf = sc_create_vcd_trace_file("trace");
        sc_trace(tf, clk_, "clk");
        sc_trace(tf, addr, "addr");
        while (true) {
            addr_signal_.write(addr);
            wait(10, SC_NS);
            addr += 0x7fff;
        }
        sc_close_vcd_trace_file(tf);
    }
};

int sc_main(int argc, char* argv[]) {
    Log_Init();
    TestDecoder test("test");
    sc_start(100, SC_NS);
    return 0;
}
