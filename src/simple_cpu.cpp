/* #include "simple_cpu.h"

void SimpleCPU::Process() {
    this->Prepare(); // 初始化了一套Read_INCR4的配置
    size_t beat_cnt = 0;
    while (true) {
        wait(); // 等待时钟上升沿
        if (HRESETn.read() == false) {
            this->Reset();
            continue;
        }
        if (HGRANT.read()) { // 仲裁器授权了当前Master
            if (beat_cnt == 0) {
                HTRANS.write(TRANS_TYPE::NONSEQ); // 表示burst/single传输的第一次传输
            } else {
                HTRANS.write(TRANS_TYPE::SEQ); // burst传输中的剩余部分
            }
            while (true) {
                if (HWRITE.read()) {
                    WriteOut(); // 写操作,往HWDATA写输出
                } else {
                    ReadIn(); // 读操作,从HRDATA读输入
                }
                wait(); //  等待从设备响应,传输成功才能进行下一beat,否则重试
                if (HREADY.read() && HRESP.read() == TRANS_RESP::OKAY)
                    break;
            }
            HADDR.write(HADDR.read() + BUS_WIDTH);
            beat_cnt++;
            if (beat_cnt == trans_beat_size_) { // 传输完成
                HTRANS.write(TRANS_TYPE::IDLE); // 表示传输完成
                HLOCK.write(false); // 解除总线锁定
            }
        } else {
            HBUSREQ.write(true); // 发起总线访问请求
        }
    }
}

void SimpleCPU::Reset() {
    // 复位处理逻辑
    HBUSREQ.write(false);
    HLOCK.write(false);
    HTRANS.write(0);
    HADDR.write(0);
    HWRITE.write(false);
    HSIZE.write(0);
    HBURST.write(0);
    HPROT.write(0);
    HWDATA.write(0);
}

void SimpleCPU::WriteOut() {
    // 从从HWDATA端口写出
    sc_uint<3> trans_size = HSIZE.read();
    if (trans_size == TRANS_SIZE::Byte) {
        HWDATA.write(cache_[cache_ptr_] & 0xff);
        cache_ptr_ = (cache_ptr_ + 1) % CACHE_SIZE;
    } else if (trans_size == TRANS_SIZE::Halfword) {
        HWDATA.write(cache_[cache_ptr_] & 0xffff);
        cache_ptr_ = (cache_ptr_ + 1) % CACHE_SIZE;
    } else if (trans_size == TRANS_SIZE::Word) {
        HWDATA.write(cache_[cache_ptr_]);
        cache_ptr_ = (cache_ptr_ + 1) % CACHE_SIZE;
    } else {
        LOG_ERROR(logger, "HSIZE={:#b} not support.", trans_size.to_uint());
    }
}

void SimpleCPU::ReadIn() {
    // 从HRDATA端口读入
    sc_uint<3> trans_size = HSIZE.read();
    if (trans_size == TRANS_SIZE::Byte) {
        cache_[cache_ptr_] = (cache_[cache_ptr_] & 0xffff'ff00) | (HRDATA.read() & 0xff);
        cache_ptr_ = (cache_ptr_ + 1) % CACHE_SIZE;
    } else if (trans_size == TRANS_SIZE::Halfword) {
        cache_[cache_ptr_] = (cache_[cache_ptr_] & 0xffff'0000) | (HRDATA.read() & 0xffff);
        cache_ptr_ = (cache_ptr_ + 1) % CACHE_SIZE;
    } else if (trans_size == TRANS_SIZE::Word) {
        cache_[cache_ptr_] = HRDATA.read();
        cache_ptr_ = (cache_ptr_ + 1) % CACHE_SIZE;
    } else {
        LOG_ERROR(logger, "HSIZE={:#b} not support.", trans_size.to_uint());
    }
}

void SimpleCPU::Prepare() {
    sc_uint<32> addr = RAM_ADDR_START;
    sc_uint<3> trans_size = TRANS_SIZE::Word;
    sc_uint<3> burst_type = BURST_TYPE::INCR4;

    HLOCK.write(true); // burst传输需要占用总线
    HBURST.write(burst_type); // INCR4类型
    HSIZE.write(trans_size); // 32bit位宽读
    HWRITE.write(false); // 读操作
    HPROT.write(0b1110); // 非特权访问，可缓存，缓冲，数据访问
    trans_beat_size_ = 4; // INCR4类型需要传输4个beat,传输过程需要占用4个时钟周期
}
 */