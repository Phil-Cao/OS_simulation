#include "infodialog.h"
#include <QLabel>
#include <QLayout>
//int cpu_time;
//int priority;
//int start_time;
//int needtime;

InfoDialog::InfoDialog(QWidget *parent) : QDialog(parent)
{

}

InfoDialog::InfoDialog(QWidget *parent, int start_time, int priority, int cpu_time, int needtime, int over_time) : QDialog(parent)
{

    QString str1 = QString::number(start_time, 10);
    QString str2 = QString::number(priority, 10);
    QString str3 = QString::number(cpu_time, 10);
    QString str4 = QString::number(needtime, 10);
    QString str5 = QString::number(over_time, 10);

    QLabel *label1 = new QLabel(this);
    label1->setText("开始时间:");
    QLabel *start_time_label = new QLabel(this);
    start_time_label->setText(str1);

    QLabel *label2 = new QLabel(this);
    label2->setText("优先级:");
    QLabel *priority_label = new QLabel(this);
    priority_label->setText(str2);


    QLabel *label3 = new QLabel(this);
    label3->setText("运行时间:");
    QLabel *cpu_time_label = new QLabel(this);
    cpu_time_label->setText(str3);

    QLabel *label4 = new QLabel(this);
    label4->setText("需要时间:");
    QLabel *needtime_label = new QLabel(this);
    needtime_label->setText(str4);

    QLabel *label5 = new QLabel(this);
    label5->setText("结束时间:");
    QLabel *overtime_label = new QLabel(this);
    overtime_label->setText(str5);


    QGridLayout *gridLayout = new QGridLayout();

    gridLayout->addWidget(label1,0,0,1,1);
    gridLayout->addWidget(start_time_label,0,1,1,1);
    gridLayout->addWidget(label2,0,2,1,1);
    gridLayout->addWidget(priority_label,0,3,1,1);

    gridLayout->addWidget(label3,1,0,1,1);
    gridLayout->addWidget(cpu_time_label,1,1,1,1);
    gridLayout->addWidget(label4,1,2,1,1);
    gridLayout->addWidget(needtime_label,1,3,1,1);
    gridLayout->addWidget(label5, 2,0,1,2);
    gridLayout->addWidget(overtime_label,2,2,1,2);

    this->setLayout(gridLayout);
    this->setWindowTitle("进程");
    this->resize(350,200);
}
