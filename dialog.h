#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include<QtSerialPort/QSerialPort>
#include<QtSerialPort/QSerialPortInfo>
#include<QDropEvent>
#include<QStringList>
#include<QWaitCondition>
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    //拖拽事件
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void SendFile(QByteArray data);
    //end
    //端口声明
    QSerialPort *port;
    QSerialPortInfo *portinfo;
    QList<QSerialPortInfo> ports;
    //end
    ~Dialog();
    bool flag;
    QStringList baudlist={"1200","2400","4800","9600","19200","38400","57600","115200"};
    bool sendStanby=false;
    QString filepath;
    void Sleep(int msecond);
    QByteArray setStartLenth(int length);
    QString filename;
private slots:
    void onSendClicked();
    void cleanFile();
    void changeBaud();
private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
