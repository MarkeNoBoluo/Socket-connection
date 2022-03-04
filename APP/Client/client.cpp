/*****************************************************************************************/
/*                                        客户端                                          */
/*                                        2022.03.02                                     */
/*                                        GeKuang                                        */
/*                                        v1.0                                           */
/*****************************************************************************************/
#include "client.h"
#include "ui_client.h"
#include<QDebug>
#include<windows.h>
#include<QThread>

/*构造函数*/
App::App(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::App)
{
    ui->setupUi(this);
    ui->ipEdit->setText("192.168.1.44");//默认显示ip为192.168.1.44
    ui->portEdit->setText("8080");//默认显示端口为8080
    mSocket = new QTcpSocket();//TCP套接字初始化
    ui->pushButton_send->setEnabled(true);
    QObject::connect(mSocket,&QTcpSocket::readyRead,this,&App::on_pushButton_send_clicked);
    QObject::connect(mSocket,&QTcpSocket::disconnected,this,&App::socket_disconnected);
    uSocket = new QUdpSocket(this);//UDP套接字初始化

    tim = new QTimer();//初始化计时器对象
    tim->setInterval(1000);//设置计时器间隔时间为1s

    connect(tim,SIGNAL(timeout()),this,SLOT(onTimeOut()));//连接信号槽
    tim->start();//初始化计时器状态
}

/*析构函数*/
App::~App()
{
    delete this->uSocket;
    delete this->mSocket;
    delete ui;
}

/*TCP连接*/
void App::on_pushButtonconnect_clicked()
{
    if(ui->pushButtonconnect->text() == QString::fromLocal8Bit("连接"))
        {
            QString IP;
            int port;         
            IP = ui->ipEdit->text(); //获取IP地址
            port = ui->portEdit->text().toInt(); //获取端口号
            mSocket->abort();//取消已有的连接
            mSocket->connectToHost(IP, port);//连接服务器
            if(!mSocket->waitForConnected(30000))//等待连接成功
            {
                qDebug() << "Connection failed!";
                ui->lineEdit_state->setText(QString::fromLocal8Bit("连接失败！"));
                return;
            }
            qDebug() << "Connect successfully!";
            ui->lineEdit_state->setText(QString::fromLocal8Bit("连接成功！"));          
            ui->pushButton_send->setEnabled(true);//发送按键使能
            ui->pushButtonconnect->setText(QString::fromLocal8Bit("断开连接"));//修改按键文字
        }
        else
        {           
            mSocket->disconnectFromHost();//断开连接
            ui->pushButtonconnect->setText(QString::fromLocal8Bit("连接"));//修改按键文字
            ui->pushButton_send->setEnabled(true);//发送按键使能
        }
}

/*发送数据*/
void App::on_pushButton_send_clicked()
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

/*断开连接*/
void App::socket_disconnected()
{
    ui->pushButton_send->setEnabled(false);//关闭发送按键
    ui->pushButtonconnect->setText(QString::fromLocal8Bit("连接"));//显示
    qDebug()<<"Disconnected!";
}

 /*自动发送*/
void App::on_checkBox_auto_clicked(bool checked)
{
    if(checked)//判断自动发送是否选中
    {
       onTimeOut();//调用计时器延时发送
    }
}

/*设置定时器延时发送*/
void App::onTimeOut()
{
   if(ui->checkBox_auto->isChecked())//判断自动发送是否选中
    {
       static int value = 0;
       ui->progressBar->setValue(value++);

       if(value>5)//延时5秒发送一次
       {
           tim->stop();
           on_pushButton_send_clicked();
           tim->start();
           ui->progressBar->setValue(value = 0);
           ui->checkBox_auto->setChecked(false);
       }
    }

}


