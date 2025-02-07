#include "ahb_decoder.h"

void AHBDecoder::Process() {
    if (HRESETn.read() == false) {
        LOG_DEBUG(logger, "{}  [{}] is reset.", sc_time_stamp().to_string(), name());
        this->Reset();
        return;
    }
    DEVICE_ID slave_sel = this->AddrDecode(HADDR.read());
    HSELx[slave_sel].write(true);
}

void AHBDecoder::Reset() {
    for (size_t i = 0; i < SLAVE_CNT; i++)
        HSELx[i].write(false);
}

DEVICE_ID AHBDecoder::AddrDecode(sc_uint<BW> addr) {
    DEVICE_ID sel = 0;
    if (DMA_ADDR_START <= addr && addr <= DMA_ADDR_END) {
        sel = SLAVE_DMA_ID;
        LOG_DEBUG(logger, "{}  [{}] addr={:#010x}, slave dma be selected, HSELx={:#b}.", sc_time_stamp().to_string(),
                  name(), addr.to_uint(), sel.to_uint());
    } else if (RAM_ADDR_START <= addr && addr <= RAM_ADDR_END) {
        sel = SLAVE_RAM_ID;
        LOG_DEBUG(logger, "{}  [{}] addr={:#010x}, slave ram be selected, HSELx={:#b}.", sc_time_stamp().to_string(),
                  name(), addr.to_uint(), sel.to_uint());
    } else {
        LOG_ERROR(logger, "{}  [{}] addr={:#010x} is out of bounds and cannot be matched to any slave device.",
                  sc_time_stamp().to_string(), name(), addr.to_uint());
        // #  表示带有0x前缀的十六进制格式。
        // 0  表示用零填充左边的空位。
        // 10 表示总宽度为10个字符（包括0x前缀），即实际数字部分为8位宽。
        // x  表示以小写字母输出十六进制数。
    }
    return sel;
}
