#include "ahb_decoder_shell.h"

void AHBDecoderShell::Process() {
    HSELx.write(decoder_port_->AddrDecode(HADDR.read()));
}
