#ifndef RAMWIDGET_H
#define RAMWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QFrame>
#include "processmemoryinfo.h"
#include <QLabel>
#include <vector>

class RAMWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RAMWidget(QWidget *parent = nullptr);

public:
    QFrame *frame;
    std::vector<QLabel*> labelList;

    void refreshRAMUI();

signals:

public slots:
};

#endif // RAMWIDGET_H
//QPalette pal;
//pal.setColor(QPalette::Background, Qt::green);
//this->setAutoFillBackground(true);
//this->setPalette(pal);
