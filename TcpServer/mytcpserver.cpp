#include "mytcpserver.h"
#include <QDebug>
#include <stdio.h>
#include "mytcpsocket.h"
#include "opedb.h"
#include <QSocketNotifier>

MyTcpServer::MyTcpServer()
{
    // -cleantimeout------------------------------------------------------
    m_cleanupTimer = new QTimer(this);
    connect(m_cleanupTimer, &QTimer::timeout, this, &MyTcpServer::cleanupInactiveConnections);
    m_cleanupTimer->start(10000);  // 每 10 秒检查一次连接
    // -------------------------------------------------------------------
    // --------自动创建存放头像的dir----------------------------------------
        QString dirPath = PIC_DIR_PATH;

        // Create a QDir object for the directory
        QDir dir;

        // Check if the directory exists
        if (!dir.exists(dirPath)) {
           // Directory does not exist, so we attempt to create it
           if (dir.mkpath(dirPath)) {
               qDebug() << "Directory created:" << dirPath;
           } else {
               qDebug() << "Failed to create directory:" << dirPath;
           }
       }
        // -------------------------------------------------------------------
}

MyTcpServer &MyTcpServer::getInstance()
{
    static MyTcpServer instance;
    return instance;
}

void MyTcpServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << "new client connected";
    // 改写多线程的方法。---------------------------------------------------------------
    MyTcpSocket *pTcpSocket = new MyTcpSocket();
    pTcpSocket->setSocketDescriptor(socketDescriptor);
    m_tcpSocketList.append(pTcpSocket);

    QThread *thread = new QThread();

    // 在线程启动时，为线程初始化数据库连接
    connect(thread, &QThread::started, []() {
        qDebug() << "Thread started, creating database connection...";
        QSqlDatabase db = OpeDB::getInstance().getDatabase();  // 创建或获取当前线程的数据库连接
        if (!db.isOpen()) {
            qDebug() << "Failed to open database in thread:" << QThread::currentThreadId();
        } else {
            qDebug() << "Database opened successfully in thread:" << QThread::currentThreadId();
        }
    });

    // 在线程退出时，释放线程资源
    connect(pTcpSocket, &MyTcpSocket::disconnected, thread, &QThread::quit);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(thread, &QThread::finished, pTcpSocket, &MyTcpSocket::deleteLater);

    connect(pTcpSocket, SIGNAL(offline(MyTcpSocket*)), this, SLOT(deleteSocket(MyTcpSocket*)));

    pTcpSocket->moveToThread(thread);
    thread->start();

    emit newConnection();
    // -------------------------------------------------------------------------------
}



void MyTcpServer::resend(const char *targetName, PDU *pdu)
{
    if (NULL == targetName || NULL == pdu)
    {
        return;
    }

    QString strTargetName = targetName;

    // 遍历 socket 列表，找到目标 socket2
    for (int i = 0; i < m_tcpSocketList.size(); i++)
    {
        MyTcpSocket *targetSocket = m_tcpSocketList.at(i);
        if (strTargetName == targetSocket->getName())
        {

            // 检查目标 socket 是否在当前线程中
            if (targetSocket->thread() == QThread::currentThread())
            {
                // 如果目标 socket 和当前线程相同，直接写入
                targetSocket->write((char*)pdu,pdu->uiPDULen);
            }
            else
            {
                // 如果不在同一线程，使用 QMetaObject::invokeMethod 传递消息
                QMetaObject::invokeMethod(targetSocket, [targetSocket, pdu]() {
                    targetSocket->write((char*)pdu,pdu->uiPDULen);
                }, Qt::QueuedConnection);
            }

            break; // 找到目标后退出循环
        }
    }
}


void MyTcpServer::deleteSocket(MyTcpSocket *mysocket)
{
    //服务器在delete掉当前要关闭的socket时服务器总是异常结束啊(添加delete *iter会出现问题，
    //不添加delete只是从列表中删除并不会异常结束)的解决方案：将delete改为deleteLater()，延迟释放空间
    QList<MyTcpSocket*>::iterator iter = m_tcpSocketList.begin();
    for(;iter!=m_tcpSocketList.end();iter++)
    {
        if(mysocket == *iter)
        {
//            delete *iter;
            (*iter) ->deleteLater();
            *iter = NULL;
            m_tcpSocketList.erase(iter);
            break;
        }
    }
    for (int i=0; i<m_tcpSocketList.size();i++){
        qDebug() << m_tcpSocketList.at(i)->getName();
    }

}

void MyTcpServer::cleanupInactiveConnections()
{
    auto sockets = m_tcpSocketList;    // 复制一份列表，避免修改原列表时冲突
    QDateTime currentTime = QDateTime::currentDateTime();

    for (MyTcpSocket* pTcpSocket : sockets) {
        if (pTcpSocket->m_lastActiveTime.secsTo(currentTime) > 300) { //五分钟
            qDebug() << "Closing inactive connection:" << pTcpSocket->getName();
            // 使用 invokeMethod 在 pTcpSocket 所属线程中调用
            QMetaObject::invokeMethod(pTcpSocket, [pTcpSocket]() {
                pTcpSocket->disconnectFromHost();
                if (pTcpSocket->state() != QAbstractSocket::UnconnectedState) {
                    pTcpSocket->waitForDisconnected();
                }
            }, Qt::QueuedConnection);
        }
    }

}
