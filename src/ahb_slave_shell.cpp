#include "ahb_slave_shell.h"

void AHBSlaveShell::Process() {
    while (true) {
        wait(); // 等待时钟信号
        if (HRESET.read() == false) {
            LOG_DEBUG(logger, "{}  [{}] is reset.", sc_time_stamp().to_string(), name());
            this->Reset(); // 复位
            return;
        }
        if (HSEL.read() == true) { // 当前Slave被选中
            if (HTRANS.read() == TRANS_TYPE::IDLE || HTRANS.read() == TRANS_TYPE::BUSY) {
                // 如果HTRANS是IDLE/BUSY状态,Slave应该忽略当前传输并立即响应OKAY
                HRESP.write(TRANS_RESP::OKAY);
                ClearHSPLIT();
                return;
            }
            // 检查是否锁定事务
            if (HMASTERLOCK.read() == true) {
                // 如果HMASTERLOCK为true，表示该Master已锁定总线，其他Master不能继续
                ClearHSPLIT(); // 设置所有HSPLITx位为0，禁止其他Master访问
            } else {
                // 如果没有锁定事务，且当前Slave没有返回SPLIT响应，则设置HSPLITx
                if (HTRANS.read() != TRANS_TYPE::IDLE && HTRANS.read() != TRANS_TYPE::BUSY) {
                    HSPLITx[HMASTER.read()].write(true);
                    // HSPLITx.write(HSPLITx.read() | (1 << HMASTER.read())); // 允许当前Master继续
                }
            }
            sc_uint<BW> addr = HADDR.read();
            sc_uint<BW> size = 1 << HSIZE.read(); // 1 2 4
            if (HWRITE.read() == true) {          // 写操作
                sc_uint<BW> data = HWDATA.read();
                if (port_->SlaveWrite(addr, data, size)) {
                    HRESP.write(TRANS_RESP::OKAY);
                    HSPLITx[HMASTER.read()].write(false);
                    // HSPLITx.write(HSPLITx.read() & ~(1 << HMASTER.read())); // 写操作完成后清除对应位
                } else {
                    HRESP.write(TRANS_RESP::ERROR);
                    ClearHSPLIT(); // 错误时清除所有HSPLITx位
                }
            } else { // 读操作
                sc_uint<BW> data = 0;
                if (port_->SlaveRead(addr, data, size)) {
                    HRDATA.write(data);
                    HRESP.write(TRANS_RESP::OKAY);
                    HSPLITx[HMASTER.read()].write(false);
                    // HSPLITx.write(HSPLITx.read() & ~(1 << HMASTER.read())); // 读操作完成后清除对应位
                } else {
                    HRESP.write(TRANS_RESP::ERROR);
                    ClearHSPLIT(); // 错误时清除所有HSPLITx位
                }
            }
        }
    }
}

void AHBSlaveShell::Reset() {
    // 复位自己的输出信号
    HREADY.write(true);
    HRESP.write(TRANS_RESP::OKAY);
    HRDATA.write(0);
    ClearHSPLIT();
}

inline void AHBSlaveShell::ClearHSPLIT() {
    for (size_t i = 0; i < MASTER_CNT; i++)
        HSPLITx[i].write(false);
}
