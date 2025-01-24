#include "simple_cpu.h"

SimpleCPU::SimpleCPU(DEVICE_ID id) : id_(id) {
}

void SimpleCPU::Read(sc_uint<BW> addr, sc_uint<BW>& data) {
}

void SimpleCPU::Write(sc_uint<BW> addr, sc_uint<BW> data) {
}
