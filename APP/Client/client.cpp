/*****************************************************************************************/
/*                                        客户端                                          */
/*                                        22.3.2                                         */
/*                                        GeKuang                                        */
/*                                        v1.0                                           */
/*****************************************************************************************/
#include "client.h"
#include "ui_client.h"
#include<QDebug>
#include<windows.h>
#include<QThread>
App::App(QWidget *parent)//构造函数
    : QMainWindow(parent)
    , ui(new Ui::App)
{
    ui->setupUi(this);
    ui->ipEdit->setText("192.168.1.44");//默认显示ip为192.168.1.44
    ui->portEdit->setText("8080");//默认显示端口为8080

    mSocket = new QTcpSocket();
    //TCP对象初始化
    ui->pushButton_send->setEnabled(true);

    QObject::connect(mSocket,&QTcpSocket::readyRead,this,&App::on_pushButton_send_clicked);
    QObject::connect(mSocket,&QTcpSocket::disconnected,this,&App::socket_disconnected);


    //UDP对象初始化
    uSocket = new QUdpSocket(this);
//    connect(ui->pushButton_send,SIGNAL(clicked()),this,SLOT(on_pushButton_send_clicked()));
}

App::~App()//析构函数
{
    delete this->uSocket;
    delete this->mSocket;
    delete ui;
}

void App::on_pushButtonconnect_clicked() //TCP连接
{
    if(ui->pushButtonconnect->text() == QString::fromLocal8Bit("连接"))
        {
            QString IP;
            int port;
            //获取IP地址
            IP = ui->ipEdit->text();
            //获取端口号
            port = ui->portEdit->text().toInt();
            //取消已有的连接
            mSocket->abort();
            //连接服务器
            mSocket->connectToHost(IP, port);

            //等待连接成功
            if(!mSocket->waitForConnected(30000))
            {
                qDebug() << "Connection failed!";
                ui->lineEdit_state->setText(QString::fromLocal8Bit("连接失败！"));
                return;
            }
            qDebug() << "Connect successfully!";
            ui->lineEdit_state->setText(QString::fromLocal8Bit("连接成功！"));
            //发送按键使能
            ui->pushButton_send->setEnabled(true);
            //修改按键文字
            ui->pushButtonconnect->setText(QString::fromLocal8Bit("断开连接"));
        }
        else
        {
            //断开连接
            mSocket->disconnectFromHost();
            //修改按键文字
            ui->pushButtonconnect->setText(QString::fromLocal8Bit("连接"));
            ui->pushButton_send->setEnabled(true);
        }


}

void App::on_pushButton_send_clicked()//发送数据
{
    qDebug()<<"Send: "<<ui->textEdit_enter->toPlainText().toLatin1();


    if(ui->radioButton_TCP->isChecked())//判断使用TCP协议
    {
        qDebug()<<"TCP";
        mSocket->write(ui->textEdit_enter->toPlainText().toLatin1());//tcp协议下客户端发送框内容写入缓冲
        mSocket->flush();
    }
    else if(ui->radioButton_UDP->isChecked())//判断使用UDP协议
    {
        qDebug()<<"UDP";
        uSocket->writeDatagram(ui->textEdit_enter->toPlainText().toLatin1(),QHostAddress("192.168.1.44"),8080);//udp协议下客户端发送框内容写入缓冲
        uSocket->flush();
    }
    else
    {
        ui->lineEdit_state->setText("choose TCP or UDP ");//未选择协议
    }
}

void App::socket_disconnected()//断开连接
{
    ui->pushButton_send->setEnabled(false);//关闭发送按键
    ui->pushButtonconnect->setText(QString::fromLocal8Bit("连接"));//显示
    qDebug()<<"Disconnected!";
}

void App::on_checkBox_auto_clicked(bool checked)//自动发送
{
    while(checked)
    {
        QThread::sleep(3);  //间隔3秒发送
        on_pushButton_send_clicked(); //调用发送函数
    }
}



