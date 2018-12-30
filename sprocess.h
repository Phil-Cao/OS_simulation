#ifndef SPROCESS_H
#define SPROCESS_H

#include <QWidget>
#include <QPushButton>

class SProcess : public QPushButton
{
    Q_OBJECT
public:
    explicit SProcess(QWidget *parent = nullptr);
    SProcess(QWidget *parent, int start_time, int priority, int cpu_time, int needtime);
    int num;
    int cpu_time;
    int priority;
    int start_time;
    int needtime;
    int overtime;
    int r;
    int g;
    int b;

protected:
    void mousePressEvent(QMouseEvent *event);

signals:

public slots:
};

#endif // SPROCESS_H
