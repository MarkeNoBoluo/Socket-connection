#ifndef APP_H
#define APP_H

#include <QMainWindow>
#include<QTcpServer>
#include<QTcpSocket>
#include<QHostAddress>
#include<QFile>
#include<QTimer>
#include<QMessageBox>
#include<QUdpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class App; }
QT_END_NAMESPACE

/*客户端*/
class App : public QMainWindow
{
    Q_OBJECT

public:
    App(QWidget *parent = nullptr);
    ~App();

public slots:
    void onTimeOut();//计时器

private slots:

    void socket_disconnected();//断开连接

    void on_pushButtonconnect_clicked();//TCP连接

    void on_pushButton_send_clicked();//发送数据

    void on_checkBox_auto_clicked(bool checked);//自动发送


private:
    Ui::App *ui;
    QTcpSocket *mSocket;//TCP套接字对象
    QUdpSocket *uSocket;//UDP套接字对象
    QTimer *tim;//定时器对象
};
#endif // APP_H
