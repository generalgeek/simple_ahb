#include "channel/ahb_decoder_channel.h"

void AHBDecoderChannel::Read(sc_uint<32>& data) {
    data = HADDR.read();
}

void AHBDecoderChannel::Write(sc_uint<SLAVE_COUNT> data) {
    HSELx.write(data);
}
