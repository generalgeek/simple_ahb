#include "simple_cpu.h"

SimpleCPU::SimpleCPU(DEVICE_ID id) : id_(id) {
}

bool SimpleCPU::Read(sc_uint<BW> addr, sc_uint<BW>& data, sc_uint<BW> size) {
    return false;
}

bool SimpleCPU::Write(sc_uint<BW> addr, sc_uint<BW> data, sc_uint<BW> size) {
    return false;
}
