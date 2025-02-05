#include <systemc.h>
#include "ahb_decoder.h"

class TestDecoder : public sc_module {
  public:
    SC_HAS_PROCESS(TestDecoder);
    TestDecoder(sc_module_name name) : sc_module(name){
        SC_THREAD(Process);
        decoder_ = new AHBDecoder("decoder");
        decoder_->HADDR(addr_signal_);
        decoder_->HSELx(sel_signal_);
    }

  public:
    AHBDecoder* decoder_;
    sc_signal<sc_uint<BW>> addr_signal_;
    sc_signal<sc_uint<SLAVE_COUNT>> sel_signal_;

  private:
    void Process() {
        sc_uint<BW> addr = 0;
        sc_uint<SLAVE_COUNT> sel = 0;
        sc_trace_file* tf = sc_create_vcd_trace_file("trace");
        sc_trace(tf, addr, "addr");
        sc_trace(tf, sel, "sel");
        while (true) {
            addr_signal_.write(addr);
            wait(10, SC_NS);
            addr += 0x7fff;
            sel = decoder_->HSELx.read();
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
