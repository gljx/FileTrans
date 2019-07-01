#include "dialog.h"
#include "ui_dialog.h"
#include"QDebug"
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    port=new QSerialPort;
    portinfo = new QSerialPortInfo();
    ports = portinfo->availablePorts();
    port->setPort(ports.at(0));
    port->setPortName(ports.at(0).systemLocation());
    flag=port->open(QIODevice::ReadWrite);
    port->setStopBits(QSerialPort::OneStop);
    port->setParity(QSerialPort::NoParity);
    port->setBaudRate(QSerialPort::Baud1200);
    ui->setupUi(this);
    connect(ui->SendBtn,SIGNAL(clicked(bool)),this,SLOT(onSendClicked()));
}

Dialog::~Dialog()
{
    port->close();
    delete ui;
}

void Dialog::onSendClicked()
{
    if(flag){
        qDebug()<<"开始发送"<<endl;
        QByteArray bytes;
        bytes.resize(1024);
        for(int i=0;i<1024;i++){
            bytes[i]=0xff;
        }
        port->write(bytes);
        qDebug()<<"发送完毕"<<endl;
    }else{
        qDebug()<<"失败"<<endl;
    }

}
