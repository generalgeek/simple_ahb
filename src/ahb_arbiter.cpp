#include "ahb_arbiter.h"

void AHBAribter::Process() {

    // 判断复位信号,复位时清除状态,所有主设备失去控制权
    if (HRESETn.read() == false) {
        this->Reset();
        return;
    }
    // 判断是否有主设备请求
    if (HBUSREQx.read() == 0) {
        return; // 没有主设备请求
    }
    DEVICE_ID cur_id = HMASTER.read();
    sc_uint<MASTER_COUNT> grantx = 0;
    if (HMASTLOCK.read() && HGRANTx.read().range(cur_id, cur_id)) {
        // 仲裁规则1：如果总线在之前的仲裁中 被已授权的设备 锁定
        grantx[cur_id] = 1;
        HGRANTx.write(grantx);
        return;
    }
    for (DEVICE_ID i = 0; i < MASTER_COUNT; i++) {
        // 仲裁规则2：设备id越小,优先级越高
        if (HBUSREQx.read().range(i, i)) {
            grantx[i] = 1;
            HGRANTx.write(grantx);
            HMASTER.write(i);
            if (HLOCKx.read().range(i, i)) {
                HMASTLOCK.write(true); // 如果设备请求锁定总线,则锁定总线
            } else {
                HMASTLOCK.write(false); // 清除总线锁定
            }
            return; // 只授权1个设备
        }
    }
}

void AHBAribter::Reset() {
    // 复位处理逻辑
    HGRANTx.write(0);       // 清除全部授权
    HMASTER.write(0);       // 清除主设备编号
    HMASTLOCK.write(false); // 清除总线锁定
}
