#include "dialog.h"
#include "ui_dialog.h"
#include"QDebug"
#include<QMimeData>
#include<QTime>
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
    port->setParity(QSerialPort::OddParity);
    port->setBaudRate(QSerialPort::Baud1200);
    this->setAcceptDrops(true);
    ui->setupUi(this);
    this->setWindowTitle("串口文件发送工具Ver1.0-高刘觉曦版权所有!");
    ui->baudrate->addItems(baudlist);
    connect(ui->SendBtn,SIGNAL(clicked(bool)),this,SLOT(onSendClicked()));
    connect(ui->clean,SIGNAL(clicked(bool)),this,SLOT(cleanFile()));
    connect(ui->baudrate,SIGNAL(currentIndexChanged(int)),this,SLOT(changeBaud()));
}

Dialog::~Dialog()
{
    port->close();
    delete ui;
}

void Dialog::onSendClicked()
{
    if(flag&&sendStanby){
        qDebug()<<"开始发送"<<endl;
        QFile file(filepath);
        file.open(QIODevice::ReadOnly);
        SendFile(file.readAll());
        file.close();
        ui->sendlog->append("文件"+filename+"发送完毕\n");
        qDebug()<<"发送完毕"<<endl;
    }else{
        qDebug()<<"失败"<<endl;
        ui->sendlog->append("发送失败\n");
    }

}

void Dialog::SendFile(QByteArray data){
    int page=data.length()%512;
    if(page!=0){
        page=(data.length()/512)+1;
    }else{
        page=data.length()/512;
    }
    QByteArray startFlag;
    startFlag.reserve(512);
    for(int i=0;i<512;i++){
        startFlag[i]=0xff;
    }
    int base=256;
    for(int i=0;i<4;i++){
        startFlag[base+i]=setStartLenth(page)[i];
    }
    base=260;
    QByteArray filenameBytes =filename.toUtf8();
    for(int i=0;i<filenameBytes.length();i++){
        startFlag[base+i]=filenameBytes[i];
    }

    port->write(startFlag);
    Sleep(15);
    for(int i=0;i<page;i++){
        QByteArray datablock;
        datablock.resize(512);
        for(int j=0,k=i;j<512;j++){
            datablock[j]=data[k*512+j];
        }
        ui->sendlog->append("发送第"+QString("%1").arg(i)+"块,共"+QString("%1").arg(page)+"块\n");
        port->write(datablock);
        qDebug()<<"块"<<page<<"发送完毕"<<endl;
        Sleep(10);
        qDebug()<<"LOG"<<endl;
    }

}

void Dialog::dragEnterEvent(QDragEnterEvent *event){
    event->acceptProposedAction();
}

void Dialog::dropEvent(QDropEvent *event){
    filepath = event->mimeData()->urls().first().toLocalFile();
    filename=event->mimeData()->urls().first().fileName();
    ui->sendlog->setText(filepath);
    sendStanby=true;
}

void Dialog::cleanFile(){
    sendStanby=false;
    ui->sendlog->clear();
}

void Dialog::changeBaud(){
    int currentIndex = ui->baudrate->currentIndex();
    switch(currentIndex){
    case 0:
        port->setBaudRate(QSerialPort::Baud1200);
        ui->sendlog->append("改变波特率为1200\n");
        break;
    case 1:
        port->setBaudRate(QSerialPort::Baud2400);
        ui->sendlog->append("改变波特率为2400\n");
        break;
    case 2:
        port->setBaudRate(QSerialPort::Baud4800);
        ui->sendlog->append("改变波特率为4800\n");
        break;
    case 3:
        port->setBaudRate(QSerialPort::Baud9600);
        ui->sendlog->append("改变波特率为9600\n");
        break;
    case 4:
        port->setBaudRate(QSerialPort::Baud19200);
        ui->sendlog->append("改变波特率为19200\n");
        break;
    case 5:
        port->setBaudRate(QSerialPort::Baud38400);
        ui->sendlog->append("改变波特率为38400\n");
        break;
    case 6:
        port->setBaudRate(QSerialPort::Baud57600);
        ui->sendlog->append("改变波特率为57600\n");
        break;
    case 7:
        port->setBaudRate(QSerialPort::Baud115200);
        ui->sendlog->append("改变波特率为11520\n");
        break;
    default:
        port->setBaudRate(QSerialPort::Baud1200);
        ui->sendlog->append("改变波特率为1200\n");
        break;
    }
}

void Dialog::Sleep(int msecond){
    QTime dieTime = QTime::currentTime().addMSecs(msecond);
    while( QTime::currentTime() < dieTime )
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

QByteArray Dialog::setStartLenth(int length){
          QByteArray b;
          b.resize(4);
          for(int i = 0;i < 4;i++)
          {
           b[i]=(char)(length>>(24-i*8));
           }
          return b;
}

