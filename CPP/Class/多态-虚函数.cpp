/* 使用多态模拟电脑组装运行 */

// 大致思想：
//     1.将零件创建为虚基类（CPU,显卡，内存条）
//     2.computer类属性为其组件（CPU,显卡，内存条）
//     3.封装厂商：不同零件生产厂商继承虚基类，并产生不同效果
//     4.将厂商生产的零件组装为完整的电脑
//     5.运行
//     6.最后释放内存

#include <iostream>
using namespace std;
// 抽象类 （零件）
class CPU {
   public:
    virtual void Calculate() = 0;
};

class VideoCard {  // 显卡
   public:
    virtual void Show() = 0;
};

class Memory {  // 内存条
   public:
    virtual void Storage() = 0;
};

class Computer {  // 计算机
   public:
    Computer(CPU* cpu, VideoCard* vc, Memory* mem)
        : m_cpu(cpu), m_vc(vc), m_mem(mem) {}  // 初始化列表

    // Computer(CPU* cpu, VideoCard* vc, Memory* mem) {
    //     m_cpu = cpu;
    //     m_vc = vc;
    //     m_mem = mem;
    // }
    void work() {  // 模拟工作时组件运行
        m_cpu->Calculate();
        m_vc->Show();
        m_mem->Storage();
    }
    ~Computer() {  // 释放属性指针在堆区开辟的空间
        if (m_cpu != NULL) {
            delete m_cpu;
            cout << "释放m_cpu " << endl;
            m_cpu = NULL;
        }
        if (m_vc != NULL) {
            delete m_vc;
            cout << "释放m_vc " << endl;
            m_vc = NULL;
        }
        if (m_mem != NULL) {
            delete m_mem;
            cout << "释放m_mem " << endl;
            m_mem = NULL;
        }
    }

   private:
    CPU* m_cpu;       // CPU零件
    VideoCard* m_vc;  // 显卡零件
    Memory* m_mem;    // 内存条零件
};
class intelCPU : public CPU {  // intel生产的CPU
   public:
    virtual void Calculate() { cout << "intelCPU开始处理了" << endl; };
};

class intelVideoCard : public VideoCard {  // intel生产的显卡
   public:
    virtual void Show() { cout << "intelVideoCard开始显示了" << endl; };
};
class intelMemory : public Memory {  // intel生产的内存条
   public:
    virtual void Storage() { cout << "intelMemory开始存储了" << endl; };
};
int main() {
    Computer* computer1 =
        new Computer(new intelCPU, new intelVideoCard, new intelMemory);
    computer1->work();
    delete computer1;
    return 0;
}