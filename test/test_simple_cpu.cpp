// test_simple_cpu.cpp
#include <systemc.h>
#include "ahb_master_shell.h"
#include "simple_cpu.h"

//----------------------------------------------------------------------
// Testbench 模块：实例化 SimpleCPU 和 AHBMasterShell，驱动读写操作
//----------------------------------------------------------------------
class SimpleCPUTestbench : public sc_module {
  public:
    // 构造函数，采用初始化列表传递模块名
    SimpleCPUTestbench(sc_module_name name) :
        sc_module(name), HCLK("HCLK", 10, SC_NS) // 定义时钟，周期为 10 ns
    {
        SC_HAS_PROCESS(SimpleCPUTestbench);
        // 实例化 SimpleCPU 模块（假设 DEVICE_ID 为 int 类型，此处传入 0）
        cpu = new SimpleCPU(0);
        // 实例化 AHBMasterShell 模块
        master_shell = new AHBMasterShell("AHBMasterShell");

        // 将 SimpleCPU 绑定到 AHBMasterShell 的接口端口上
        master_shell->port_.bind(*cpu);

        // 将测试平台的信号与 AHBMasterShell 的端口连接
        master_shell->HGRANT(HGRANT);
        master_shell->HREADY(HREADY);
        master_shell->HRESP(HRESP);
        master_shell->HRESET(HRESET);
        master_shell->HCLK(HCLK);
        master_shell->HRDATA(HRDATA);
        master_shell->HBUSREQ(HBUSREQ);
        master_shell->HLOCK(HLOCK);
        master_shell->HTRANS(HTRANS);
        master_shell->HADDR(HADDR);
        master_shell->HWRITE(HWRITE);
        master_shell->HSIZE(HSIZE);
        master_shell->HBURST(HBURST);
        master_shell->HPROT(HPROT);
        master_shell->HWDATA(HWDATA);

        // 初始化部分信号
        HGRANT.write(true); // 总线授权信号为高
        HREADY.write(true); // 总线就绪
        HRESP.write(0);     // 假定 0 表示正常响应
        HRESET.write(true); // 非复位状态
        HRDATA.write(0);

        // 注册测试线程（线程方式）
        SC_THREAD(run_test);
    }

    // 析构函数，清理动态分配的内存
    ~SimpleCPUTestbench() {
        delete cpu;
        delete master_shell;
    }

  private:
    // 信号定义，与 AHBMasterShell 端口对应
    sc_signal<bool> HGRANT;
    sc_signal<bool> HREADY;
    sc_signal<sc_uint<2>> HRESP;
    sc_signal<bool> HRESET;
    sc_clock HCLK; // 时钟信号
    sc_signal<sc_uint<BW>> HRDATA;
    sc_signal<bool> HBUSREQ;
    sc_signal<bool> HLOCK;
    sc_signal<sc_uint<2>> HTRANS;
    sc_signal<sc_uint<BW>> HADDR;
    sc_signal<bool> HWRITE;
    sc_signal<sc_uint<3>> HSIZE;
    sc_signal<sc_uint<3>> HBURST;
    sc_signal<sc_uint<4>> HPROT;
    sc_signal<sc_uint<BW>> HWDATA;

    // 模块指针
    AHBMasterShell* master_shell;
    SimpleCPU* cpu;

    // 测试线程，执行一系列读写测试
    void run_test() {
        // 等待20 ns，确保系统初始化完成
        wait(20, SC_NS);

        // -------------------------------
        // 测试 1：写操作
        // -------------------------------
        sc_uint<BW> test_addr = 0x100;       // 测试写入地址
        sc_uint<BW> write_data = 0xDEADBEEF; // 测试写入数据
        sc_uint<BW> trans_size = 4;          // 传输大小（单位：字节或根据实际定义）

        cout << sc_time_stamp() << "：开始写操作，地址=0x" << std::hex << test_addr << ", 数据=0x" << write_data
             << std::dec << endl;
        bool write_result = cpu->MasterWrite(test_addr, write_data, trans_size);
        if (write_result)
            cout << sc_time_stamp() << "：写操作成功" << endl;
        else
            cout << sc_time_stamp() << "：写操作失败" << endl;

        // 等待一段时间后进行下一步操作
        wait(30, SC_NS);

        // -------------------------------
        // 测试 2：读操作
        // -------------------------------
        sc_uint<BW> read_data;
        cout << sc_time_stamp() << "：开始读操作，地址=0x" << std::hex << test_addr << std::dec << endl;
        bool read_result = cpu->MasterRead(test_addr, read_data, trans_size);
        if (read_result)
            cout << sc_time_stamp() << "：读操作成功，数据=0x" << std::hex << read_data << std::dec << endl;
        else
            cout << sc_time_stamp() << "：读操作失败" << endl;

        // -------------------------------
        // 测试 3：获取 CPU 任务
        // -------------------------------
        MasterTask task = cpu->GetTask();
        cout << sc_time_stamp() << "：获取到的 CPU 任务信息：" << endl;
        cout << "    write: " << task.write << ", inner_addr: 0x" << std::hex << task.inner_addr << ", slave_addr: 0x"
             << task.slave_addr << std::dec << endl;

        // 测试结束，停止仿真
        sc_stop();
    }
};

//
// sc_main：SystemC 仿真入口函数
//
int sc_main(int argc, char* argv[]) {
    Log_Init();
    // 实例化测试平台模块
    SimpleCPUTestbench tb("SimpleCPUTestbench");
    // 启动仿真
    sc_start();
    return 0;
}
