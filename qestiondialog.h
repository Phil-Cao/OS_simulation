#ifndef QESTIONDIALOG_H
#define QESTIONDIALOG_H

#include <QLineEdit>
#include <QDialog>
#include <QPushButton>

class QestionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit QestionDialog(QWidget *parent = nullptr);

    QLineEdit *start_time_line;
    QLineEdit *priority_line;
    QLineEdit *needtime_line;
    QLineEdit *need_space_line;

signals:
public slots:
    void confirmPress();
};

#endif // QESTIONDIALOG_H
