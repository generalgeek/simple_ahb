#pragma once
#include <cstddef>
#include <cstdint>
#include <systemc.h>

#define BW 32 // Bus Width
// #define BUS_WIDTH 4
#define MASTER_COUNT  16
#define SLAVE_COUNT   16
#define MASTER_PE_ID  0
#define MASTER_DMA_ID 1
#define SLAVE_DMA_ID  0
#define SLAVE_RAM_ID  1

using DEVICE_ID = sc_uint<4>;

// beat 一个时钟周期总线传输的数据量，在32位宽的总线上，一个beat是4Byte
enum BURST_TYPE : uint8_t {
    SINGLE = 0b000, // Single transfer
    INCR = 0b001,   // Incrementing burst of unspecified length
    WRAP4 = 0b010,  // 4-beat warpping burst
    INCR4 = 0b011,  // 4-beat incrementing burst
    WRAP8 = 0b100,  // 8-beat warpping burst
    INCR8 = 0b101,  // 8-beat incrementing burst
    WRAP16 = 0b110, // 16-beat warpping burst
    INCR16 = 0b111, // 16-beat incrementing burst
}; // HBURST[2:0]

enum TRANS_SIZE : uint8_t {
    Byte = 0b000,     // 传输宽度为8-bit
    Halfword = 0b001, // 传输宽度为16-bit
    Word = 0b010,     // 传输宽度为32-bit
}; // HSIZE[2:0]

enum TRANS_RESP : uint8_t {
    OKAY = 0b00,  // 表示传输正常进行，当HREADY变为高电平，表示传输已经完成
    ERROR = 0b01, // 表示传输发生了错误
    RETRY = 0b10, // 表示传输需要重新发起
    SPLIT = 0b11, // 表示传输被拆分，主设备需要释放总线，从设备在稍后时间响应
}; // HRESP[1:0]

enum TRANS_TYPE : uint8_t {
    IDLE = 0b00,   // 表示不需要进行数据传输
    BUSY = 0b01,   // 表示总线上的主设备正在进行burst传输
    NONSEQ = 0b10, // 表示burst/single传输的第一次传输
    SEQ = 0b11,    // burst传输中的剩余部分
}; // HTRANS[1:0]

const sc_uint<BW> DMA_ADDR_START = 0x0000'0000;
const sc_uint<BW> DMA_ADDR_END = 0x0000'FFFF;
const sc_uint<BW> RAM_ADDR_START = 0x0001'0000;
const sc_uint<BW> RAM_ADDR_END = 0x0001'FFFF;
