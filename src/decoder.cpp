#include "decoder.h"

void Decoder::Process() {
    sc_uint<32> addr = 0;
    port_->Read(addr);
    port_->Write(this->AddrDecode(addr));
}

sc_uint<SLAVE_COUNT> Decoder::AddrDecode(sc_uint<32> addr) {
    sc_uint<SLAVE_COUNT> selx = 0;
    if (DMA_ADDR_START <= addr && addr <= DMA_ADDR_END) {
        selx[SLAVE_DMA_ID] = 1;
        LOG_INFO(logger, "addr[{:#010x}], slave dma be selected, HSELx={:#b}.", addr.to_uint(), selx.to_uint());
    } else if (RAM_ADDR_START <= addr && addr <= RAM_ADDR_END) {
        selx[SLAVE_RAM_ID] = 1;
        LOG_INFO(logger, "addr[{:#010x}], slave ram be selected, HSELx={:#b}.", addr.to_uint(), selx.to_uint());
    } else {
        LOG_ERROR(logger, "addr[{:#010x}] is out of bounds and cannot be matched to any slave device.", addr.to_uint());
        // #  表示带有0x前缀的十六进制格式。
        // 0  表示用零填充左边的空位。
        // 10 表示总宽度为10个字符（包括0x前缀），即实际数字部分为8位宽。
        // x  表示以小写字母输出十六进制数。
    }
    return selx;
}
