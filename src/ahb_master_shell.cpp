#include "ahb_master_shell.h"

static size_t trans_cycle[4]{ 1, 4, 8, 16 };

void AHBMasterShell::Process() {
    sc_uint<BW> data = 0;
    size_t cycle = 0;
    MasterTask task;
    bool task_done = true;
    while (true) {
        wait(); // 等待时钟信号
        if (HRESET.read() == false) {
            LOG_DEBUG(logger, "{}  [{}] is reset.", sc_time_stamp().to_string(), name());
            this->Reset();
            continue;
        }

        if (task_done) { // 需要获取一个新的task
            LOG_INFO(logger, "{}  [{}] get a new master task.", sc_time_stamp().to_string(), name());
            task = port_->GetTask();
            cycle = trans_cycle[task.burst_type / 2]; // 1/4/8
            task_done = false;
        }

        if (HGRANT.read() == false) { // 未获得授权,需要发起总线请求
            LOG_DEBUG(logger, "{}  [{}] initiates a bus request.", sc_time_stamp().to_string(), name());
            HBUSREQ.write(true);
            if (cycle > 1) { // 如果要进行连续传输,申请锁定总线
                HLOCK.write(true);
            }
            continue;
        } else { // 已获得授权,可以发起读写操作
            LOG_INFO(logger, "{}  [{}] receives bus grant and starts read/write operations.",
                     sc_time_stamp().to_string(), name());
            HWRITE.write(task.write);
            HBURST.write(task.burst_type);
            HSIZE.write(task.trans_size);
            HADDR.write(task.slave_addr);
            for (size_t i = 0; i < cycle; i++) {
                if (i == 0) {
                    HTRANS.write(TRANS_TYPE::NONSEQ);
                } else {
                    HTRANS.write(TRANS_TYPE::SEQ);
                }
                while (true) { // 进行一笔数据的读/写
                    if (HREADY.read() == false) {
                        wait();
                    }
                    if (task.write) { // 写操作, 从自己内部缓存读数据然后写出去
                        port_->MasterRead(task.inner_addr, data, 1 << task.trans_size);
                        HWDATA.write(data);
                        wait();
                    } else { // 读操作, 从外面读数据进来写入内部缓存
                        wait();
                        data = HRDATA.read();
                        port_->MasterWrite(task.inner_addr, data, 1 << task.trans_size);
                    }
                    while (HRESP.read() == TRANS_RESP::SPLIT) {
                        // 如果响应是SPLIT，继续等待更多时钟周期
                        wait();
                    }
                    if (HRESP.read() == TRANS_RESP::OKAY) {
                        break; // 单笔数据读写成功
                    } else if (HRESP.read() == TRANS_RESP::ERROR || HRESP.read() == TRANS_RESP::RETRY) {
                        continue; // 单笔数据读写失败,继续重试
                    }
                }
                HADDR.write(HADDR.read() + (1 << HSIZE.read())); // 外部操作地址递增1/2/4
                task.inner_addr += (1 << task.trans_size);
            }
            // 上面for循环结束后就完成了一个完整Task的处理
            task_done = true;
            HBUSREQ.write(false);
            HTRANS.write(TRANS_TYPE::IDLE); // 挂起
            HLOCK.write(false);             // 释放总线
            wait();                         // 增加一个时钟等待周期，让仲裁器更新信号
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
