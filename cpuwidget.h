#ifndef CPUWIDGET_H
#define CPUWIDGET_H

#include <QWidget>
#include <vector>
#include "sprocess.h"


class CpuWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CpuWidget(QWidget *parent = nullptr);

    void refreshData();
protected:
    void initData();
    void setUsingState(SProcess* &s);
    void setNotUsingState(SProcess* &s);

public:
    std::vector<SProcess*> readyLine; // 就绪队列
    std::vector<SProcess*> reserveLine; // 后备队列
    std::vector<SProcess*> hangLine;  // 挂起队列
    std::vector<SProcess*> finishLine;  // 完成队列

    SProcess *runningProcess;
signals:
public slots:
};

#endif // CPUWIDGET_H
