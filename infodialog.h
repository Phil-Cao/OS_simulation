#ifndef INFODIALOG_H
#define INFODIALOG_H

#include <QWidget>
#include <QDialog>

class InfoDialog : public QDialog
{
    Q_OBJECT

public:
      explicit InfoDialog(QWidget *parent = nullptr);
      InfoDialog(QWidget *parent, int cpu_time, int priority, int start_time, int needtime, int over_time);
signals:
public slots:
};

#endif // INFODIALOG_H
