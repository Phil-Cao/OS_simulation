#include "qestiondialog.h"
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QVBoxLayout>


QestionDialog::QestionDialog(QWidget *parent) : QDialog(parent)
{

    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->setColumnMinimumWidth(0, 200);
    gridLayout->setColumnMinimumWidth(1, 100);

//    QLabel *label1 = new QLabel(this);
//    label1->setText("开始时间:");
//    this->start_time_line = new QLineEdit(this);

    QLabel *label2 = new QLabel(this);
    label2->setText("优先级:");
    this->priority_line = new QLineEdit(this);

    QLabel *label4 = new QLabel(this);
    label4->setText("需要时间:");
    this->needtime_line = new QLineEdit(this);

    QLabel *label5 = new QLabel(this);
    label5->setText("需要空间(KB):");
    this->need_space_line = new QLineEdit(this);

//    gridLayout->addWidget(label1,0,0,1,1);
//    gridLayout->addWidget(this->start_time_line,0,1,1,1);
    gridLayout->addWidget(label2,0,0,1,1);
    gridLayout->addWidget(this->priority_line,0,1,1,1);

    gridLayout->addWidget(label4,1,0,1,1);
    gridLayout->addWidget(this->needtime_line,1,1,1,1);

    gridLayout->addWidget(label5, 2,0,1,1);
    gridLayout->addWidget(this->need_space_line, 2,1,1,1);

    QPushButton *confirmButton = new QPushButton(this);
    confirmButton->setText("确定");
    connect(confirmButton, SIGNAL(clicked()),this,SLOT(confirmPress()));
    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->addLayout(gridLayout);
    vLayout->addWidget(confirmButton);

    this->setWindowTitle("添加进程");
    this->resize(400,250);
    this->setLayout(vLayout);
}

void QestionDialog::confirmPress()
{
    accept();
}


