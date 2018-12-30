#include "sprocess.h"
#include "infodialog.h"
#include "pcbmanager.h"
#include <QPalette>

SProcess::SProcess(QWidget *parent) : QPushButton(parent)
{
    this->setFont(QFont("Microsoft YaHei", 9, 55));
    QPalette pal = this->palette();              //startBtn是我已经定义好的QPushButton对象
    pal.setColor(QPalette::ButtonText, Qt::black);    //设置按钮上的字体颜色，理论上可以，实际上就是可以
    this->setPalette(pal);
}

SProcess::SProcess(QWidget *parent, int start_time, int priority, int cpu_time, int needtime) : QPushButton(parent)
{
    this->start_time = start_time;
    this->priority = priority;
    this->cpu_time = cpu_time;
    this->needtime = needtime;
    this->overtime = 0;

}


void SProcess::mousePressEvent(QMouseEvent *event)
{
    PCBManager::getInstance()->cpuWidget->refreshData();
    if(this->num == -1){
        QDialog dlg(this);
        dlg.setWindowTitle("无进程");
        dlg.resize(300,70);
        dlg.exec();
    }else{
        InfoDialog dlg(this,this->start_time,this->priority,this->cpu_time,this->needtime,this->overtime);
        dlg.exec();
    }


}
