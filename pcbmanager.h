#ifndef PCBMANAGER_H
#define PCBMANAGER_H

#include <QObject>
#include "pcb.h"
#include <vector>
#include "cpuwidget.h"
#include <QThread>
#include <QTimer>
#include "processmemoryinfo.h"
#include "ramwidget.h"


class PCBManager : public QThread
{
    Q_OBJECT
private:
    PCBManager(QObject *parent = nullptr);
public:
    static PCBManager *getInstance();
    void addPCB(PCB* pcb);
    void deletePCB();
    void hangPCB();
    void freshUI();
    void clearAll();
    void run();
    void clearList();
    int getStartPostion(PCB *pcb);
    void getList();
public:
    std::vector<PCB*> readyLine; // 就绪队列
    std::vector<PCB*> reserveLine; // 后备队列
    std::vector<PCB*> hangLine;  // 挂起队列
    std::vector<PCB*> finishLine;  // 完成队列
    int runningState;  // 为0是抢占式，为1是非抢占式
    CpuWidget *cpuWidget;
    RAMWidget *ramWidget;

    PCB *runningPCB;  // 正在执行的PCB
    QTimer *timer;    // 计时器
    int timeCount;    // 总时间
    int deleteNum;    // 删除进程编号
    int hangNum;      // 挂起教程编号
    int solveHangNum;  // 解挂进程编号
    int stop = 0;     // 系统是否暂停
    int usedSize;     // 已使用主存大小
    std::vector<ProcessMemoryInfo*> originInfoList;   // 根据就绪队列和运行中的进程获取内存分配的原始信息
    std::vector<ProcessMemoryInfo*> positionInfoList;  // 计算空闲分区的信息
signals:
    void freshRAM();
    void deleteOriginList();
public slots:
    void update();

};

#endif // PCBMANAGER_H
