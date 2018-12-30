#ifndef PCB_H
#define PCB_H


class PCB
{
public:
    PCB(int priority, int needtime, int PID, int PCBsize);
    int PID; // 进程名
    int needtime; // 要求运行时间
    int cputime;  // 已经运行时间
    int priority;  // 优先权(越小越好)
    int startTime; // 进入就绪队列时间
    int overTime;  // 运行结束时间
    int state;     // 状态：1就绪 2后备 3挂起 4完成 5运行
    int hangTime;
    int PCBsize;
    int startPosition;
    int r;
    int g;
    int b;
};

#endif // PCB_H
