#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QWidget>
#include "mytcpserver.h"

QT_BEGIN_NAMESPACE
namespace Ui { class TcpServer; }
QT_END_NAMESPACE

class TcpServer : public QWidget
{
    Q_OBJECT

public:
    TcpServer(QWidget *parent = nullptr);
    ~TcpServer();
    void loadConfig();
    QWidget* getUI() {
            return this;  // 返回当前窗口的指针，即 TcpServer 对象本身
        }
        static TcpServer &getInstance();

signals:
        void replyready(int reply);

private:
    Ui::TcpServer *ui;

    QString m_strIP;
    //存放端口8888
    quint16 m_usPort;

private slots:
    void onTableSelectionChanged(); // Handle table selection
    void on_pushButtonRefresh_clicked();
};


#endif // TCPSERVER_H
