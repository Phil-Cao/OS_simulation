#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cpuwidget.h"
#include "ramwidget.h"
#include <QLineEdit>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    QMenu *menu[3];
    QAction *act_part1[4];
    QAction *act_part2[2];
    QAction *act_part3[3];
    QMenuBar *menu_bar;
    // 处理器区域
    CpuWidget *cpu_widget;
    // 主存区域
    RAMWidget *ram_widget;
    int count = 1;

    QLineEdit *deleteLineEdit;
    QDialog *deleteDialog;

    QDialog *hangDialog;
    QLineEdit *hangNum;
//    QLineEdit *hangTime;

    QDialog *solveHangDialog;
    QLineEdit *solveHangEdit;


public slots:
    void showAddDialog(QAction* act);
    void deleteConfirm();
    void hangConfirm();
    void solveHangConfirm();
    void mFreshRAMUI();
};

#endif // MAINWINDOW_H
