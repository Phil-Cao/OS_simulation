#include "ramwidget.h"
#include <QLabel>
#include "pcbmanager.h"

RAMWidget::RAMWidget(QWidget *parent) : QWidget(parent)
{
//    QPalette pal;
//    pal.setColor(QPalette::Background, Qt::blue);
//    this->setAutoFillBackground(true);
//    this->setPalette(pal);
    QLabel *infoLabel = new QLabel(this);
    infoLabel->setText("512KB");
    infoLabel->setGeometry(125,10,50,20);

    this->frame = new QFrame(this);
    this->frame->setGeometry(10,30,280,514);
    this->frame->setLineWidth(1);
    this->frame->setFrameStyle(QFrame::Box);

    QLabel *systemLabel = new QLabel(this->frame);
    systemLabel->setText(" OS 32KB");
    systemLabel->setGeometry(1,1,278,32);
//    QPalette pe;
//    pe.setColor(QPalette::Background, Qt::gray);
    systemLabel->setStyleSheet("QLabel { background-color : gray;}");
//    systemLabel->setPalette(pe);


}

void RAMWidget::refreshRAMUI()
{
    delete this->frame;
    this->frame = new QFrame(this);
    this->frame->setGeometry(10,30,280,514);
    this->frame->setLineWidth(1);
    this->frame->setFrameStyle(QFrame::Box);
    this->frame->show();

    QLabel *systemLabel = new QLabel(this->frame);
    systemLabel->setText(" OS 32KB");
    systemLabel->setGeometry(1,1,278,32);
    systemLabel->setStyleSheet("QLabel { background-color : gray;}");
    systemLabel->show();

    PCBManager *manager = PCBManager::getInstance();
    for(int i = 0; i < manager->originInfoList.size(); ++i){
        ProcessMemoryInfo info(manager->originInfoList[i]);
        QLabel *label = new QLabel(this->frame);
        QString strID = QString::number(info.PID, 10);
        QString strStart = QString::number(info.start+32, 10);
        QString strLength = QString::number(info.length, 10);
        QString str = "起始位置:"+strStart+"KB 编号:"+strID+" 大小:"+strLength+"KB";
        label->setText(str);
        label->setGeometry(1,info.start+32,278,info.length);
        PCB *pcb = nullptr;
        if(manager->runningPCB !=nullptr){
            int id = manager->runningPCB->PID;
            if(id == info.PID)
                pcb = manager->runningPCB;
        }
        if(pcb == nullptr){
            for(int i = 0; i < manager->readyLine.size(); ++i){
                if(manager->readyLine[i]->PID == info.PID){
                    pcb = manager->readyLine[i];
                    break;
                }
            }
        }
        if(pcb == nullptr)
            return;
        QString r_str = QString::number(pcb->r,10);
        QString g_str = QString::number(pcb->g,10);
        QString b_str = QString::number(pcb->b,10);
        QString str2 = "QLabel {background-color:rgb("+r_str+","+g_str+","+b_str+");}";
//        Color c(rand()%256,rand()%256,rand()%256);
        label->setStyleSheet(str2);
        label->show();

    }

}
