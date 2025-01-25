#include "ahb_master_shell.h"

void AHBMasterShell::Process() {
    if (HRESET.read() == false) {
        this->Reset();
        return;
    }
    if (HGRANT.read() == false) {
        // 未获得授权,需要发起总线请求
        HBUSREQ.write(true);
    } else {
        // 已获得授权,可以发起读写操作
    }
}

void AHBMasterShell::Reset() {
    // 复位自己的输出信号
    HBUSREQ.write(false);
    HLOCK.write(false);
    HTRANS.write(TRANS_TYPE::IDLE);
    HADDR.write(0);
    HSIZE.write(TRANS_SIZE::Word);
    HBURST.write(BURST_TYPE::SINGLE);
    HPROT.write(0);
    HWDATA.write(0);
}
