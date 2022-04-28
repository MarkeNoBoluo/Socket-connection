/*****************************************************************************************/
/*                                        服务端                                          */
/*                                        2022.03.02                                     */
/*                                        GeKuang                                        */
/*                                        v1.0                                           */
/*****************************************************************************************/

#include "server.h"
#include "ui_server.h"

/*构造函数*/
Server::Server(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Server)
{
    ui->setupUi(this);
    ui->portEdit->setText("8080");//默认端口号为8080

    mServer = new QTcpServer();//初始化TCP对象
    connect(mServer,&QTcpServer::newConnection,this,&Server::server_new_connect);
    //初始化表格行列计数变量
    i1 = 0;
    j1 = 0;
    i2 = 0;
    j2 = 0;

    uServer = new QUdpSocket(this); //初始化UDP对象
    uServer->bind(QHostAddress("192.168.1.44"),8080);//绑定ip地址和端口号
    connect(uServer,SIGNAL(readyRead()),this,SLOT(read_data()));//连接信号槽
}

/*UDP读取缓冲数据*/
void Server::read_data()
{
    if(ui->radioButton_UDP->isChecked())//判断是否勾选UDP协议
    {
        qDebug()<<"recv_UDP";

        QByteArray arr;//创建QByteArray对象存储缓冲区数据
        arr.resize(uServer->bytesAvailable());//设置字符串大小
        uServer->readDatagram(arr.data(),arr.size());//读取缓冲区数据
        qDebug()<<uServer->readDatagram(arr.data(),arr.size());

        QString str ;//创建QString对象用于进行缓冲数据的处理
        str.prepend(arr);//QByteArray转QString
        qDebug()<<"str="<<str;

        if(ui->comboBox->currentText() == "DataA")//处理为DataA数据类型
        {
            ui->stackedWidget->setCurrentIndex(0);
            qDebug()<<str.section(' ',0,0);
            ui->table_output_A->item(i2,j2)->setText(str.section(' ',0,j2));
            j2++;
            qDebug()<<str.section(' ',1,1);
            ui->table_output_A->item(i2,j2)->setText(str.section(' ',1,j2));
            j2++;
            qDebug()<<str.section(' ',2,2);
            ui->table_output_A->item(i2,j2)->setText(str.section(' ',2,j2));

            i2++;
            j2=0;

        }
        else//处理为DataB数据类型
        {
            ui->stackedWidget->setCurrentIndex(1);
            ui->table_output_B->item(i2,j2++)->setText(str.section(' ',0,0));
            ui->table_output_B->item(i2,j2++)->setText(str.section(' ',1,1));
            i2++;
            j2=0;
        }
    }

}


/*析构函数*/
Server::~Server()
{
    uServer->close();
    uServer->deleteLater();
    mServer->close();
    mServer->deleteLater();
    delete ui;
}
/*TCP侦听*/
void Server::on_pushButton_listen_clicked()
{
    if(ui->pushButton_listen->text() == QString::fromLocal8Bit("侦听"))
    {
        int port = ui->portEdit->text().toInt();
        if(!mServer->listen(QHostAddress("192.168.1.44"),port))//判断侦听本机端口和地址是否成功
        {
            ui->portEdit_state->setText(QString::fromLocal8Bit("侦听失败！"));//侦听失败
            qDebug()<<mServer->errorString();
            return;
        }
        ui->pushButton_listen->setText(QString::fromLocal8Bit("取消侦听"));//改变按键功能
        ui->portEdit_state->setText(QString::fromLocal8Bit("侦听成功！"));//侦听成功
        qDebug()<<"Listen successful!";
    }
    else//侦听失败
    {
        mSocket->abort();//销毁TCP套接字对象
        mServer->close();
        ui->portEdit_state->setText(QString::fromLocal8Bit("侦听失败！"));//返回侦听失败
        ui->pushButton_listen->setText(QString::fromLocal8Bit("侦听"));//改变按键功能

    }
}

/*连接新用户*/
void Server::server_new_connect()
{
    mSocket = mServer->nextPendingConnection();
    QObject::connect(mSocket,&QTcpSocket::readyRead,this,&Server::socket_read_data);//连接函数槽
    ui->portEdit_state->setText(QString::fromLocal8Bit("连接成功！"));
    qDebug()<<"A client connect!";
}

/*接收数据*/
void Server::socket_read_data()
{
    if(ui->radioButton_TCP->isChecked())
    {
        QString str= mSocket->readAll();
        qDebug()<<"recv_TCP";
        if(ui->comboBox->currentText() == "DataA")//判断下拉选项的数据类型
        {
            ui->stackedWidget->setCurrentIndex(0);//切换为dataA数据格式表
            ui->table_output_A->item(i1,j1++)->setText(str.section(' ',0,0));
            ui->table_output_A->item(i1,j1++)->setText(str.section(' ',1,1));
            ui->table_output_A->item(i1,j1++)->setText(str.section(' ',2,2));
            i1++;
            j1=0;
        }
        else
        {
            ui->stackedWidget->setCurrentIndex(1);//切换为data数据格式表
            ui->table_output_B->item(i1,j1++)->setText(str.section(' ',0,0));
            ui->table_output_B->item(i1,j1++)->setText(str.section(' ',1,1));
            i1++;
            j1=0;
        }
    }

}
/*下拉框实现dataA与dataB数据结构表的切换*/
void Server::on_comboBox_currentIndexChanged()
{
    if(ui->comboBox->currentText() == "DataA")//判断选定数据类型
        ui->stackedWidget->setCurrentIndex(0);
    else
        ui->stackedWidget->setCurrentIndex(1);
}


