#include "mainwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include "cpuwidget.h"
#include "pcbmanager.h"
#include "qestiondialog.h"
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <iostream>
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle(tr("处理机调度和内存分配模拟"));
    this->resize(1200,600);

    // 添加处理器模块
    this->cpu_widget = new CpuWidget(this);
    //this->setCentralWidget(cpu_widget);
    this->cpu_widget->setGeometry(0,25,900,600);
    PCBManager *manager = PCBManager::getInstance();
    manager->cpuWidget = this->cpu_widget;
//    cout<<this->cpu_widget->frameGeometry().height()<<endl;
    // 添加主存模块
    this->ram_widget = new RAMWidget(this);
    this->ram_widget->setGeometry(900,25,300,600);
    manager->ramWidget = this->ram_widget;
    connect(manager, SIGNAL(freshRAM()), this, SLOT(mFreshRAMUI()));
    // 设置菜单栏
    this->menu_bar = new QMenuBar(this);
    //this->menu_bar->resize(1200,25);
    //定义菜单项组
    this->menu[0] = new QMenu("进程");
    this->menu[1] = new QMenu("调度算法");
    this->menu[2] = new QMenu("控制");
    //添加进程
    this->act_part1[0] = new QAction("添加进程",this);
    this->menu[0]->addAction(this->act_part1[0]);
    //删除进程
    this->act_part1[1] = new QAction("删除进程",this);
    this->menu[0]->addAction(this->act_part1[1]);
    //挂起进程
    this->act_part1[2] = new QAction("挂起进程",this);
    this->menu[0]->addAction(this->act_part1[2]);
    // 解挂进程
    this->act_part1[3] = new QAction("解挂进程",this);
    this->menu[0]->addAction(this->act_part1[3]);

    //抢占式优先权
    this->act_part2[0] = new QAction("抢占式优先权",this);
    //非抢占式优先权
    this->act_part2[1] = new QAction("非抢占式优先权",this);
    this->menu[1]->addAction(this->act_part2[0]);
    this->menu[1]->addAction(this->act_part2[1]);

    //开始
    this->act_part3[0] = new QAction("开始执行",this);
    this->act_part3[1] = new QAction("重新开始",this);
//    this->act_part3[3] = new QAction("暂停执行",this);
    this->menu[2]->addAction(this->act_part3[0]);
    this->menu[2]->addAction(this->act_part3[1]);
//    this->menu[2]->addAction(this->act_part3[3]);

    //将菜单项组加入菜单栏
    this->menu_bar->addMenu(this->menu[0]);
    this->menu_bar->addMenu(this->menu[1]);
    this->menu_bar->addMenu(this->menu[2]);
    this->menu_bar->setGeometry(0,0,this->width(),30);

    connect(this->menu_bar,SIGNAL(triggered(QAction*)),this,SLOT(showAddDialog(QAction*)));

}

MainWindow::~MainWindow()
{

}
void MainWindow::showAddDialog(QAction* act)
{
    // 添加
    if(act == this->act_part1[0]){
        QestionDialog qestionDialog(this);
        qestionDialog.exec();
        //int start_time = qestionDialog.start_time_line->text().toInt();
        int priority = qestionDialog.priority_line->text().toInt();
        int need_time = qestionDialog.needtime_line->text().toInt();
        int need_space = qestionDialog.need_space_line->text().toInt();
        //cout<<start_time<<" "<<priority<<" "<<cpu_time<<" "<<need_time<<endl;
        //PCBManager::addPCB();
        PCB *pcb = new PCB(priority, need_time, this->count++, need_space);
        PCBManager::getInstance()->addPCB(pcb);
    }
    // 删除
    else if(act == this->act_part1[1]){
        this->deleteDialog = new QDialog(this);
        deleteDialog->setWindowTitle("删除进程");
        deleteDialog->setModal(true);
        QLabel *label = new QLabel(deleteDialog);
        label->setText("删除进程编号:");
        this->deleteLineEdit = new QLineEdit(deleteDialog);
        QHBoxLayout *layout = new QHBoxLayout;
        layout->addWidget(label);
        layout->addWidget(this->deleteLineEdit);

        QPushButton *confirmButton = new QPushButton(this);
        confirmButton->setText("确定");
        connect(confirmButton, SIGNAL(clicked()),this, SLOT(deleteConfirm()));
        QVBoxLayout *vlayout = new QVBoxLayout;
        vlayout->addLayout(layout);
        vlayout->addWidget(confirmButton);

        deleteDialog->setLayout(vlayout); 
        deleteDialog->show();
    }
    // 挂起
    else if(act == this->act_part1[2]){
        this->hangDialog = new QDialog(this);
        this->hangDialog->setWindowTitle("挂起进程");
        this->hangDialog->setModal(true);
        QLabel *label1 = new QLabel(this->hangDialog);
        label1->setText("挂起进程编号:");
        this->hangNum = new QLineEdit(this->hangDialog);
//        QLabel *label2 = new QLabel(this->hangDialog);
//        label2->setText("挂起进程时间:");
//        this->hangTime = new QLineEdit(this->hangDialog);

        QPushButton *confirmButton = new QPushButton(this);
        confirmButton->setText("确定");
        connect(confirmButton, SIGNAL(clicked()), this, SLOT(hangConfirm()));
        QGridLayout *layout = new QGridLayout;
        layout->addWidget(label1, 0, 0, 1,1);
        layout->addWidget(this->hangNum,0,1,1,1);
//        layout->addWidget(label2, 1,0,1,1);
//        layout->addWidget(this->hangTime,1,1,1,1);
        layout->addWidget(confirmButton,1,0,1,2);

        this->hangDialog->setLayout(layout);
        this->hangDialog->show();
    }
    // 解挂
    else if(act == this->act_part1[3]){
        this->solveHangDialog = new QDialog(this);
        this->solveHangDialog->setWindowTitle("解挂进程");
        this->solveHangDialog->setModal(true);
        QLabel *label = new QLabel(this->solveHangDialog);
        label->setText("解挂进程编号:");
        this->solveHangEdit = new QLineEdit(this->solveHangDialog);
        QPushButton *confirmButton = new QPushButton(this->solveHangDialog);
        confirmButton->setText("确定");
        connect(confirmButton, SIGNAL(clicked()), this, SLOT(solveHangConfirm()));
        QGridLayout *layout = new QGridLayout;
        layout->addWidget(label, 0,0,1,1);
        layout->addWidget(this->solveHangEdit, 0,1,1,1);
        layout->addWidget(confirmButton, 1,0,1,2);
        this->solveHangDialog->setLayout(layout);
        this->solveHangDialog->show();
    }
    // 抢占式优先权
    else if(act == this->act_part2[0]){
        PCBManager *manager = PCBManager::getInstance();
        manager->runningState = 0;
    }
    // 非抢占式优先权
    else if(act == this->act_part2[1]){
        PCBManager *manager = PCBManager::getInstance();
        manager->runningState = 1;
    }
    // 开始
    else if(act == this->act_part3[0]){
        PCBManager *manager = PCBManager::getInstance();
        manager->timer->start(1000);
        manager->start();
    }
    // 暂停
    else if(act == this->act_part3[1]){
        PCBManager::getInstance()->stop = 1;
        PCBManager::getInstance()->timer->stop();
    }else if(act == this->act_part3[2]){
        PCBManager::getInstance()->clearAll();
        this->count = 1;
    }

}

void MainWindow::deleteConfirm()
{
    int deleteNum = this->deleteLineEdit->text().toInt();
    PCBManager::getInstance()->deleteNum = deleteNum;
    this->deleteDialog->accept();

}

void MainWindow::hangConfirm()
{
    int hangNumber = this->hangNum->text().toInt();
//    int hangTime = this->hangTime->text().toInt();
    PCBManager::getInstance()->hangNum = hangNumber;
//    PCBManager::getInstance()->hangtime = hangTime;
    this->hangDialog->accept();
}

void MainWindow::solveHangConfirm()
{
    int solveHangNum = this->solveHangEdit->text().toInt();
    PCBManager::getInstance()->solveHangNum = solveHangNum;
    this->solveHangDialog->accept();
}

void MainWindow::mFreshRAMUI()
{
    this->cpu_widget->refreshData();
    // 刷新ramUI

    this->ram_widget->refreshRAMUI();

}
