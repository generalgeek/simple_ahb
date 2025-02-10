#include "ahb_arbiter.h"

void AHBArbiter::Process() {
    size_t id = 0;
    while (true) {
        wait(); // 等待时钟信号
        // 判断复位信号,复位时清除状态,所有主设备失去控制权
        if (HRESETn.read() == false) {
            LOG_DEBUG(logger, "{}  [{}] is reset.", sc_time_stamp().to_string(), name());
            this->Reset();
            continue;
        }
        for (id = 0; id < MASTER_CNT; id++) {
            HGRANTx[id].write(false); // 清除授权
        }
        for (id = 0; id < MASTER_CNT; id++) {
            // 仲裁规则1：之前的连续传输未完成,需要继续授权传输
            if (HSPLITx[id].read() && HLOCKx[id].read()) {
                GranteMaster(id);
                goto GRANTE_DONE;
            }
        }
        for (id = 0; id < MASTER_CNT; id++) {
            // 仲裁规则2：设备id越小,优先级越高
            if (HBUSREQx[id].read() == true) {
                GranteMaster(id);
                goto GRANTE_DONE;
            }
        }
    GRANTE_DONE:
        LOG_INFO(logger, "{}  [{}] grants authorization to Master({}).", sc_time_stamp().to_string(), name(), id);
    }
}

void AHBArbiter::Reset() {
    // 复位处理逻辑
    for (size_t i = 0; i < MASTER_CNT; i++)
        HGRANTx[i].write(false); // 清除全部授权
    HMASTER.write(0);            // 清除主设备编号
    HMASTLOCK.write(false);      // 清除总线锁定
}

void AHBArbiter::GranteMaster(DEVICE_ID id) {
    size_t i = id.to_uint();
    HGRANTx[i].write(true);
    HMASTER.write(i);
    if (HLOCKx[i].read() == true) {
        HMASTLOCK.write(true); // 如果设备请求锁定总线,则锁定总线
    } else {
        HMASTLOCK.write(false); // 清除总线锁定
    }
}
