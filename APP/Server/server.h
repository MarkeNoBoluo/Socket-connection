#ifndef SERVER_H
#define SERVER_H

#include <QMainWindow>
#include<QTcpServer>
#include<QTcpSocket>
#include<QUdpSocket>


namespace Ui { class Server; }


class Server : public QMainWindow
{
    Q_OBJECT

public:
    Server(QWidget *parent = nullptr);
    ~Server();

private slots:
    void on_pushButton_listen_clicked();//TCP侦听


    void server_new_connect(); //创建新连接

    void socket_read_data();//TCP读取缓冲区数据

    void read_data();//UDP读取缓冲区数据

    void on_comboBox_currentIndexChanged();//界面转换


private:
    Ui::Server *ui;
    QUdpSocket *uServer;//UDP套接字对象
    QTcpServer *mServer;
    QTcpSocket *mSocket;//TCP套接字对象
    int i1,i2;
    int j1,j2;
};
#endif // SERVER_H
