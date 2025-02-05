#include "ahb_master_shell.h"

void AHBMasterShell::Process() {
    sc_uint<BW> data = 0;
    size_t cnt = 0;
    while (true) {
        wait(); // 等待时钟信号
        if (HRESET.read() == false) {
            this->Reset();
            continue;
        }
        if (HGRANT.read() == false) {
            // 未获得授权,需要发起总线请求
            HBUSREQ.write(true);
            continue;
        } else {
            // 已获得授权,可以发起读写操作
            MasterTask task = port_->GetTask();
            HWRITE.write(task.write);
            HBURST.write(task.burst_type);
            HSIZE.write(task.trans_size);
            HADDR.write(task.slave_addr);
            HLOCK.write(true);
            cnt = 1 << (HBURST.read().to_uint() / 2); // 1/4/8
            for (size_t i = 0; i < cnt; i++) {
                if (i == 0) {
                    HTRANS.write(TRANS_TYPE::NONSEQ);
                } else {
                    HTRANS.write(TRANS_TYPE::SEQ);
                }
                while (true) {
                    if (task.write) {
                        // 写操作, 从自己内部缓存读数据然后写出去
                        port_->Read(task.inner_addr, data, task.trans_size);
                        HWDATA.write(data);
                    } else {
                        // 读操作, 从外面读数据进来写入内部缓存
                        data = HRDATA.read();
                        port_->Write(task.inner_addr, data, task.trans_size);
                    }
                    wait(); // 上述操作完成,在下一个时钟周期查看Slave的相应情况
                    while (HRESP.read() == TRANS_RESP::SPLIT) {
                        // 如果响应是SPLIT，继续等待更多时钟周期
                        wait();
                    }
                    if (HRESP.read() == TRANS_RESP::OKAY) {
                        break;
                    } else if (HRESP.read() == TRANS_RESP::ERROR || HRESP.read() == TRANS_RESP::RETRY) {
                        continue;
                    }
                }
                HADDR.write(HADDR.read() + (1 << HSIZE.read())); // 外部操作地址递增1/2/4
                task.inner_addr += (1 << task.trans_size);
            }
            HLOCK.write(false); // 释放总线
        }
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
