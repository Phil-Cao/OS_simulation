#include "cpuwidget.h"
#include <QPushButton>
#include "pcbmanager.h"
#include <iostream>
#include "sprocess.h"
#include <QFont>
#include <QLabel>
#include <QPalette>



CpuWidget::CpuWidget(QWidget *parent) : QWidget(parent)
{
//    QPalette pal;
//    pal.setColor(QPalette::Background, Qt::yellow);
//    this->setAutoFillBackground(true);
//    this->setPalette(pal);
    this->initData();





}

void CpuWidget::refreshData()
{
    int count = PCBManager::getInstance()->readyLine.size();
//    int t = this->readyLine.size();
//    printf("%d",t);
    for(int i = 0; i < 11; i++){
        SProcess *process = this->readyLine[i];
        if(i < count){
            PCB *pcb = PCBManager::getInstance()->readyLine[i];
            process->num = pcb->PID;

            QString str = QString::number(process->num, 10);
            process->setText(str);
            process->num = pcb->PID;
            process->start_time = pcb->startTime;
            process->priority = pcb->priority;
            process->cpu_time = pcb->cputime;
            process->needtime = pcb->needtime;
            process->overtime = pcb->overTime;
            process->r = pcb->r;
            process->g = pcb->g;
            process->b = pcb->b;
            this->setUsingState(process);
        }else{
            this->setNotUsingState(process);
            process->num = -1;
            process->setText(" ");
        }
    }
    count = PCBManager::getInstance()->reserveLine.size();
    for(int i = 0; i < 11; ++i){
        SProcess *process = this->reserveLine[i];
        if(i < count){
            PCB *pcb = PCBManager::getInstance()->reserveLine[i];
            process->num = pcb->PID;

            QString str = QString::number(process->num, 10);
            process->setText(str);
            process->num = pcb->PID;
            process->start_time = pcb->startTime;
            process->priority = pcb->priority;
            process->cpu_time = pcb->cputime;
            process->needtime = pcb->needtime;
            process->overtime = pcb->overTime;
            process->r = pcb->r;
            process->g = pcb->g;
            process->b = pcb->b;
            this->setUsingState(process);
        }else{
            this->setNotUsingState(process);
            process->num = -1;
            process->setText(" ");
        }
    }
    count = PCBManager::getInstance()->hangLine.size();
    for(int i = 0; i < 11; ++i){
        SProcess *process = this->hangLine[i];
        if(i < count){
            PCB *pcb = PCBManager::getInstance()->hangLine[i];
            process->num = pcb->PID;
            QString str = QString::number(process->num, 10);
            process->setText(str);
            process->num = pcb->PID;
            process->start_time = pcb->startTime;
            process->priority = pcb->priority;
            process->cpu_time = pcb->cputime;
            process->needtime = pcb->needtime;
            process->overtime = pcb->overTime;
            process->r = pcb->r;
            process->g = pcb->g;
            process->b = pcb->b;
            this->setUsingState(process);
        }else{
            this->setNotUsingState(process);
            process->num = -1;
            process->setText(" ");
        }

    }
    count = PCBManager::getInstance()->finishLine.size();
    for(int i = 0; i < 22; ++i){
        SProcess *process = this->finishLine[i];
        if(i < count){
            PCB *pcb = PCBManager::getInstance()->finishLine[i];
            process->num = pcb->PID;
            QString str = QString::number(process->num, 10);
            process->setText(str);
            process->num = pcb->PID;
            process->start_time = pcb->startTime;
            process->priority = pcb->priority;
            process->cpu_time = pcb->cputime;
            process->needtime = pcb->needtime;
            process->overtime = pcb->overTime;
            process->r = pcb->r;
            process->g = pcb->g;
            process->b = pcb->b;
            this->setUsingState(process);
        }else{
            this->setNotUsingState(process);
            process->num = -1;
            process->setText(" ");
        }
    }

    PCB *runningPCB = PCBManager::getInstance()->runningPCB;
    if(runningPCB == nullptr){
        this->setNotUsingState(this->runningProcess);
        this->runningProcess->setText("无运行进程");
        this->runningProcess->num = -1;
    }else{
        this->runningProcess->num = runningPCB->PID;  
        QString str = QString::number(this->runningProcess->num, 10);
        QString strall = "进程 "+str+" 正在运行";
        this->runningProcess->setText(strall);
        this->runningProcess->start_time = runningPCB->startTime;
        this->runningProcess->priority = runningPCB->priority;
        this->runningProcess->cpu_time = runningPCB->cputime;
        this->runningProcess->needtime = runningPCB->needtime;
        this->runningProcess->overtime = runningPCB->overTime;
        this->runningProcess->r = runningPCB->r;
        this->runningProcess->g = runningPCB->g;
        this->runningProcess->b = runningPCB->b;
        this->setUsingState(this->runningProcess);
    }
//    PCBManager::getInstance()->clearList();
//    PCBManager::getInstance()->getList();
//    PCBManager::getInstance()->ramWidget->refreshRAMUI();

}

void CpuWidget::initData()
{
    QLabel *readyLabel = new QLabel(this);
    QLabel *reserveLabel = new QLabel(this);
    QLabel *hangLabel = new QLabel(this);
    QLabel *finishLabel = new QLabel(this);
    readyLabel->setFont(QFont("Microsoft YaHei", 12, 55));
    reserveLabel->setFont(QFont("Microsoft YaHei", 12, 55));
    hangLabel->setFont(QFont("Microsoft YaHei", 12, 55));
    finishLabel->setFont(QFont("Microsoft YaHei", 12, 55));
    readyLabel->setText("就绪队列");
    reserveLabel->setText("后备队列");
    hangLabel->setText("挂起队列");
    finishLabel->setText("完成队列");

    this->runningProcess = new SProcess(this);
    this->runningProcess->setText("无运行程序");
    this->runningProcess->setGeometry(380,70,150,59);

    int i = 0;
    for(int j = 0; j < 11; ++j){
        SProcess *s = new SProcess(this);
        s->setGeometry(j*60+180,i*60+160,59,59);
        s->num = -1;
        this->readyLine.push_back(s);

    }
    readyLabel->setGeometry(50,i*60+160,100,59);
    i++;
    for(int j = 0; j < 11; ++j){
        SProcess *s = new SProcess(this);
        s->setGeometry(j*60+180, i*60+162,59,59);
        s->num = -1;
        this->reserveLine.push_back(s);
    }
    reserveLabel->setGeometry(50,i*60+162,100,59);
    i++;
    for(int j = 0; j < 11; ++j){
        SProcess *s = new SProcess(this);
        s->setGeometry(j*60+180, i*60+164,59,59);
        s->num = -1;
        this->hangLine.push_back(s);
    }
    hangLabel->setGeometry(50,i*60+164,100,59);
    i++;
    for(int j = 0; j < 11; ++j){
        SProcess *s = new SProcess(this);
        s->setGeometry(j*60+180, i*60+166,59,59);
        s->num = -1;
        this->finishLine.push_back(s);
    }
    finishLabel->setGeometry(50,i*60+166,100,59);
    i++;
    for(int j = 0; j < 11; ++j){
        SProcess *s = new SProcess(this);
        s->setGeometry(j*60+180, i*60+168,59,59);
        s->num = -1;
        this->finishLine.push_back(s);
    }

}

void CpuWidget::setUsingState(SProcess* &s)
{
    QString r_str = QString::number(s->r,10);
    QString g_str = QString::number(s->g,10);
    QString b_str = QString::number(s->b,10);
    QString str2 = "QPushButton {background-color:rgb("+r_str+","+g_str+","+b_str+");}";
    s->setStyleSheet(str2);

}

void CpuWidget::setNotUsingState(SProcess* &s)
{
    s->setStyleSheet("");
}

