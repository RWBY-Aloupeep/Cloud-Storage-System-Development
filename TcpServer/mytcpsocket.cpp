#include "mytcpsocket.h"
#include <QDebug>
#include "mytcpserver.h"
#include <QDir>
#include <QFileInfoList>
#include <QMessageBox>
#include "tcpserver.h"
#include <QProcess>

MyTcpSocket::MyTcpSocket()
{
    connect(this,SIGNAL(readyRead())
            ,this,SLOT(recvMsg()));
    connect(this,SIGNAL(disconnected())
            ,this,SLOT(clientOffline()));
    connect(&TcpServer::getInstance(), &TcpServer::replyready, this, &MyTcpSocket::handleReply);
    m_bUpload= false;
    QMetaObject::invokeMethod(this, [=]() {
        m_pTimer = new QTimer(this);  // `this` 是主线程的 QObject
        connect(m_pTimer, SIGNAL(timeout()), this, SLOT(sendFileToClient()));
    }, Qt::QueuedConnection);
}

// 返回当前请求的用户用户名
QString MyTcpSocket::getName()
{
    return m_strName;
}

// 拷贝文件夹
void MyTcpSocket::copyDir(QString strSrcDir, QString strDestDir)
{
    QDir dir;
    dir.mkdir(strDestDir);
    dir.setPath(strSrcDir);
    QFileInfoList fileInfoList = dir.entryInfoList();
    QString srcTemp;
    QString destTemp;
    for(int i = 0;i<fileInfoList.size();i++)
    {
        qDebug() << "filename:" << fileInfoList[i].fileName();
        if(fileInfoList[i].isFile())
        {
            srcTemp = strSrcDir + '/' + fileInfoList[i].fileName();
            destTemp = strDestDir + '/' + fileInfoList[i].fileName();
            QFile::copy(srcTemp,destTemp);
        }
        else if(fileInfoList[i].isDir())
        {
            if(QString(".") == fileInfoList[i].fileName() || QString("..") == fileInfoList[i].fileName())
            {
                continue;
            }
            srcTemp = strSrcDir + '/' + fileInfoList[i].fileName();
            destTemp = strDestDir + '/' + fileInfoList[i].fileName();
            copyDir(srcTemp,destTemp);
        }
    }
}

// 接收来自客户端的消息
void MyTcpSocket::recvMsg()
{
    m_lastActiveTime = QDateTime::currentDateTime(); //更新最新操作时间
    if(!m_bUpload)
    {
        qDebug() << this->bytesAvailable();
        uint uiPDULen  = 0;
        this->read((char*)&uiPDULen,sizeof(uint));
        uint uiMsgLen = uiPDULen - sizeof(PDU);
        PDU *pdu = mkPDU(uiMsgLen);
        this->read((char*)pdu+sizeof(uint),uiPDULen-sizeof(uint));
        switch (pdu->uiMsgType)
        {
            // 注册请求
            case ENUM_MSG_TYPE_REGIST_REQUEST:
            {
                char caName[32] = {'\0'};
                char caPwd[32] = {'\0'};
                strncpy(caName, pdu->caData, 32);
                strncpy(caPwd, pdu->caData + 32, 32);

                bool res = OpeDB::getInstance().handleRegist(caName, caPwd);
                PDU *respdu = mkPDU(0);
                respdu->uiMsgType = ENUM_MSG_TYPE_REGIST_RESPOND;

                if (res)
                {
                    strcpy(respdu->caData, REGIST_OK);

                    // 创建用户目录
                    QDir dir;
                    QString userDir = QString("./%1").arg(caName);
                    bool userDirCreated = dir.mkdir(userDir);
                    qDebug() << "User directory created:" << userDirCreated;

                    // 创建回收站目录
                    QString recycleBinPath = userDir + "/RecycleBin";
                    bool recycleBinCreated = dir.mkdir(recycleBinPath);
                    qDebug() << "RecycleBin directory created:" << recycleBinCreated;
                }
                else
                {
                    strcpy(respdu->caData, REGIST_FAILED);
                }

                write((char *)respdu, respdu->uiPDULen);
                free(respdu);
                respdu = NULL;
                break;
            }
            // 登录请求
            case ENUM_MSG_TYPE_LOGIN_REQUEST:
            {
                char caName[32] = {'\0'};
                char caPwd[32] = {'\0'};
                strncpy(caName,pdu->caData,32);
                strncpy(caPwd,pdu->caData+32,32);
                bool res = OpeDB::getInstance().handleLogin(caName,caPwd);
                int VIP = OpeDB::getInstance().handleGetVIP(caName);

                // --------------------修改判断是否已经存在头像--------------------------------
                PDU *respdu;
                if(res)
                {
                    QString pic_path= QString("%1/%2.png").arg(PIC_DIR_PATH).arg(caName);
                    m_file.setFileName(pic_path);
                    qDebug()<<"pic_size:"<<m_file.size();
                    respdu = mkPDU(m_file.size()+sizeof(int));

                    respdu->uiMsgType = ENUM_MSG_TYPE_LOGIN_RESPOND;
                    if (m_file.exists()) {
                        // 存在头像
                        if (m_file.open(QIODevice::ReadOnly)) {
                            QByteArray fileData = m_file.readAll();
                            const char *charPtr = fileData.data();
                            QString fileContent = QString::fromUtf8(fileData);
                            respdu->caMsg[0]=VIP;
                            memcpy((char*)respdu->caMsg+sizeof(int), charPtr,m_file.size());  // 说明存在头像
                        }
                    } else {
                          // 说明不存在头像
                          respdu->caMsg[0]=VIP;
            //                        memset(respdu->caMsg, 0, respdu->uiMsgLen);
                    }
                    qDebug()<<"储存最后一个数"<<VIP;

                    strcpy(respdu->caData,LOGIN_OK);
                    m_strName= caName;
                    m_file.close();
                }
                // ----------------------------------------------------------------------
                else
                {
                    respdu = mkPDU(sizeof(int));
                    respdu->caMsg[0]=VIP;
                    respdu->uiMsgType = ENUM_MSG_TYPE_LOGIN_RESPOND;
                    strcpy(respdu->caData,LOGIN_FAILED);
                }


                // 检查回收站目录是否存在，如果不存在就创建一个----------------------------------------
                QString userDir = QString("./%1").arg(m_strName);
                QString recycleBinPath = QString("./%1/RecycleBin").arg(userDir);

                QDir dir;
                if (!dir.exists(recycleBinPath))
                {
                    bool recycleBinCreated = dir.mkdir(recycleBinPath);
                    qDebug() << "RecycleBin directory created:" << recycleBinCreated;
                }
                //----------------------------------------------------------------------------------

                write((char*)respdu,respdu->uiPDULen);
                free(respdu);
                respdu = NULL;
                break;
            }
            // 查看在线用户请求
            case ENUM_MSG_TYPE_ALL_ONLINE_REQUEST:
            {
                QStringList res = OpeDB::getInstance().handleAllOnline();
                uint uiMsgLen = res.size() * 32;
                PDU *resPdu = mkPDU(uiMsgLen);
                resPdu->uiMsgType = ENUM_MSG_TYPE_ALL_ONLINE_RESPOND;
                for(int i = 0;i<res.size();i++)
                {
                    memcpy((char*)(resPdu->caMsg) + i * 32,res.at(i).toStdString().c_str(),res.at(i).size());
                }
                write((char*)resPdu,resPdu->uiPDULen);
                free(resPdu);
                resPdu = NULL;
                break;
            }
            // 查看查找用户请求
            case ENUM_MSG_TYPE_SEARCH_USR_REQUEST:
            {
                int res = OpeDB::getInstance().handleSearchUsr(pdu->caData);
                PDU *resPdu = mkPDU(0);
                resPdu->uiMsgType = ENUM_MSG_TYPE_SEARCH_USR_RESPOND;
                if(-1 == res)
                {
                    strcpy(resPdu->caData,SEARCH_USR_NO);
                }
                else if(1 == res)
                {
                    strcpy(resPdu->caData,SEARCH_USR_ONLINE);
                }
                else if(0 == res)
                {
                    strcpy(resPdu->caData,SEARCH_USR_OFFLINE);
                }
                write((char*)resPdu,resPdu->uiPDULen);
                free(resPdu);
                resPdu = NULL;
                break;
            }
            // 查看添加好友请求
            case ENUM_MSG_TYPE_ADD_FRIEND_REQUEST:
            {
                char caPerName[32] = {'\0'};
                char caName[32] = {'\0'};
                strncpy(caPerName,pdu->caData,32);
                strncpy(caName,pdu->caData+32,32);
                int res = OpeDB::getInstance().handleAddFriend(caPerName,caName);
                qDebug() << "当前这个人在不在？" << res;
                PDU *respdu = NULL;
                if(-1== res)
                {
                    respdu = mkPDU(0);
                    respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
                    strcpy(respdu->caData,UNKNOW_ERROR);
                    write((char*)respdu,respdu->uiPDULen);
                    free(respdu);
                    respdu = NULL;
                }
                else if(0 == res)
                {
                    respdu = mkPDU(0);
                    respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
                    strcpy(respdu->caData,EXISTED_FRIEND);
                    write((char*)respdu,respdu->uiPDULen);
                    free(respdu);
                    respdu = NULL;
                }
                else if(1 == res)
                {
                    MyTcpServer::getInstance().resend(caPerName,pdu);
                }
                else if(2 == res)
                {
                    respdu = mkPDU(0);
                    respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
                    strcpy(respdu->caData,ADD_FRIEND_OFFLINE);
                    write((char*)respdu,respdu->uiPDULen);
                    free(respdu);
                    respdu = NULL;
                }
                else if(3 == res)
                {
                    respdu = mkPDU(0);
                    respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
                    strcpy(respdu->caData,ADD_FRIEND_NO_EXIST);
                    write((char*)respdu,respdu->uiPDULen);
                    free(respdu);
                    respdu = NULL;
                }
                break;
            }
            case ENUM_MSG_TYPE_ADD_FRIEND_AGGREE:
            {
                char caPerName[32] = {'\0'};
                char caName[32] = {'\0'};
                strncpy(caPerName, pdu->caData, 32);
                strncpy(caName, pdu->caData+32, 32);
                OpeDB::getInstance().handleAgreeAddFriend(caPerName, caName);
                MyTcpServer::getInstance().resend(caName, pdu);
                break;
            }
            case ENUM_MSG_TYPE_ADD_FRIEND_REFUSE:
            {
                char caName[32] = {'\0'};
                strncpy(caName, pdu->caData+32, 32);
                MyTcpServer::getInstance().resend(caName, pdu);
                break;
            }
            // 查看刷新在线好友列表请求
            case ENUM_MSG_TYPE_FLUSH_FRIEND_REQUEST:
            {
                char caName[32] = {'\0'};
                strncpy(caName, pdu->caData, 32);
                QStringList res = OpeDB::getInstance().handleFlushFriend(caName);
                uint uiMsglen = res.size() * 32;
                PDU *respdu = mkPDU(uiMsglen);
                respdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_FRIEND_RESPOND;
                for(int i = 0;i<res.size();i++)
                {
                    memcpy((char*)(respdu->caMsg) + i * 32,res.at(i).toStdString().c_str(),res.at(i).size());
                }
                write((char*)respdu,respdu->uiPDULen);
                free(respdu);
                respdu = NULL;
                break;
            }
            // 查看删除好友请求
            case ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST:
            {
                char caSelName[32] = {'\0'};
                char caFriendName[32] = {'\0'};
                strncpy(caSelName,pdu->caData,32);
                strncpy(caFriendName,pdu->caData+32,32);
                OpeDB::getInstance().handleDelFriend(caSelName,caFriendName);
                PDU *respdu = mkPDU(0);
                respdu->uiMsgType = ENUM_MSG_TYPE_DELETE_FRIEND_RESPOND;
                strcpy(respdu->caData,DEL_FRIEND_OK);
                write((char*)respdu,respdu->uiPDULen);
                free(respdu);
                respdu = NULL;
                MyTcpServer::getInstance().resend(caFriendName,pdu);
                break;
            }
            // 查看私聊请求
            case ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST:
            {
                char caPerName[32] = {'\0'};
                memcpy(caPerName,pdu->caData+32,32);
                qDebug()<<caPerName;
                MyTcpServer::getInstance().resend(caPerName,pdu);
//                char caPerName[32] = {'\0'};
//                memcpy(caPerName,pdu->caData + 32,32);
//                qDebug() << caPerName;
//                MyTcpServer::getInstance().resend(caPerName,pdu);
                break;
            }

            // 查看群聊请求
//            case ENUM_MSG_TYPE_GROUP_CHAT_REQUEST:
//            {
//                char caName[32] = {'\0'};
//                strncpy(caName, pdu->caData, 32);
//                QStringList onlineFriend = OpeDB::getInstance().handleFlushFriend(caName);
//                QString temp;
//                for(int i = 0;i<onlineFriend.size();i++)
//                {
//                    temp = onlineFriend.at(i);
//                    MyTcpServer::getInstance().resend(temp.toStdString().c_str(),pdu);
//                }
//                break;
//            }
            // 查看创建文件夹请求
            case ENUM_MSG_TYPE_CREATE_DIR_REQUEST:
            {
                QDir dir;
                QString strCurPath = QString("%1").arg((char*)(pdu->caMsg));
                bool res = dir.exists(strCurPath);
                PDU *respdu = NULL;
                // 当前目录存在
                if(res)
                {
                    char caNewDir[32] = {'\0'};
                    memcpy(caNewDir,pdu->caData + 32,32);
                    QString strNewPath = strCurPath + "/" + caNewDir;
                    qDebug() << strNewPath;
                    res = dir.exists(strNewPath);
                    // 创建的文件名已存在
                    if(res)
                    {
                        respdu = mkPDU(0);
                        respdu->uiMsgType = ENUM_MSG_TYPE_CREATE_DIR_RESPOND;
                        strcpy(respdu->caData,FILE_NAME_EXIST);
                    }
                    // 创建的文件名不存在
                    else
                    {
                        dir.mkdir(strNewPath);
                        respdu = mkPDU(0);
                        respdu->uiMsgType = ENUM_MSG_TYPE_CREATE_DIR_RESPOND;
                        strcpy(respdu->caData,CREAT_DIR_OK);
                    }
                }
                // 当前目录不存在
                else
                {
                    respdu = mkPDU(0);
                    respdu->uiMsgType = ENUM_MSG_TYPE_CREATE_DIR_RESPOND;
                    strcpy(respdu->caData,DIR_NO_EXIST);
                }
                write((char*)respdu,respdu->uiPDULen);
                free(respdu);
                respdu = NULL;
                break;
            }
            // 查看查看所有文件请求
            case ENUM_MSG_TYPE_FLUSH_FILE_REQUEST:
            {
                char *pCurPath = new char[pdu->uiMsgLen];
                memcpy(pCurPath,pdu->caMsg,pdu->uiMsgLen);
                QDir dir(pCurPath);
                QFileInfoList fileInfoList = dir.entryInfoList();
                int iFileCount = fileInfoList.size();
                PDU *respdu = mkPDU(sizeof(FileInfo) * iFileCount);
                respdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_FILE_RESPOND;
                FileInfo *pFileInfo = NULL;
                QString strFileName;
                for(int i = 0;i<fileInfoList.size();i++)
                {
                    pFileInfo = (FileInfo*)(respdu->caMsg) + i;
                    strFileName = fileInfoList[i].fileName();
                    memcpy(pFileInfo->caFileName,strFileName.toStdString().c_str(),strFileName.size());
                    if(fileInfoList[i].isDir())
                    {
                        pFileInfo->iFileType = 0;
                    }
                    else if(fileInfoList[i].isFile())
                    {
                        pFileInfo->iFileType = 1;
                    }
                }
                write((char*)respdu,respdu->uiPDULen);
                free(respdu);
                respdu = NULL;
                break;
            }
            // 查看删除文件夹请求
            case ENUM_MSG_TYPE_DEL_DIR_REQUEST:
            {
                char caName[32] = {'\0'};
                strcpy(caName,pdu->caData);
                char *pPath = new char[pdu->uiMsgLen];
                memcpy(pPath,pdu->caMsg,pdu->uiMsgLen);
                QString strPath = QString("%1/%2").arg(pPath).arg(caName);
                qDebug() << strPath;
                QFileInfo fileInfo(strPath);
                bool res = false;
                // 文件夹
                if(fileInfo.isDir())
                {
                    QDir dir;
                    dir.setPath(strPath);
                    res = dir.removeRecursively();
                }
                // 常规文件
                else if(fileInfo.isFile())
                {
                    res = false;
                }
                PDU *respdu = NULL;
                if(res)
                {
                    respdu = mkPDU(strlen(DEL_DIR_OK) + 1);
                    respdu->uiMsgType = ENUM_MSG_TYPE_DEL_DIR_RESPOND;
                    memcpy(respdu->caData,DEL_DIR_OK,strlen(DEL_DIR_OK));
                }
                else
                {
                    respdu = mkPDU((strlen(DEL_DIR_FAILURED)) + 1);
                    respdu->uiMsgType = ENUM_MSG_TYPE_DEL_DIR_RESPOND;
                    memcpy(respdu->caData,DEL_DIR_FAILURED,strlen(DEL_DIR_FAILURED));
                }
                write((char*)respdu,respdu->uiPDULen);
                free(respdu);
                respdu = NULL;
                break;
            }
            // 查看重命名文件请求
            case ENUM_MSG_TYPE_RENAME_FILE_REQUEST:
            {
                char caOldName[32] = {'\0'};
                char caNewName[32] = {'\0'};
                strncpy(caOldName,pdu->caData,32);
                strncpy(caNewName,pdu->caData + 32,32);
                char *pPath = new char[pdu->uiMsgLen];
                memcpy(pPath,pdu->caMsg,pdu->uiMsgLen);
                QString strOldPath = QString("%1/%2").arg(pPath).arg(caOldName);
                QString strNewPath = QString("%1/%2").arg(pPath).arg(caNewName);
                QDir dir;
                bool res = dir.rename(strOldPath,strNewPath);
                PDU *respdu = mkPDU(0);
                respdu->uiMsgType = ENUM_MSG_TYPE_RENAME_FILE_RESPOND;
                if(res)
                {
                    strcpy(pdu->caData,RENAME_FILE_OK);
                }
                else
                {
                    strcpy(pdu->caData,RENAME_FILE_FAILURED);
                }
                write((char*)respdu,respdu->uiPDULen);
                free(respdu);
                respdu = NULL;
                break;
            }
            // 查看进入文件夹请求
            case ENUM_MSG_TYPE_ENTER_DIR_REQUEST:
            {
                char caEnterName[32] = {'\0'};
                strncpy(caEnterName,pdu->caData,32);
                char *pPath = new char[pdu->uiMsgLen];
                memcpy(pPath,pdu->caMsg,pdu->uiMsgLen);
                QString strPath = QString("%1/%2").arg(pPath).arg(caEnterName);
                QFileInfo fileInfo(strPath);
                PDU *respdu = NULL;
                if(fileInfo.isDir())
                {
                    QDir dir(strPath);
                    QFileInfoList fileInfoList = dir.entryInfoList();
                    int iFileCount = fileInfoList.size();
                    respdu = mkPDU(sizeof(FileInfo) * iFileCount);
                    respdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_FILE_RESPOND;
                    FileInfo *pFileInfo = NULL;
                    QString strFileName;
                    for(int i = 0;i<fileInfoList.size();i++)
                    {
                        pFileInfo = (FileInfo*)(respdu->caMsg) + i;
                        strFileName = fileInfoList[i].fileName();
                        memcpy(pFileInfo->caFileName,strFileName.toStdString().c_str(),strFileName.size());
                        if(fileInfoList[i].isDir())
                        {
                            pFileInfo->iFileType = 0;
                        }
                        else if(fileInfoList[i].isFile())
                        {
                            pFileInfo->iFileType = 1;
                        }
                    }
                    write((char*)respdu,respdu->uiPDULen);
                    free(respdu);
                    respdu = NULL;
                }
                else if(fileInfo.isFile())
                {
                    respdu = mkPDU(0);
                    respdu->uiMsgType = ENUM_MSG_TYPE_ENTER_DIR_RESPOND;
                    strcpy(respdu->caData,ENTER_DIR_FAILURED);
                    write((char*)respdu,respdu->uiPDULen);
                    free(respdu);
                    respdu = NULL;
                }
                break;
            }
            // 查看上传文件请求
            case ENUM_MSG_TYPE_UPLOAD_FILE_REQUEST:
            {
                char caFileName[32] = {'\0'};
                qint64 fileSize = 0;
                sscanf(pdu->caData,"%s %lld",caFileName,&fileSize);
                char *pPath = new char[pdu->uiMsgLen];
                memcpy(pPath,pdu->caMsg,pdu->uiMsgLen);
                QString strPath = QString("%1/%2").arg(pPath).arg(caFileName);
                delete []pPath;
                pPath = NULL;
                m_file.setFileName(strPath);
                // 以只写的方式打开文件，若文件不存在，则会自动创建文件
                if(m_file.open(QIODevice::WriteOnly))
                {
                    m_bUpload = true;
                    m_iTotal = fileSize;
                    m_iRecved = 0;
                }
                break;
            }
            // 查看删除文件请求
            case ENUM_MSG_TYPE_DEL_FILE_REQUEST:
            {
                char caName[32] = {'\0'};
                strcpy(caName,pdu->caData);
                char *pPath = new char[pdu->uiMsgLen];
                memcpy(pPath,pdu->caMsg,pdu->uiMsgLen);
                QString strPath = QString("%1/%2").arg(pPath).arg(caName);
                qDebug() << strPath;
                QFileInfo fileInfo(strPath);
                bool res = false;
                // 文件夹
                if(fileInfo.isDir())
                {
                    res = false;
                }
                // 常规文件
                else if(fileInfo.isFile())
                {
                    QDir dir;
                    res = dir.remove(strPath);
                }
                PDU *respdu = NULL;
                if(res)
                {
                    respdu = mkPDU(strlen(DEL_FILE_OK) + 1);
                    respdu->uiMsgType = ENUM_MSG_TYPE_DEL_FILE_RESPOND;
                    memcpy(respdu->caData,DEL_FILE_OK,strlen(DEL_FILE_OK));
                }
                else
                {
                    respdu = mkPDU((strlen(DEL_FILE_FAILURED)) + 1);
                    respdu->uiMsgType = ENUM_MSG_TYPE_DEL_FILE_RESPOND;
                    memcpy(respdu->caData,DEL_FILE_FAILURED,strlen(DEL_FILE_FAILURED));
                }
                write((char*)respdu,respdu->uiPDULen);
                free(respdu);
                respdu = NULL;
                break;
            }
            // 查看下载文件请求
            case ENUM_MSG_TYPE_DOWNLOAD_FILE_REQUEST:
            {
                char caFileName[32] = {'\0'};
                strcpy(caFileName, pdu->caData);
                char *pPath = new char[pdu->uiMsgLen];
                memcpy(pPath, pdu->caMsg, pdu->uiMsgLen);
                QString strPath = QString("%1/%2").arg(pPath).arg(caFileName);
                qDebug() << strPath;
                delete []pPath;
                pPath = NULL;
                QFileInfo fileInfo(strPath);
                qint64 fileSize = fileInfo.size();
                PDU *respdu = mkPDU(0);
                respdu->uiMsgType = ENUM_MSG_TYPE_DOWNLOAD_FILE_RESPOND;
                sprintf(respdu->caData, "%s %lld", caFileName, fileSize);
                write((char*)respdu, respdu->uiPDULen);
                free(respdu);
                respdu = NULL;
                m_file.setFileName(strPath);
                m_file.open(QIODevice::ReadOnly);
                m_pTimer->start(1000);
                break;
            }
            // 查看分享文件请求
            case ENUM_MSG_TYPE_SHARE_FILE_REQUEST:
            {
                char caSendName[32] = {'\0'};
                int num = 0;
                sscanf(pdu->caData,"%s%d",caSendName,&num);
                int size = num * 32;
                PDU *respdu = mkPDU(pdu->uiMsgLen - size);
                respdu->uiMsgType = ENUM_MSG_TYPE_SHARE_FILE_NOTE_REQUEST;
                strcpy(respdu->caData,caSendName);
                memcpy(respdu->caMsg,(char*)(pdu->caMsg) + size,pdu->uiMsgLen - size);
                char caRecvName[32] = {'\0'};
                for(int i = 0;i<num;i++)
                {
                    memcpy(caRecvName,(char*)(pdu->caMsg) + i * 32,32);
                    MyTcpServer::getInstance().resend(caRecvName,respdu);
                }
                free(respdu);
                respdu = NULL;
                respdu = mkPDU(0);
                respdu->uiMsgType = ENUM_MSG_TYPE_SHARE_FILE_RESPOND;
                strcpy(respdu->caData,"share file ok");
                write((char*)respdu,respdu->uiPDULen);
                free(respdu);
                respdu = NULL;
                break;
            }
            // 查看分享文件通知回复
            case ENUM_MSG_TYPE_SHARE_FILE_NOTE_RESPOND:
            {
                QString strRecvPath = QString("./%1").arg(pdu->caData);
                QString strShareFilePath = QString("%1").arg((char*)(pdu->caMsg));
                int index = strShareFilePath.lastIndexOf('/');
                QString strFileName = strShareFilePath.right(strShareFilePath.size() - index - 1);
                strRecvPath = strRecvPath + '/' + strFileName;
                QFileInfo fileInfo(strShareFilePath);
                if(fileInfo.isFile())
                {
                    QFile::copy(strShareFilePath,strRecvPath);
                }
                else if(fileInfo.isDir())
                {
                    copyDir(strShareFilePath,strRecvPath);
                }
                break;
            }
            // 查看移动文件请求
            case ENUM_MSG_TYPE_MOVE_FILE_REQUEST:
            {
                char caFileName[32] = {'\0'};
                int srcLen = 0;
                int destLen = 0;
                sscanf(pdu->caData,"%d%d%s",&srcLen,&destLen,caFileName);
                char *pSrcPath = new char[srcLen + 1];
                char *pDestPath = new char[destLen + 1 + 32];
                memset(pSrcPath,'\0',srcLen + 1);
                memset(pDestPath,'\0',destLen + 1 + 32);
                memcpy(pSrcPath,pdu->caMsg,srcLen);
                memcpy(pDestPath,(char*)(pdu->caMsg) + (srcLen + 1),destLen);
                PDU *respdu = mkPDU(0);
                respdu->uiMsgType = ENUM_MSG_TYPE_MOVE_FILE_RESPOND;
                QFileInfo fileInfo(pDestPath);
                if(fileInfo.isDir())
                {
                    strcat(pDestPath,"/");
                    strcat(pDestPath,caFileName);
                    bool res = QFile::rename(pSrcPath,pDestPath);
                    if(res)
                    {
                        strcpy(respdu->caData,MOVE_FILE_OK);
                    }
                    else
                    {
                        strcpy(respdu->caData,COMMON_ERR);
                    }
                }
                else if(fileInfo.isFile())
                {
                    strcpy(respdu->caData,MOVE_FILE_FAILURED);
                }
                write((char*)respdu,respdu->uiPDULen);
                free(respdu);
                respdu = NULL;
                break;
            }
            // 清空回收站请求
            case ENUM_MSG_TYPE_CLEAR_BIN_REQUEST:
            {
                QString userName = QString::fromStdString(pdu->caData);
                QString recycleBinPath = QString("./%1/RecycleBin").arg(userName);
                QDir dir(recycleBinPath);

                PDU *respdu = mkPDU(0);
                respdu->uiMsgType = ENUM_MSG_TYPE_CLEAR_BIN_RESPOND;

                if (dir.exists()) {
                    bool success = true;
                    dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
                    QFileInfoList fileList = dir.entryInfoList();
                    for (const QFileInfo &fileInfo : fileList) {
                        if (fileInfo.isDir()) {
                            QDir subDir(fileInfo.absoluteFilePath());
                            if (!subDir.removeRecursively()) {
                                success = false;
                            }
                        } else if (fileInfo.isFile()) {
                            if (!QFile::remove(fileInfo.absoluteFilePath())) {
                                success = false;
                            }
                        }
                    }
                    if (success) {
                        strcpy(respdu->caData, "Recycle Bin cleared successfully.");
                    } else {
                        strcpy(respdu->caData, "Failed to clear some files in Recycle Bin.");
                    }
                } else {
                    strcpy(respdu->caData, "Recycle Bin not found.");
                }
                write((char*)respdu, respdu->uiPDULen);
                free(respdu);
                break;
                }
            //刷新回收站请求
                case ENUM_MSG_TYPE_FLUSH_BIN_REQUEST:
                {
                    char *pCurPath = new char[pdu->uiMsgLen];
                    memcpy(pCurPath,pdu->caMsg,pdu->uiMsgLen);
                    QDir dir(pCurPath);
                    QFileInfoList fileInfoList = dir.entryInfoList();
                    int iFileCount = fileInfoList.size();
                    PDU *respdu = mkPDU(sizeof(FileInfo) * iFileCount);
                    respdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_BIN_RESPOND;
                    FileInfo *pFileInfo = NULL;
                    QString strFileName;
                    for(int i = 0;i<fileInfoList.size();i++)
                    {
                        qDebug()<<fileInfoList[i].fileName();
                        pFileInfo = (FileInfo*)(respdu->caMsg) + i;
                        strFileName = fileInfoList[i].fileName();
                        memcpy(pFileInfo->caFileName,strFileName.toStdString().c_str(),strFileName.size());
                        if(fileInfoList[i].isDir())
                        {
                            pFileInfo->iFileType = 0;
                        }
                        else if(fileInfoList[i].isFile())
                        {
                            pFileInfo->iFileType = 1;
                        }
                    }
                    write((char*)respdu,respdu->uiPDULen);
                    free(respdu);
                    respdu = NULL;
                    break;
            }
            //进入回收站文件夹
            case ENUM_MSG_TYPE_ENTER_BIN_REQUEST:
            {
            char caEnterName[32] = {'\0'};
            strncpy(caEnterName,pdu->caData,32);
            char *pPath = new char[pdu->uiMsgLen];
            memcpy(pPath,pdu->caMsg,pdu->uiMsgLen);
            QString strPath = QString("%1/RecycleBin/%2").arg(pPath).arg(caEnterName);
            QFileInfo fileInfo(strPath);
            qDebug() << "fileInfo.exists():" << fileInfo.exists();
            PDU *respdu = NULL;
            if(fileInfo.isDir())
            {
                QDir dir(strPath);
                QFileInfoList fileInfoList = dir.entryInfoList();
                int iFileCount = fileInfoList.size();
                respdu = mkPDU(sizeof(FileInfo) * iFileCount);
                respdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_BIN_RESPOND;
                FileInfo *pFileInfo = NULL;
                QString strFileName;
                for(int i = 0;i<fileInfoList.size();i++)
                {
                    pFileInfo = (FileInfo*)(respdu->caMsg) + i;
                    strFileName = fileInfoList[i].fileName();
                    memcpy(pFileInfo->caFileName,strFileName.toStdString().c_str(),strFileName.size());
                    if(fileInfoList[i].isDir())
                    {
                        pFileInfo->iFileType = 0;
                    }
                    else if(fileInfoList[i].isFile())
                    {
                        pFileInfo->iFileType = 1;
                    }
                }
                write((char*)respdu,respdu->uiPDULen);
                free(respdu);
                respdu = NULL;
            }
            else if(fileInfo.isFile())
            {
                respdu = mkPDU(0);
                respdu->uiMsgType = ENUM_MSG_TYPE_ENTER_BIN_RESPOND;
                strcpy(respdu->caData,ENTER_DIR_FAILURED);
                write((char*)respdu,respdu->uiPDULen);
                free(respdu);
                respdu = NULL;
            }
            break;
            }
            //恢复回收站文件请求
            case ENUM_MSG_TYPE_RECOVER_REQUEST:
            {
                char caFileName[32] = {'\0'};
                int srcLen = 0;
                int destLen = 0;
                sscanf(pdu->caData,"%d%d%s",&srcLen,&destLen,caFileName);
                char *pSrcPath = new char[srcLen + 1];
                char *pDestPath = new char[destLen + 1 + 32];
                memset(pSrcPath,'\0',srcLen + 1);
                memset(pDestPath,'\0',destLen + 1 + 32);
                memcpy(pSrcPath,pdu->caMsg,srcLen);
                memcpy(pDestPath,(char*)(pdu->caMsg) + (srcLen + 1),destLen);
                PDU *respdu = mkPDU(0);
                respdu->uiMsgType = ENUM_MSG_TYPE_RECOVER_RESPOND;
                QFileInfo fileInfo(pDestPath);
                if(fileInfo.isDir())
                {
                    strcat(pDestPath,"/");
                    strcat(pDestPath,caFileName);
                    bool res = QFile::rename(pSrcPath,pDestPath);
                    if(res)
                    {
                        strcpy(respdu->caData,"Recover OK");
                    }
                    else
                    {
                        strcpy(respdu->caData,"Common Error");
                    }
                }
                else if(fileInfo.isFile())
                {
                    strcpy(respdu->caData,"Recover failed");
                }
                write((char*)respdu,respdu->uiPDULen);
                free(respdu);
                respdu = NULL;
                break;
            }
            //VIP删除文件夹请求
            case ENUM_MSG_TYPE_VIP_DEL_DIR_REQUEST:
            {
                char caFileName[32] = {'\0'};
                int srcLen = 0;
                int destLen = 0;
                sscanf(pdu->caData, "%d%d%s", &srcLen, &destLen, caFileName);
                char *pSrcPath = new char[srcLen + 1];
                char *pDestPath = new char[destLen + 1 + 32];
                memset(pSrcPath, '\0', srcLen + 1);
                memset(pDestPath, '\0', destLen + 1 + 32);
                memcpy(pSrcPath, pdu->caMsg, srcLen);
                memcpy(pDestPath, (char*)(pdu->caMsg) + (srcLen + 1), destLen);

                PDU *respdu = mkPDU(0);
                respdu->uiMsgType = ENUM_MSG_TYPE_VIP_DEL_DIR_RESPOND;
                QDir destDir(pDestPath);
                if (destDir.exists()) {
                    strcat(pDestPath, "/");
                    strcat(pDestPath, caFileName);
                    bool res = QFile::rename(pSrcPath, pDestPath);
                    if (res) {
                        strcpy(respdu->caData, "Folder Delete OK");
                    } else {
                        strcpy(respdu->caData, "Folder Delete Failed");
                    }
                } else {
                    strcpy(respdu->caData, "Invalid Destination Path");
                }
                write((char*)respdu, respdu->uiPDULen);
                free(respdu);
                respdu = NULL;
                delete[] pSrcPath;
                delete[] pDestPath;
                break;
            }
            //群聊相关--------------------------------------------
            case ENUM_MSG_TYPE_GROUP_CHAT_REQUEST:
            {
                char groupName[32] = {'\0'};
                strncpy(groupName,pdu->caData+32,32);
                QStringList groupMember = OpeDB::getInstance().handleGetOnlineGroupMember(groupName);
                QString tempName;
                for(int i=0;i< groupMember.size();i++)
                {
                    tempName = groupMember.at(i);
                    MyTcpServer::getInstance().resend(tempName.toStdString().c_str(),pdu);
                }
                break;
            }
            //创建群聊请求
            case ENUM_MSG_TYPE_CREAT_GROUP_REQUEST:
            {
                if (pdu == nullptr || pdu->uiMsgLen == 0) {
                        qDebug() << "Invalid PDU or no message length." ;
                        break;
                }
                char memberName[32] = {'\0'};
                int num = 0;
                sscanf(pdu->caData,"%s%d",memberName,&num);
                int group_id = OpeDB::getInstance().handleGroupID() + 1;
                qDebug() << "group_id: " << group_id;
                OpeDB::getInstance().handleAddGroup(group_id, memberName);

                // 遍历所有复选框文本
                for (size_t i = 0; i < (size_t) num * 32; i += 32) {
                    char checkboxText[32] = {'\0'};
                    // 使用strncpy从PDU中提取文本
                    strncpy(checkboxText, (char*)pdu->caMsg + i, 32);
                    OpeDB::getInstance().handleAddGroup(group_id, checkboxText);
                }
                break;
            }

                //获取群成员请求
            case ENUM_MSG_TYPE_UPDATE_MEMBER_LIST_REQUEST:
            {
                char caName[32] = {'\0'};
                strncpy(caName,pdu->caData,32);
                QStringList ret = OpeDB::getInstance().handleGetOnlineGroupMember(caName);
                uint uiMsgLen = ret.size()*32;
                PDU *respdu = mkPDU(uiMsgLen);
                respdu->uiMsgType = ENUM_MSG_TYPE_UPDATE_MEMBER_LIST_RESPOND;
                for(int i=0;i<ret.size();i++)
                {
                    memcpy((char*)(respdu->caMsg)+i*32,ret.at(i).toStdString().c_str(),ret.at(i).size());
                }
                this->write((char*)respdu,respdu->uiPDULen);
                free(respdu);
                respdu = NULL;
                break;
            }
            //注销请求
            case ENUM_MSG_TYPE_CANCEL_REQUEST:
            {
                char caName[32] = {'\0'};
                char caPwd[32] = {'\0'};
                strncpy(caName,pdu->caData,32);
                strncpy(caPwd,pdu->caData+32,32);
                QString userName = QString::fromStdString(pdu->caData);
                //删除数据库中的记录
                bool success = OpeDB::getInstance().handleCancelUsr(caName, caPwd);
                PDU *respdu = mkPDU(0);
                respdu->uiMsgType = ENUM_MSG_TYPE_CANCEL_RESPOND;
                if (success)
                {
                    strcpy(respdu->caData, "Cancel successful.");
                    QDir dir("./" + userName);
                    if (dir.exists()) {
                        dir.removeRecursively();
                        qDebug() << "User directory have been removed.";
                    }
                }
                else
                {
                    strcpy(respdu->caData, "Cancel failed.");
                }
                write((char*)respdu, respdu->uiPDULen);
                free(respdu);
                respdu = NULL;
                break;
            }
            // 修改密码
            case ENUM_MSG_TYPE_CHANGE_PWD_REQUEST:
            {
                char caName[32] = {'\0'};
                char caPwd[32] = {'\0'};
                strncpy(caName,pdu->caData,32);
                strncpy(caPwd,pdu->caData+32,32);

                char caNewPwd[32] = {'\0'};

                // 从 pdu->caMsg 中读取新密码
                strncpy(caNewPwd, (char*)pdu->caMsg, pdu->uiMsgLen);

                PDU *resPdu = mkPDU(0);
                resPdu->uiMsgType = ENUM_MSG_TYPE_CHANGE_PWD_RESPOND;


                qDebug() << "Received username: " << caName;
                qDebug() << "Received old password: " << caPwd;
                qDebug() << "Received new password: " << caNewPwd;

                //    返回 0：密码更新成功。
                //    返回 1：用户名不存在。
                //    返回 2：旧密码不匹配。
                //    返回 3：密码更新失败（数据库操作失败）
                int flag=OpeDB::getInstance().handleChangePWD(caName,caPwd,caNewPwd);


                switch (flag) {
                    case 0:  // 密码更新成功
                        strcpy(resPdu->caData, CHANGE_PWD_OK);
                        break;
                    case 1:  // 用户名不存在
                        strcpy(resPdu->caData, CHANGE_PWD_USR_NOT_EXIST);
                        break;
                    case 2:  // 旧密码不匹配
                        strcpy(resPdu->caData, CHANGE_PWD_PWD_MISMATCH);
                        break;
                    case 3:  // 密码更新失败（数据库操作失败）
                        strcpy(resPdu->caData, CHANGE_PWD_FAILED);
                        break;
                    default:
                        strcpy(resPdu->caData, UNKNOW_ERROR);
                        break;
                }

                write((char*)resPdu, resPdu->uiPDULen);
                free(resPdu);
                resPdu = NULL;


                break;
            }
            // 申请VIP
            case ENUM_MSG_TYPE_VIP_APPLY_REQUEST:
            {
                const char* applicationContent = (char*)pdu->caMsg;
                int reply;
                QMetaObject::invokeMethod(
                    &TcpServer::getInstance(), // 主线程的 QObject 实例
                    [applicationContent,&reply]() {
                        // 确保 UI 操作在主线程中执行
                        QWidget *ui = TcpServer::getInstance().getUI();
                        if (ui) {
                            reply = QMessageBox::question(
                                ui,  // 父窗口
                                tr("Confirm"),
                                tr("The application is:\n") + applicationContent + tr("\n\nApply or not？"),
                                QMessageBox::Yes | QMessageBox::No,
                                QMessageBox::No
                            );
                        }
                         emit TcpServer::getInstance().replyready(reply);

                    },
                    Qt::QueuedConnection  // 确保 lambda 在主线程中执行
                );
                break;
            }
        case ENUM_MSG_TYPE_FLUSH_NONMEMBER_FRIEND_REQUEST:
                {
                    char sendName[32] = {'\0'};
                    strncpy(sendName,pdu->caData,32);
                    char groupName[32] = {'\0'};
                    strncpy(groupName,pdu->caData + 32,32);
                    QStringList ret = OpeDB::getInstance().handleGetNonMemberFriend(sendName,groupName);
                    uint uiMsgLen = ret.size()*32;
                    PDU *respdu = mkPDU(uiMsgLen);
                    respdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_NONMEMBER_FRIEND_RESPOND;
                    for(int i=0;i<ret.size();i++)
                    {
                        memcpy((char*)(respdu->caMsg)+i*32,ret.at(i).toStdString().c_str(),ret.at(i).size());
                    }
                    this->write((char*)respdu,respdu->uiPDULen);
                    free(respdu);
                    respdu = NULL;
                    break;
                }
            case ENUM_MSG_TYPE_ADD_MEMBER_REQUEST:
            {
                if (pdu == nullptr || pdu->uiMsgLen == 0) {
                        qDebug() << "Invalid PDU or no message length." ;
                        break;
                }
                char groupName[32] = {'\0'};
                int num = 0;
                strncpy(groupName,pdu->caData,32);
                sscanf(pdu->caData + 32,"%d",&num); //%s 会读取到空格 ，invalid

                // 遍历所有复选框文本
                for (size_t i = 0; i < (size_t) num * 32; i += 32) {
                    char checkboxText[32] = {'\0'};
                    // 使用strncpy从PDU中提取文本
                    strncpy(checkboxText, (char*)pdu->caMsg + i, 32);
                    OpeDB::getInstance().handleAddGroup(groupName, checkboxText);
                    //给进入群聊的人发送
                    PDU *respdu = mkPDU(0);
                    respdu->uiMsgType = ENUM_MSG_TYPE_ENTER_GROUP_RESPOND;
                    QString resMsg = QString("You are invited into %1.").arg(groupName);
                    strcpy(respdu->caData,resMsg.toStdString().c_str());
                    this->write((char*)respdu,pdu->uiPDULen);
                    MyTcpServer::getInstance().resend(checkboxText,pdu);
                    free(respdu);
                    respdu = NULL;
                }
                break;
            }
            case ENUM_MSG_TYPE_CHANGE_GROUP_NAME_REQUEST:
                {
                    char sendName[32] = {'\0'};
                    char oldGroupName[32] = {'\0'};
                    char newName[32] = {'\0'};
                    strncpy(sendName,pdu->caData,32);
                    strncpy(oldGroupName,pdu->caData + 32,32);
                    strncpy(newName,pdu->caData + 64,32);
                    OpeDB::getInstance().handleChangeGroupName(oldGroupName,newName);

                    //让每个群成员刷新界面
                    QStringList groupMember = OpeDB::getInstance().handleGetOnlineGroupMember(newName);
                    pdu->uiMsgType = ENUM_MSG_TYPE_CHANGE_GROUP_NAME_RESPOND;
                    strcpy(pdu->caData,newName);
                    strcpy(pdu->caData + 32,oldGroupName);
                    strcpy(pdu->caData + 64,sendName);
                    //用resend
                    QString tempName;
                    for(int i=0;i< groupMember.size();i++)
                    {
                        tempName = groupMember.at(i);
                        MyTcpServer::getInstance().resend(tempName.toStdString().c_str(),pdu);
                        qDebug() << "resend给" << tempName;
                    }

                    break;
                }
        case ENUM_MSG_TYPE_SEARCH_FILE_REQUEST:
            {
            // Extract username and keyword from pdu->caData
                char caUserName[32] = {'\0'};
                char caKeyword[32] = {'\0'};
                strncpy(caUserName, pdu->caData, 32);
                strncpy(caKeyword, pdu->caData + 32, 32);

                // Extract current path from pdu->caMsg
                char *pCurPath = new char[pdu->uiMsgLen + 1];
                memset(pCurPath, '\0', pdu->uiMsgLen + 1);
                memcpy(pCurPath, pdu->caMsg, pdu->uiMsgLen);
                QString strCurPath = QString::fromUtf8(pCurPath);
                delete []pCurPath;
                pCurPath = NULL;

                // Construct the search path
                QString strSearchPath = strCurPath;

                // Perform recursive search
                QStringList foundFiles;
                searchFile(strSearchPath, QString::fromUtf8(caKeyword), foundFiles);

                // Prepare and send response
                PDU *respdu = NULL;
                if (!foundFiles.isEmpty())
                {
                    QString resultData = foundFiles.join(';');
                    QByteArray baResultData = resultData.toUtf8();

                    respdu = mkPDU(baResultData.size() + 1);
                    respdu->uiMsgType = ENUM_MSG_TYPE_SEARCH_FILE_RESPOND;
                    memcpy(respdu->caMsg, baResultData.data(), baResultData.size());
                }
                else
                {
                    respdu = mkPDU(0);
                    respdu->uiMsgType = ENUM_MSG_TYPE_SEARCH_FILE_RESPOND;
                    strcpy(respdu->caData, "No matching files or folders found");
                }

                write((char*)respdu, respdu->uiPDULen);
                free(respdu);
                respdu = NULL;

                break;
            }
            //查看压缩和解压缩信息
            case ENUM_MSG_TYPE_ZIP_REQUEST:
            {
                char *pCurPath = new char[pdu->uiMsgLen];
                memcpy(pCurPath,pdu->caMsg,pdu->uiMsgLen);
                bool compress_status=compressFile( QString::fromUtf8(pCurPath));
                PDU *respdu = mkPDU(0);
                respdu->uiMsgType = ENUM_MSG_TYPE_ZIP_RESPOND;
                if (compress_status){
                    strcpy(respdu->caData,"Zip OK");

                }else{
                    strcpy(respdu->caData,"Zip Failed");

                }
                write((char*)respdu, respdu->uiPDULen);
                free(respdu);
                respdu = NULL;

                break;
            }
            case ENUM_MSG_TYPE_UNZIP_REQUEST:
            {
                char *pCurPath = new char[pdu->uiMsgLen];
                memcpy(pCurPath,pdu->caMsg,pdu->uiMsgLen);
                bool compress_status=decompressFile( QString::fromUtf8(pCurPath));
                PDU *respdu = mkPDU(0);
                respdu->uiMsgType = ENUM_MSG_TYPE_UNZIP_RESPOND;
                if (compress_status){
                    strcpy(respdu->caData,"UnZip OK");

                }else{
                    strcpy(respdu->caData,"UnZip Failed");

                }
                write((char*)respdu, respdu->uiPDULen);
                free(respdu);
                respdu = NULL;

                break;
            }
            default:
            {
                break;
            }
        }
        free(pdu);
        pdu = NULL;
    }
    else
    {
        PDU *respdu = NULL;
        respdu = mkPDU(0);
        respdu->uiMsgType = ENUM_MSG_TYPE_UPLOAD_FILE_RESPOND;
        QByteArray buff = readAll();
        m_file.write(buff);
        m_iRecved += buff.size();
        if(m_iTotal == m_iRecved)
        {
            m_file.close();
            m_bUpload = false;
            // -------适配-----头像更新-----------------------------------
            QString filePath = m_file.fileName();
            int lastSlashIndex = filePath.lastIndexOf('/');
            QString dirPath = filePath.left(lastSlashIndex);
            if (strcmp(dirPath.toStdString().c_str(), PIC_DIR_PATH) == 0){
                strcpy(respdu->caData,UPDATE_PIC_OK);
            }else{
                strcpy(respdu->caData,UPLOAD_FILE_OK);
            }
            // ----------------------------------------------------------
            write((char*)respdu,respdu->uiPDULen);
            free(respdu);
            respdu = NULL;
        }
        else if(m_iTotal < m_iRecved)
        {
            m_file.close();
            m_bUpload = false;
            strcpy(respdu->caData,UPLOAD_FILE_FAILURED);
            write((char*)respdu,respdu->uiPDULen);
            free(respdu);
            respdu = NULL;
        }
    }
}

// 处理客户端用户下线的信号
void MyTcpSocket::clientOffline()
{
    OpeDB::getInstance().handleOffline(m_strName.toStdString().c_str());
    emit offline(this);
}

// 向客户端发送文件数据
void MyTcpSocket::sendFileToClient()
{
    char *pData = new char[4096];
        qint64 res = 0;
        while(true)
        {
            res = m_file.read(pData,4096);
            if(res > 0 && res <= 4096)
            {
                write(pData,res);
            }
            else if(0 == res)
            {
                m_file.close();
                m_pTimer->stop(); // Stop the timer after completing the transfer
                break;
            }
            else if(0 > res)
            {
                qDebug() << "fail to send file to client";
                m_pTimer->stop(); // Stop the timer after completing the transfer
                m_file.close();
                break;
            }
        }
        delete []pData;
        pData = NULL;

//    char *pData = new char[4096];
//    qint64 res = 0;
//    while(true)
//    {
//        res = m_file.read(pData,4096);
//        if(res > 0 && res <= 4096)
//        {
//            write(pData,res);
//        }
//        else if(0 == res)
//        {
//            m_file.close();
//            break;
//        }
//        else if(0 > res)
//        {
//            qDebug() << "发送文件内容给客户端过程中失败";
//            m_file.close();
//            break;
//        }
//    }
//    delete []pData;
//    pData = NULL;
}

void MyTcpSocket::handleReply(int reply)
{
    if (reply == QMessageBox::Yes) {
        qDebug() << "VIP apply passed";
        bool res=OpeDB::getInstance().handleAddVIP(m_strName.toStdString().c_str());
        PDU* respdu = mkPDU(0);
        respdu->uiMsgType = ENUM_MSG_TYPE_VIP_APPLY_RESPOND;
        if (res){
            strcpy(respdu->caData,VIP_PASS);
        } else{
            strcpy(respdu->caData,UNKNOW_ERROR); // 可能是数据库操作失败
        }
        write((char*)respdu,respdu->uiPDULen);
        free(respdu);
        respdu = NULL;
    } else {
        qDebug() << "VIP apply refused";
        PDU* respdu = mkPDU(0);
        respdu->uiMsgType = ENUM_MSG_TYPE_VIP_APPLY_RESPOND;
        strcpy(respdu->caData,VIP_REJECT);
        write((char*)respdu,respdu->uiPDULen);
        free(respdu);
        respdu = NULL;
    }
}

void MyTcpSocket::searchFile(const QString &strPath, const QString &keyword, QStringList &foundFiles)
{
    QDir dir(strPath);
    if (!dir.exists())
    {
        return;
    }

    QFileInfoList list = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
    for (const QFileInfo &fileInfo : list)
    {
        if (fileInfo.isDir())
        {
            // Check if directory name matches the keyword
            if (fileInfo.fileName().contains(keyword, Qt::CaseInsensitive))
            {
                foundFiles.append(fileInfo.absoluteFilePath());
            }
            // Recursively search in subdirectories
            searchFile(fileInfo.absoluteFilePath(), keyword, foundFiles);
        }
        else if (fileInfo.isFile())
        {
            if (fileInfo.fileName().contains(keyword, Qt::CaseInsensitive))
            {
                // Found a matching file
                foundFiles.append(fileInfo.absoluteFilePath());
            }
        }
    }
}


bool MyTcpSocket::compressFile(const QString &filePath) {

    QFileInfo fileInfo(filePath);
    QString dirPath = QFileInfo(filePath).path();
    QString zipFilePath = dirPath + "/" + QFileInfo(filePath).baseName() + ".zip";  // 输出的压缩文件名

    qDebug() << "File to be compressed:" << filePath;
    qDebug() << "Output zip file:" << zipFilePath;

    QProcess process;

    // 如果传入的是文件夹路径，需要递归压缩
    QStringList arguments;
    if (fileInfo.isDir()) {
        // 对目录进行递归压缩，使用 -r 参数
        arguments << "-r" << zipFilePath << filePath;
    } else {
        // 对单个文件进行压缩
        arguments << zipFilePath << filePath;
    }

    // 启动压缩进程
    process.start("zip", arguments);
    process.waitForFinished();  // 等待命令执行完毕

    // 捕获标准输出和标准错误
    QString output = process.readAllStandardOutput();
    QString errorOutput = process.readAllStandardError();

    qDebug() << "Process output: " << output;
    qDebug() << "Error output: " << errorOutput;

    if (process.exitStatus() == QProcess::NormalExit) {
        qDebug() << "File compressed successfully to" << zipFilePath;

        // 删除源文件或目录
        if (fileInfo.isDir()) {
            QDir dir(filePath);
            if (dir.removeRecursively()) {
                qDebug() << "Directory deleted successfully.";
            } else {
                qDebug() << "Failed to delete directory.";
            }
        } else {
            if (QFile::remove(filePath)) {
                qDebug() << "Source file deleted successfully.";
            } else {
                qDebug() << "Failed to delete source file.";
            }
        }

        return true;  // 压缩成功，返回 true
    } else {
        qDebug() << "Compression failed!";
        return false;  // 压缩失败，返回 false
    }
}

bool MyTcpSocket::decompressFile(const QString &zipFilePath) {
    //    QString destDir = QFileInfo(zipFilePath).path();
    QProcess process;

    // 调用 unzip 命令行工具进行文件解压
    QStringList arguments;
    arguments << "-d" << "./" << zipFilePath;
    process.start("unzip", arguments);
    process.waitForFinished();  // 等待命令执行完毕

    // 捕获标准输出和标准错误
    QString output = process.readAllStandardOutput();
    QString errorOutput = process.readAllStandardError();
    qDebug() << "Process output: " << output;
    qDebug() << "Error output: " << errorOutput;

    if (process.exitStatus() == QProcess::NormalExit) {
        if (QFile::remove(zipFilePath)) {
            qDebug() << "Source file deleted successfully.";
        } else {
            qDebug() << "Failed to delete source file.";
        }
        return true;  // 解压成功，返回 true
    } else {
        qDebug() << "Decompression failed!";
        return false;  // 解压失败，返回 false
    }
}

