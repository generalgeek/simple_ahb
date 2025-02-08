#include "ahb_arbiter.h"

void AHBArbiter::Process() {
    while (true) {
        wait(); // 等待时钟信号
        // 判断复位信号,复位时清除状态,所有主设备失去控制权
        if (HRESETn.read() == false) {
            LOG_DEBUG(logger, "{}  [{}] is reset.", sc_time_stamp().to_string(), name());
            this->Reset();
            continue;
        }
        DEVICE_ID cur_id = HMASTER.read();
        if (HBUSREQx[cur_id].read() && HMASTLOCK.read() && HGRANTx[cur_id].read()) {
            // 仲裁规则1：如果总线在之前的仲裁中 被已授权的设备 锁定
            HGRANTx[cur_id].write(true);
            continue;
        }
        for (size_t i = 0; i < MASTER_CNT; i++) {
            HGRANTx[i].write(false); // 清除授权
        }
        for (size_t i = 0; i < MASTER_CNT; i++) {
            // 仲裁规则2：设备id越小,优先级越高
            if (HBUSREQx[i].read() == true) {
                LOG_INFO(logger, "{}  [{}] grants authorization to Master({}).", sc_time_stamp().to_string(), name(),
                         i);
                HGRANTx[i].write(true);
                HMASTER.write(i);
                if (HLOCKx[i].read() == true) {
                    HMASTLOCK.write(true); // 如果设备请求锁定总线,则锁定总线
                } else {
                    HMASTLOCK.write(false); // 清除总线锁定
                }
                break; // 只授权1个设备
            }
        }
    }
}

void AHBArbiter::Reset() {
    // 复位处理逻辑
    for (size_t i = 0; i < MASTER_CNT; i++)
        HGRANTx[i].write(false); // 清除全部授权
    HMASTER.write(0);            // 清除主设备编号
    HMASTLOCK.write(false);      // 清除总线锁定
}
