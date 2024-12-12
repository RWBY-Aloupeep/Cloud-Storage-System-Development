#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QTcpSocket>
#include "protocol.h"
#include "opedb.h"
#include <QDir>
#include <QFile>
#include <QTimer>
#include <QDateTime>


class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    MyTcpSocket();
    QString getName();
    void copyDir(QString strSrcDir,QString strDestDir);
    QDateTime m_lastActiveTime; // clean
    bool compressFile(const QString &filePath);
    bool decompressFile(const QString &filePath);
    void searchFile(const QString &strPath, const QString &keyword, QStringList &foundFiles);


signals:
    void offline(MyTcpSocket *mysocket);

public slots:
    void recvMsg();
    void clientOffline();
    void sendFileToClient();
    void handleReply(int reply);

private:
    QString m_strName;
    QFile m_file;
    qint64 m_iTotal;
    qint64 m_iRecved;
    bool m_bUpload;
    QTimer *m_pTimer;
};

#endif // MYTCPSOCKET_H
