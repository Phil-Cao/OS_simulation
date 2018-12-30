#include "pcbmanager.h"
#include <iostream>
#include <QDebug>
#include <QMutex>

//QMutex mutex;

PCBManager::PCBManager(QObject *parent) : QThread(parent)
{
    this->runningPCB = nullptr;
    this->cpuWidget = nullptr;
    this->runningState = 0;
    this->timer = new QTimer(this);
    connect(this->timer, SIGNAL(timeout()), this, SLOT(update()));
    this->timeCount = 0;
    this->deleteNum = 0;
    this->hangNum = 0;
    //    this->hangtime = 0;
    this->solveHangNum = 0;
    this->usedSize = 0;
}

PCBManager *PCBManager::getInstance()
{
    static PCBManager *manager;
    if (!manager)
        manager = new PCBManager();
    return manager;
}

void PCBManager::addPCB(PCB *pcb)
{
    int totalSize = this->usedSize + pcb->PCBsize;
    if(readyLine.size()<11 && totalSize <= 480){
        this->clearList();
        this->getList();
        int start = this->getStartPostion(pcb);
        pcb->startPosition = start;
        this->readyLine.push_back(pcb);
        this->usedSize = totalSize;
    }else{
        pcb->state = 2;
        this->reserveLine.push_back(pcb);
    }
    this->cpuWidget->refreshData();
    // 刷新ramUI

    this->getList();
    this->ramWidget->refreshRAMUI();

}


void PCBManager::freshUI()
{
    // 更新就绪队列
    int count = this->readyLine.size();
    for (int i = 0; i < 11; ++i){
        if(i < count){
            this->cpuWidget->readyLine[i]->num = this->readyLine[i]->PID;
        }else{
            this->cpuWidget->readyLine[i]->num = -1;
        }
    }
    // 更新后备队列
    count = this->reserveLine.size();
    for (int i = 0; i < 11; ++i){
        if(i < count){
            this->cpuWidget->reserveLine[i]->num = this->reserveLine[i]->PID;
        }else{
            this->cpuWidget->reserveLine[i]->num = -1;
        }
    }
    // 更新挂起队列
    count = this->hangLine.size();
    for (int i = 0; i < 11; ++i){
        if(i < count){
            this->cpuWidget->hangLine[i]->num = this->hangLine[i]->PID;
        }else{
            this->cpuWidget->hangLine[i]->num = -1;
        }
    }
    // 更新完成队列
    count = this->finishLine.size();
    for (int i = 0; i < 11; ++i){
        if(i < count){
            this->cpuWidget->finishLine[i]->num = this->finishLine[i]->PID;
        }else{
            this->cpuWidget->finishLine[i]->num = -1;
        }
    }
    // 刷新UI
    this->clearList();
    this->getList();
    emit freshRAM();
}

void PCBManager::clearAll()
{

    for(int i = 0; i < this->readyLine.size(); ++i){
        delete this->readyLine[i];
    }
    for(int i = 0; i < this->reserveLine.size(); ++i){
        delete this->reserveLine[i];
    }
    for(int i = 0; i < this->hangLine.size(); ++i){
        delete this->hangLine[i];
    }
    for(int i = 0; i < this->finishLine.size(); ++i){
        delete this->finishLine[i];
    }
    this->readyLine.clear();
    this->reserveLine.clear();
    this->hangLine.clear();
    this->finishLine.clear();
    this->usedSize = 0;
    this->timeCount = 0;
    this->timer->stop();
    this->cpuWidget->refreshData();
}


void PCBManager::run()
{
    if(this->runningState == 1){
        // 非抢占式
        // 就绪队列里面有进程就一直进行
        while (this->readyLine.size() !=0||this->reserveLine.size()!=0||this->runningPCB != nullptr) {
            // 将后备队列中的进程放入就绪队列
            if(this->readyLine.size()<11&&this->reserveLine.size()>0){
                for(int i = 0; i < this->reserveLine.size(); ++i){
                    PCB *pcb = this->reserveLine[i];

                    this->clearList();
                    this->getList();
                    int start = this->getStartPostion(pcb);
                    pcb->startPosition = start;

                    int totalSize = this->usedSize + pcb->PCBsize;
                    if(totalSize <= 480){
                        pcb->startTime = this->timeCount;
                        this->readyLine.push_back(pcb);
                        this->reserveLine.erase(reserveLine.begin());
                        this->usedSize = totalSize;
                        break;
                    }
                }

            }
            if(this->runningPCB == nullptr){
                // 选出优先级最高的进程
                int min_priority = this->readyLine[0]->priority;
                int index = 0;
                for(int i = 1; i < this->readyLine.size(); ++i){
                    if(min_priority > this->readyLine[i]->priority){
                        min_priority = this->readyLine[i]->priority;
                        index = i;
                    }
                }
                this->runningPCB = this->readyLine[index];
                readyLine.erase(readyLine.begin()+index);
                this->runningPCB->state = 5;
            }
            // 将后备队列中的进程放入就绪队列
            if(this->readyLine.size()<11&&this->reserveLine.size()>0){
                for(int i = 0; i < this->reserveLine.size(); ++i){
                    PCB *pcb = this->reserveLine[i];

                    this->clearList();
                    this->getList();
                    int start = this->getStartPostion(pcb);
                    pcb->startPosition = start;

                    int totalSize = this->usedSize + pcb->PCBsize;
                    if(totalSize <= 480){
                        pcb->startTime = this->timeCount;
                        this->readyLine.push_back(pcb);
                        this->reserveLine.erase(reserveLine.begin());
                        this->usedSize = totalSize;
                        break;
                    }
                }

            }
            this->freshUI();

            while(this->runningPCB != nullptr && this->runningPCB->needtime > this->runningPCB->cputime){
                sleep(1);
                this->runningPCB->cputime++;

                for(int i = 0; i < this->readyLine.size(); ++i){
                    this->readyLine[i]->priority--;
                }
                if(this->stop == 1)
                {
                    this->stop = 0;
                    return;
                }
                if(this->deleteNum != 0){
                    this->deletePCB();
                }
                if(this->hangNum != 0){
                    this->hangPCB();
                }
                if(this->solveHangNum != 0){
                    int index = -1;
                    for(int i = 0; i < this->hangLine.size(); ++i){
                        if(this->hangLine[i]->PID == this->solveHangNum){
                            index = i;
                            break;
                        }
                    }
                    if(index != -1){
                        int totalSize = this->usedSize + this->hangLine[index]->PCBsize;
                        if(this->readyLine.size()<11&&totalSize<=480){
                            this->clearList();
                            this->getList();
                            int start = this->getStartPostion(this->hangLine[index]);
                            this->hangLine[index]->startPosition = start;
                            this->usedSize = totalSize;
                            this->readyLine.push_back(this->hangLine[index]);
                            this->hangLine.erase(this->hangLine.begin()+index);

                        }else{
                            // 放入后备队列
                            this->reserveLine.insert(this->reserveLine.begin(),this->hangLine[index]);
                            this->hangLine.erase(this->hangLine.begin()+index);
                        }
                    }
                    this->solveHangNum = 0;
                }
                //                for(int i = 0; i < this->hangLine.size(); ++i){
                //                    this->hangLine[i]->hangTime--;
                //                }
                this->freshUI();
            }
            if(this->runningPCB != nullptr){
                this->runningPCB->state = 4;
                this->runningPCB->overTime = this->timeCount;
                this->usedSize = this->usedSize - this->runningPCB->PCBsize;
                this->finishLine.push_back(this->runningPCB);
                this->runningPCB = nullptr;
            }
        }
    }else{
        // 抢占式
        // 就绪队列里面有进程就一直进行
        while (this->readyLine.size() !=0||this->reserveLine.size()!=0||this->runningPCB!=nullptr) {
            // 将后备队列中的进程放入就绪队列
            if(this->readyLine.size()<11&&this->reserveLine.size()>0){
                for(int i = 0; i < this->reserveLine.size(); ++i){
                    PCB *pcb = this->reserveLine[i];

                    this->clearList();
                    this->getList();
                    int start = this->getStartPostion(pcb);
                    pcb->startPosition = start;

                    int totalSize = this->usedSize + pcb->PCBsize;
                    if(totalSize <= 480){
                        pcb->startTime = this->timeCount;
                        this->readyLine.push_back(pcb);
                        this->reserveLine.erase(reserveLine.begin());
                        this->usedSize = totalSize;
                        break;
                    }
                }
            }
            if(this->runningPCB == nullptr){
                // 选出优先级最高的进程
                int min_priority = this->readyLine[0]->priority;
                int index = 0;
                for(int i = 1; i < this->readyLine.size(); ++i){
                    if(min_priority > this->readyLine[i]->priority){
                        min_priority = this->readyLine[i]->priority;
                        index = i;
                    }
                }
                this->runningPCB = this->readyLine[index];
                readyLine.erase(readyLine.begin()+index);
                this->runningPCB->state = 5;
            }
            // 将后备队列中的进程放入就绪队列
            if(this->readyLine.size()<11&&this->reserveLine.size()>0){
                for(int i = 0; i < this->reserveLine.size(); ++i){
                    PCB *pcb = this->reserveLine[i];

                    this->clearList();
                    this->getList();
                    int start = this->getStartPostion(pcb);
                    pcb->startPosition = start;

                    int totalSize = this->usedSize + pcb->PCBsize;
                    if(totalSize <= 480){
                        pcb->startTime = this->timeCount;
                        this->readyLine.push_back(pcb);
                        this->reserveLine.erase(reserveLine.begin());
                        this->usedSize = totalSize;
                        break;
                    }
                }

            }

            this->freshUI();

            while(this->runningPCB != nullptr && this->runningPCB->needtime > this->runningPCB->cputime){
                sleep(1);
                this->runningPCB->cputime++;
                for(int i = 0; i < this->readyLine.size(); ++i){
                    this->readyLine[i]->priority--;
                }
                // 将后备队列中的进程放入就绪队列
                if(this->readyLine.size()<11&&this->reserveLine.size()>0){
                    for(int i = 0; i < this->reserveLine.size(); ++i){
                        PCB *pcb = this->reserveLine[i];

                        this->clearList();
                        this->getList();
                        int start = this->getStartPostion(pcb);
                        pcb->startPosition = start;

                        int totalSize = this->usedSize + pcb->PCBsize;
                        if(totalSize <= 480){
                            pcb->startTime = this->timeCount;
                            this->readyLine.push_back(pcb);
                            this->reserveLine.erase(reserveLine.begin());
                            this->usedSize = totalSize;
                            break;
                        }
                    }
                }
                if(this->runningPCB->needtime == this->runningPCB->cputime){
                    this->runningPCB->state = 4;
                    this->runningPCB->overTime = this->timeCount;
                    this->usedSize = this->usedSize - this->runningPCB->PCBsize;
                    this->finishLine.push_back(this->runningPCB);
                    this->runningPCB = nullptr;
                    this->freshUI();
                    break;
                }
                // 查看是否有优先级更高的进程
                for(int i = 0; i < this->readyLine.size(); ++i){
                    if(this->readyLine[i]->priority < this->runningPCB->priority){
                        PCB *tempPCB = this->runningPCB;
                        this->runningPCB = this->readyLine[i];
                        this->readyLine.erase(this->readyLine.begin()+i);
                        this->readyLine.insert(this->readyLine.begin(), tempPCB);
                    }
                }

                if(this->deleteNum != 0){
                    this->deletePCB();
                }
                if(this->hangNum != 0){
                    this->hangPCB();
                }
                if(this->solveHangNum != 0){
                    int index = -1;
                    for(int i = 0; i < this->hangLine.size(); ++i){
                        if(this->hangLine[i]->PID == this->solveHangNum){
                            index = i;
                            break;
                        }
                    }
                    if(index != -1){
                        int totalSize = this->usedSize + this->hangLine[index]->PCBsize;
                        if(this->readyLine.size()<11&&totalSize<=480){
                            this->clearList();
                            this->getList();
                            int start = this->getStartPostion(this->hangLine[index]);
                            this->hangLine[index]->startPosition = start;
                            this->usedSize = totalSize;
                            this->readyLine.push_back(this->hangLine[index]);
                            this->hangLine.erase(this->hangLine.begin()+index);

                        }else{
                            // 放入后备队列
                            this->reserveLine.insert(this->reserveLine.begin(),this->hangLine[index]);
                            this->hangLine.erase(this->hangLine.begin()+index);
                        }
                    }
                    this->solveHangNum = 0;
                }
//                if(this->runningPCB != nullptr&&this->runningPCB->cputime == this->runningPCB->needtime){
//                    this->runningPCB->state = 4;
//                    this->runningPCB->overTime = this->timeCount;
//                    this->usedSize = this->usedSize - this->runningPCB->PCBsize;
//                    this->finishLine.push_back(this->runningPCB);
//                    this->runningPCB = nullptr;
//                }
                this->freshUI();
            }

        }
    }


    this->runningPCB = nullptr;
    this->freshUI();
}

void PCBManager::clearList()
{
//    mutex.lock();
    for(int i = 0; i < this->originInfoList.size(); ++i){
        delete this->originInfoList[i];
    }
    for(int i = 0 ; i < this->positionInfoList.size(); ++i){
        delete this->positionInfoList[i];
    }
    this->originInfoList.clear();
    this->positionInfoList.clear();
//    mutex.unlock();
}
// 内存分配算法
int PCBManager::getStartPostion(PCB *pcb)
{
    //    mutex.lock();
    // 获取空闲位置的信息
    if(this->originInfoList.size() != 0){
        int length = originInfoList[0]->start;

        ProcessMemoryInfo *info = new ProcessMemoryInfo(1, length, 0, 0);

        this->positionInfoList.push_back(info);
        int start = originInfoList[originInfoList.size()-1]->end+1;
        length = 480- start;
        //        int PID2 = originInfoList[originInfoList.size()-1]->PID;
        ProcessMemoryInfo *info2 = new ProcessMemoryInfo(start, length, 0, 0);
        this->positionInfoList.push_back(info2);
    }
    int j = 0;
    int size = this->originInfoList.size()-1;
    while(j < size){
        int start = originInfoList[j]->end + 1;
        int length = originInfoList[j+1]->start - start;
        //        int PID = originInfoList[i]->PID;
        ProcessMemoryInfo *info = new ProcessMemoryInfo(start, length, 0, 0);
        this->positionInfoList.push_back(info);
        j++;
    }

    int startP = 0;
    // 如果数组里面没有
    if(this->positionInfoList.size() == 0){
        return 1;
        // 如果数组里面有
    }else{
        // 找最适应
        int minsize = 480;
        for(int i = 0; i < this->positionInfoList.size(); ++i){
            int difference = this->positionInfoList[i]->length - pcb->PCBsize;
            if(difference >= 0 && difference < minsize){
                minsize = difference;
                startP = this->positionInfoList[i]->start;
            } // if
        } // for
    }

    //    mutex.unlock();
    return startP;

}

void PCBManager::getList()
{
//    mutex.lock();
    // 获取原始的内存信息，有小到大排列
    if(this->runningPCB != nullptr){
        int startPosition = this->runningPCB->startPosition;
        int length = this->runningPCB->PCBsize;
        int end = startPosition + length-1;
        int id = this->runningPCB->PID;
        ProcessMemoryInfo *memoryInfo = new ProcessMemoryInfo(startPosition, length, end, id);
        this->originInfoList.push_back(memoryInfo);
    }
    int i = 0;
    int size = this->readyLine.size();
    while(i < size){
        int startPosition = this->readyLine[i]->startPosition;
        int length = this->readyLine[i]->PCBsize;
        int end = startPosition + length-1;
        int id = this->readyLine[i]->PID;
        ProcessMemoryInfo *memoryInfo = new ProcessMemoryInfo(startPosition, length, end, id);
        // 如果数组中还没有的话
        if(this->originInfoList.size() == 0)
            this->originInfoList.push_back(memoryInfo);
        else{
            // 找到合适的位置插入
            int k = 0;
            for(k = 0; k < this->originInfoList.size(); ++k){
                if(originInfoList[k]->start > memoryInfo->end){
                    this->originInfoList.insert(this->originInfoList.begin()+k, memoryInfo);
                    break;
                }
            }
            if(k == this->originInfoList.size())
                this->originInfoList.push_back(memoryInfo);
        }
        ++i;
    }
//    mutex.unlock();
}

void PCBManager::deletePCB()
{
    //    mutex.lock();
    // 如果在运行中
    if(this->runningPCB->PID == this->deleteNum){
        delete this->runningPCB;
        this->runningPCB = nullptr;
    }else{
        // 如果在就绪队列中
        int count = this->readyLine.size();
        for(int i = 0; i < count; ++i){
            if(this->readyLine[i]->PID == this->deleteNum){
                delete this->readyLine[i];
                this->readyLine.erase(this->readyLine.begin()+i);
                this->deleteNum = 0;
//                mutex.unlock();
                return;
            }
        }
        // 如果在后备队列中
        count = this->reserveLine.size();
        for(int i = 0; i < count; ++i){
            if(this->reserveLine[i]->PID == this->deleteNum){
                delete this->reserveLine[i];
                this->reserveLine.erase(this->reserveLine.begin()+i);
                this->deleteNum = 0;
//                mutex.unlock();
                return;
            }
        }

        // 如果在挂起队列中
        count = this->hangLine.size();
        for(int i = 0; i < count; ++i){
            if(this->hangLine[i]->PID == this->deleteNum){
                delete this->hangLine[i];
                this->hangLine.erase(this->hangLine.begin()+i);
                this->deleteNum = 0;
//                mutex.unlock();
                return;
            }
        }
        // 如果在完成队列中
        count = this->finishLine.size();
        for(int i = 0; i < count; ++i){
            if(this->finishLine[i]->PID == this->deleteNum){
                delete this->finishLine[i];
                this->finishLine.erase(this->finishLine.begin()+i);
                this->deleteNum = 0;
//                mutex.unlock();
                return;
            }
        }
    }
    this->deleteNum = 0;
    //    mutex.unlock();
}

void PCBManager::hangPCB()
{
    //    mutex.lock();
    // 如果在运行中
    if(this->runningPCB->PID == this->hangNum){
        this->hangLine.push_back(runningPCB);
        //        this->runningPCB->hangTime = this->hangtime;
        this->runningPCB = nullptr;
        this->hangNum = 0;
        this->usedSize -= this->runningPCB->PCBsize;
    }else{
        // 如果在就绪队列中
        int count = this->readyLine.size();
        for(int i = 0; i < count; ++i){
            if(this->readyLine[i]->PID == this->hangNum){
                //                this->readyLine[i]->hangTime = this->hangtime;
                this->hangLine.push_back(this->readyLine[i]);
                this->readyLine.erase(this->readyLine.begin()+i);
                this->hangNum = 0;
                this->usedSize -= this->runningPCB->PCBsize;
                //                this->hangtime = 0;
                //                mutex.unlock();
                return;
            }
        }
    }
}

void PCBManager::update()
{
    this->timeCount++;
    //    QString s = QString::number(this->time_count,10);
    //    qDebug()<<s<<" "<<endl;
    //    if(time_count == 17){
    //        this->timer->stop();
    //    }
}

