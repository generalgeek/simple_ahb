#include <systemc.h>
#include "ahb_decoder_shell.h"
#include "decoder.h"

class TestDecoder : public sc_module {
  public:
    SC_HAS_PROCESS(TestDecoder);
    TestDecoder(sc_module_name name) : sc_module(name){
        SC_THREAD(Process);
        decoder_ = new Decoder();
        decoderShell_ = new AHBDecoderShell("decodershell");
        decoderShell_->decoder_port_(*decoder_);
        decoderShell_->HADDR(addr_signal_);
        decoderShell_->HSELx(sel_signal_);
    }

  public:
    Decoder* decoder_;
    AHBDecoderShell* decoderShell_;
    sc_signal<sc_uint<32>> addr_signal_;
    sc_signal<sc_uint<SLAVE_COUNT>> sel_signal_;

  private:
    void Process() {
        sc_uint<32> addr = 0;
        sc_uint<SLAVE_COUNT> sel = 0;
        sc_trace_file* tf = sc_create_vcd_trace_file("trace");
        sc_trace(tf, addr, "addr");
        // sc_trace(tf, sel, "sel");
        while (true) {
            addr_signal_.write(addr);
            wait(10, SC_NS);
            addr += 0x7fff;
            sel = decoderShell_->HSELx.read();
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
