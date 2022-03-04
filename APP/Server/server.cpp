/*****************************************************************************************/
/*                                        �����                                          */
/*                                        2022.03.02                                     */
/*                                        GeKuang                                        */
/*                                        v1.0                                           */
/*****************************************************************************************/

#include "server.h"
#include "ui_server.h"

/*���캯��*/
Server::Server(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Server)
{
    ui->setupUi(this);
    ui->portEdit->setText("8080");//Ĭ�϶˿ں�Ϊ8080

    mServer = new QTcpServer();//��ʼ��TCP����
    connect(mServer,&QTcpServer::newConnection,this,&Server::server_new_connect);
    //��ʼ��������м�������
    i1 = 0;
    j1 = 0;
    i2 = 0;
    j2 = 0;

    uServer = new QUdpSocket(this); //��ʼ��UDP����
    uServer->bind(QHostAddress("192.168.1.44"),8080);//��ip��ַ�Ͷ˿ں�
    connect(uServer,SIGNAL(readyRead()),this,SLOT(read_data()));//�����źŲ�
}

/*UDP��ȡ��������*/
void Server::read_data()
{
    if(ui->radioButton_UDP->isChecked())//�ж��Ƿ�ѡUDPЭ��
    {
        qDebug()<<"recv_UDP";

        QByteArray arr;//����QByteArray����洢����������
        arr.resize(uServer->bytesAvailable());//�����ַ�����С
        uServer->readDatagram(arr.data(),arr.size());//��ȡ����������
        qDebug()<<uServer->readDatagram(arr.data(),arr.size());

        QString str ;//����QString�������ڽ��л������ݵĴ���
        str.prepend(arr);//QByteArrayתQString
        qDebug()<<"str="<<str;

        if(ui->comboBox->currentText() == "DataA")//����ΪDataA��������
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
        else//����ΪDataB��������
        {
            ui->stackedWidget->setCurrentIndex(1);
            ui->table_output_B->item(i2,j2++)->setText(str.section(' ',0,0));
            ui->table_output_B->item(i2,j2++)->setText(str.section(' ',1,1));
            i2++;
            j2=0;
        }
    }

}


/*��������*/
Server::~Server()
{
    uServer->close();
    uServer->deleteLater();
    mServer->close();
    mServer->deleteLater();
    delete ui;
}
/*TCP����*/
void Server::on_pushButton_listen_clicked()
{
    if(ui->pushButton_listen->text() == QString::fromLocal8Bit("����"))
    {
        int port = ui->portEdit->text().toInt();
        if(!mServer->listen(QHostAddress("192.168.1.44"),port))//�ж����������˿ں͵�ַ�Ƿ�ɹ�
        {
            ui->portEdit_state->setText(QString::fromLocal8Bit("����ʧ�ܣ�"));//����ʧ��
            qDebug()<<mServer->errorString();
            return;
        }
        ui->pushButton_listen->setText(QString::fromLocal8Bit("ȡ������"));//�ı䰴������
        ui->portEdit_state->setText(QString::fromLocal8Bit("�����ɹ���"));//�����ɹ�
        qDebug()<<"Listen successful!";
    }
    else//����ʧ��
    {
        mSocket->abort();//����TCP�׽��ֶ���
        mServer->close();
        ui->portEdit_state->setText(QString::fromLocal8Bit("����ʧ�ܣ�"));//��������ʧ��
        ui->pushButton_listen->setText(QString::fromLocal8Bit("����"));//�ı䰴������

    }
}

/*�������û�*/
void Server::server_new_connect()
{
    mSocket = mServer->nextPendingConnection();
    QObject::connect(mSocket,&QTcpSocket::readyRead,this,&Server::socket_read_data);//���Ӻ�����
    ui->portEdit_state->setText(QString::fromLocal8Bit("���ӳɹ���"));
    qDebug()<<"A client connect!";
}

/*��������*/
void Server::socket_read_data()
{
    if(ui->radioButton_TCP->isChecked())
    {
        QString str= mSocket->readAll();
        qDebug()<<"recv_TCP";
        if(ui->comboBox->currentText() == "DataA")//�ж�����ѡ�����������
        {
            ui->stackedWidget->setCurrentIndex(0);//�л�ΪdataA���ݸ�ʽ��
            ui->table_output_A->item(i1,j1++)->setText(str.section(' ',0,0));
            ui->table_output_A->item(i1,j1++)->setText(str.section(' ',1,1));
            ui->table_output_A->item(i1,j1++)->setText(str.section(' ',2,2));
            i1++;
            j1=0;
        }
        else
        {
            ui->stackedWidget->setCurrentIndex(1);//�л�Ϊdata���ݸ�ʽ��
            ui->table_output_B->item(i1,j1++)->setText(str.section(' ',0,0));
            ui->table_output_B->item(i1,j1++)->setText(str.section(' ',1,1));
            i1++;
            j1=0;
        }
    }

}
/*������ʵ��dataA��dataB���ݽṹ����л�*/
void Server::on_comboBox_currentIndexChanged()
{
    if(ui->comboBox->currentText() == "DataA")//�ж�ѡ����������
        ui->stackedWidget->setCurrentIndex(0);
    else
        ui->stackedWidget->setCurrentIndex(1);
}


