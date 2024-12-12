#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QWidget>
#include <QFile>
#include <QTcpSocket>
#include "protocol.h"

QT_BEGIN_NAMESPACE
namespace Ui { class TcpClient; }
QT_END_NAMESPACE

class TcpClient : public QWidget
{
    Q_OBJECT

public:
    TcpClient(QWidget *parent = nullptr);
    ~TcpClient();
    void loadConfig();
    static TcpClient &getInstance();
    QTcpSocket &getTcpSocket();
    QString loginName();
    QString curPath();
    void setCurPath(QString strCurPath);
    //---------------------测试VIP-----------------------
    QPixmap getpixmap();
    int getVIPStatus();

signals:
    void update_ok(); // 头像上传成功

public slots:
    void showConnect();

private slots:
//    void on_send_pd_clicked();

    void on_login_pb_clicked();

    void on_regist_pb_clicked();

    void on_cancel_pb_clicked();

    void recvMsg();

    void onDisconnected();    // 新增槽函数，处理断开连接

private:
    Ui::TcpClient *ui;
    QString m_strIP;
    quint16 m_usPort;
    // 连接服务器，和服务器进行数据交互
    QTcpSocket m_tcpSocket;
    QString m_strLoginName;
    QString m_strCurPath;
    QFile m_file;
    QByteArray all_buff;
    //---------------------测试VIP-----------------------   
    QPixmap pic_pixmap;
    int VIP_status;
};
#endif // TCPCLIENT_H
