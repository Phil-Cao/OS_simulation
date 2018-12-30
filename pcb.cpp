#include "pcb.h"
#include "pcbmanager.h"

//int PID; // 进程名
//int needtime; // 要求运行时间
//int cputime;  // 已经运行时间
//int priority;  // 优先权(越小越好)
//int start_time; // 进入就绪队列时间
//int over_time;  // 运行结束时间
//int state;     // 状态：1就绪 2后备 3挂起 4完成 5运行


PCB::PCB(int priority, int needtime, int PID, int PCBsize)
{
    this->needtime = needtime;
    this->priority = priority;
    this->startTime = 0;
    this->PID = PID;
    this->cputime = 0;
    this->overTime = 0;
    this->state = 1;
    this->hangTime = 0;
    this->PCBsize = PCBsize;
    this->r = qrand()%255;
    this->g = qrand()%255;
    this->b = qrand()%255;
}
