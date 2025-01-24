#include "ahb_slave_shell.h"

void AHBSlaveShell::Process() {
    if (HRESET.read() == false) {
        this->Reset(); // 复位
        return;
    }
    if (HSEL.read() == true) { // 当前Slave被选中
        if (HTRANS.read() == TRANS_TYPE::IDLE || HTRANS.read() == TRANS_TYPE::BUSY) {
            // 如果HTRANS是IDLE/BUSY状态,Slave应该忽略当前传输并立即响应OKAY
            HRESP.write(TRANS_RESP::OKAY);
        }
        sc_uint<BW> addr = HADDR.read();
        sc_uint<BW> size = 1 << HSIZE.read(); // 1 2 4
        if (HWRITE.read() == true) {          // 写操作
            sc_uint<BW> data = HWDATA.read();
            if (port_->Write(addr, data, size)) {
                HRESP.write(TRANS_RESP::OKAY);
            } else {
                HRESP.write(TRANS_RESP::ERROR);
            }
        } else { // 读操作
            sc_uint<BW> data = 0;
            if (port_->Read(addr, data, size)) {
                HRDATA.write(data);
                HRESP.write(TRANS_RESP::OKAY);
            } else {
                HRESP.write(TRANS_RESP::ERROR);
            }
        }
    }
}

void AHBSlaveShell::Reset() {
    // 复位自己的输出信号
    HREADY.write(true);
    HRESP.write(TRANS_RESP::OKAY);
    HRDATA.write(0);
    HSPLITx.write(0);
}
