#include "ahb_arbiter.h"

void AHBArbiter::Process() {
    // 判断复位信号,复位时清除状态,所有主设备失去控制权
    if (HRESETn.read() == false) {
        this->Reset();
        return;
    }
    DEVICE_ID cur_id = HMASTER.read();
    if (HMASTLOCK.read() && HGRANTx[cur_id].read()) {
        // 仲裁规则1：如果总线在之前的仲裁中 被已授权的设备 锁定
        HGRANTx[cur_id].write(true);
        return;
    }
    for (DEVICE_ID i = 0; i < MASTER_CNT; i++) {
        // 仲裁规则2：设备id越小,优先级越高
        if (HBUSREQx[i].read() == true) {
            HGRANTx[i].write(true);
            HMASTER.write(i);
            if (HLOCKx[i].read() == true) {
                HMASTLOCK.write(true); // 如果设备请求锁定总线,则锁定总线
            } else {
                HMASTLOCK.write(false); // 清除总线锁定
            }
            return; // 只授权1个设备
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
