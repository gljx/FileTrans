#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include<QtSerialPort/QSerialPort>
#include<QtSerialPort/QSerialPortInfo>
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    QSerialPort *port;
    QSerialPortInfo *portinfo;
    QList<QSerialPortInfo> ports;
    ~Dialog();
    bool flag;
private slots:
    void onSendClicked();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
