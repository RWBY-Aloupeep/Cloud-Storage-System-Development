#include "tcpclient.h"
#include "ui_tcpclient.h"
#include <QByteArray>
#include <QDebug>
#include <QMessageBox>
#include <QHostAddress>
#include "opewidget.h"
#include "privatechat.h"
#include "aescbc.h"
#include <QDir>
#include "groupchat.h"
#include "editinfo.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QResizeEvent>
#include <QTime>

TcpClient::TcpClient(QWidget *parent) : QWidget(parent), ui(new Ui::TcpClient)
{
    ui->setupUi(this);

    resize(600,500);
    loadConfig();
    connect(&m_tcpSocket,SIGNAL(connected()),this,SLOT(showConnect()));
    connect(&m_tcpSocket,SIGNAL(readyRead()),this,SLOT(recvMsg()));
    // 绑定 disconnected 信号到槽函数 onDisconnected()
    connect(&m_tcpSocket, &QTcpSocket::disconnected, this, &TcpClient::onDisconnected);
    connect(ui->pwd_le, SIGNAL(returnPressed()), this, SLOT(on_login_pb_clicked())); // 当光标输入完pwd后点enter可以起作用
    // 连接服务器
    m_tcpSocket.connectToHost(QHostAddress(m_strIP),m_usPort);


}

TcpClient::~TcpClient()
{
    delete ui;
}

// 加载配置文件
void TcpClient::loadConfig()
{
    QFile file(":/client.config");
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray baData = file.readAll();
        QString strData = baData.toStdString().c_str();
        strData.replace("\r\n"," ");
        QStringList strList = strData.split(" ");
        m_strIP = strList.at(0);
        m_usPort = strList.at(1).toUShort();
        qDebug() << "IP:" << m_strIP << "Port:" << m_usPort;
        file.close();
    }
    else
    {
        QMessageBox::critical(this,"open config","open config failed");
    }
}


// 返回TcpClient实例对象
TcpClient &TcpClient::getInstance()
{
    static TcpClient instance;
    return instance;
}

// 获取TcpSocket
QTcpSocket &TcpClient::getTcpSocket()
{
    return m_tcpSocket;
}

// 获取当前登录用户的用户名
QString TcpClient::loginName()
{
    return m_strLoginName;
}

// 返回当前文件夹目录
QString TcpClient::curPath()
{
    return m_strCurPath;
}

// 设置当前目录位置
void TcpClient::setCurPath(QString strCurPath)
{
    m_strCurPath = strCurPath;
}

QPixmap TcpClient::getpixmap()
{
    return pic_pixmap;
}

int TcpClient::getVIPStatus()
{
    return VIP_status;
}

// 获取和服务器连接结果
void TcpClient::showConnect()
{
    QMessageBox::information(this,"Connect to Server","Success");
}

# if 0
// 发送点击事件
void TcpClient::on_send_pd_clicked()
{
    QString strMsg = ui->lineEdit->text();
    if(!strMsg.isEmpty())
    {
        PDU *pdu = mkPDU(strMsg.size());
        pdu->uiMsgType = 8888;
        memcpy(pdu->caMsg,strMsg.toStdString().c_str(),strMsg.size());
        m_tcpSockey.write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu = NULL;
    }
    else
    {
        QMessageBox::warning(this,"信息发送","发送的信息不能为空");
    }
}
# endif

// 登录事件
void TcpClient::on_login_pb_clicked()
{
    QString strName = ui->name_le->text();
    QString strPwd = ui->pwd_le->text();
    if(!strName.isNull() && !strPwd.isNull())
    {
        m_strLoginName = strName;
        PDU *pdu = mkPDU(0);
        pdu->uiMsgType = ENUM_MSG_TYPE_LOGIN_REQUEST;
        strncpy(pdu->caData,strName.toStdString().c_str(),32);
        strncpy(pdu->caData + 32,strName.toStdString().c_str(),32);
        m_tcpSocket.write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu = NULL;
    }
    else
    {
        QMessageBox::critical(this,"Login:","Login failed: name or pwd is empty");
    }
}

// 注册事件
void TcpClient::on_regist_pb_clicked()
{
    QString strName = ui->name_le->text();
    QString strPwd = ui->pwd_le->text();
    if(!strName.isNull() && !strPwd.isNull())
    {
        PDU *pdu = mkPDU(0);
        pdu->uiMsgType = ENUM_MSG_TYPE_REGIST_REQUEST;
        strncpy(pdu->caData,strName.toStdString().c_str(),32);
        strncpy(pdu->caData + 32,strName.toStdString().c_str(),32);
        m_tcpSocket.write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu = NULL;
    }
    else
    {
        QMessageBox::critical(this,"Register","Register failed: name or pwd is empty");
    }
}

// 注销事件
void TcpClient::on_cancel_pb_clicked()
{
    QString strName = ui->name_le->text();
    QString strPwd = ui->pwd_le->text();
    if (!strName.isNull() && !strName.isEmpty() && !strPwd.isNull() && !strPwd.isEmpty())
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirm Cancel",
                                      "Are you sure to cancel you account?",
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            PDU *pdu = mkPDU(0);
            pdu->uiMsgType = ENUM_MSG_TYPE_CANCEL_REQUEST;
            strncpy(pdu->caData, strName.toStdString().c_str(), 32);
            strncpy(pdu->caData + 32,strPwd.toStdString().c_str(),32);
            m_tcpSocket.write((char*)pdu, pdu->uiPDULen);
            free(pdu);
            pdu = NULL;
        }
        else
        {
            QMessageBox::information(this, "Cancel", "Account not cancel");
        }
    }
    else
    {
        QMessageBox::critical(this, "Cancel", "Cancel failed: username or pwd is empty");
    }
}

// 接收服务器信息
void TcpClient::recvMsg()
{
    if(!OpeWidget::getInstance().getBook()->getDownloadStatus())
    {
        qDebug() << m_tcpSocket.bytesAvailable();
        uint uiPDULen = 0;
        m_tcpSocket.read((char*)&uiPDULen,sizeof(uint));
        uint uiMsgLen = uiPDULen - sizeof(PDU);
        qDebug()<< uiMsgLen;
        PDU *pdu = mkPDU(uiMsgLen);
        m_tcpSocket.read((char*)pdu+sizeof(uint),uiPDULen-sizeof(uint));
        switch (pdu->uiMsgType)
        {
            // 注册回复
            case ENUM_MSG_TYPE_REGIST_RESPOND:
            {
                if(0 == strcmp(pdu->caData,REGIST_OK))
                {
                    QMessageBox::information(this,"Register",REGIST_OK);
                }
                else if(0 == strcmp(pdu->caData,REGIST_FAILED))
                {
                    QMessageBox::warning(this,"Register",REGIST_FAILED);
                }
                break;
            }
            // 登录回复
            case ENUM_MSG_TYPE_LOGIN_RESPOND:
            {
            if(0 == strcmp(pdu->caData,LOGIN_OK))
            {
                m_strCurPath = QString("./%1").arg(m_strLoginName);
        //                    qDebug() << "m_strCurPath" << m_strCurPath;
                QMessageBox::information(this,"Login",LOGIN_OK);
                OpeWidget::getInstance().show();
                this->hide();
                // Refine -----设置窗口标题为name------避免第一次登录输错出现bug--------
                OpeWidget::getInstance().setWindowTitle(TcpClient::getInstance().loginName());
                OpeWidget::getInstance().setWindowIcon(QIcon(":/icon/cloud.png")); // 换成自己的路径主要是前端设计，如果没有的话可以先忽略
                // ---------------------------------------------------------------


                // -----------------获得VIP状态并储存---------------------------
                VIP_status=pdu->caMsg[0];
                qDebug()<<VIP_status;

                // 判断 VIP 状态
                if (VIP_status == 1) {
                    OpeWidget::getInstance().getVIP()->setEnabled(true);  // 如果 VIP 状态为 1，则禁用 m_pVIP
                } else {
                    OpeWidget::getInstance().getVIP()->setEnabled(false);   // 否则启用 m_pVIP
                }
                // -----------------------------------------------------------

                // ================储存照片============================
                QByteArray restoredData;
                // 从 respdu->caMsg 中恢复数据
                restoredData = QByteArray::fromRawData((const char*)pdu->caMsg+sizeof(int), pdu->uiMsgLen-sizeof(int));
                //测试----
                if (restoredData.startsWith("\x89PNG")) {
                    qDebug() << "This is a PNG image!";
                } else if (restoredData.startsWith("\xFF\xD8\xFF")) {
                    qDebug() << "This is a JPEG image!";//会有点问题
                }
                //-------
                if (pic_pixmap.loadFromData(restoredData)) {
                    // 成功加载 QPixmap
                    qDebug() << "QPixmap restored successfully!";
                } else {
                    // 加载失败
                    qDebug() << "Failed to restore QPixmap from data!";
                }
        //                    qDebug()<<restoredData;
                // ================================================

                // --开始画背景图-------------------

                OpeWidget::getInstance().init_draw();

                // -------------------------------
            }


            else if(0 == strcmp(pdu->caData,LOGIN_FAILED))
            {
                QMessageBox::warning(this,"Login",LOGIN_FAILED);
            }
            break;
            }
            // 查看在线用户回复
            case ENUM_MSG_TYPE_ALL_ONLINE_RESPOND:
            {
                OpeWidget::getInstance().getFriend()->showAllOnlineUsr(pdu);
                break;
            }
            // 查看查找用户回复
            case ENUM_MSG_TYPE_SEARCH_USR_RESPOND:
            {
            qDebug() << "search user client";
                    if (0 == strcmp(SEARCH_USR_NO, pdu->caData))
                    {
                        QMessageBox::information(this, "Search", QString("%1: Not exist").arg(OpeWidget::getInstance().getFriend()->m_strSearchName));
                    }
                    else if (0 == strcmp(SEARCH_USR_ONLINE, pdu->caData))
                    {
                        QMessageBox msgBox;
                        msgBox.setText(QString("%1: Online").arg(OpeWidget::getInstance().getFriend()->m_strSearchName));
                        QPushButton *add_Button = new QPushButton("Add Friend");
                        msgBox.addButton(add_Button, QMessageBox::ActionRole);
                        connect(add_Button, &QPushButton::clicked, [this]() {
                            OpeWidget::getInstance().getFriend()->handleAddFriend(OpeWidget::getInstance().getFriend()->m_strSearchName);
                        });
                        // 添加退出按钮
                        QPushButton *exit_Button = msgBox.addButton("Exit", QMessageBox::RejectRole);
                        msgBox.exec();
                    }
                    else if (0 == strcmp(SEARCH_USR_OFFLINE, pdu->caData))
                    {
                        QMessageBox::information(this, "Search", QString("%1: Offline").arg(OpeWidget::getInstance().getFriend()->m_strSearchName));
                    }else {
                        qDebug() << pdu->caData;
                    }
                    break;
//                if(0 == strcmp(SEARCH_USR_NO,pdu->caData))
//                {
//                    QMessageBox::information(this, "Search", QString("%1: not exist").arg(OpeWidget::getInstance().getFriend()->m_strSearchName));
//                }
//                else if(0 == strcmp(SEARCH_USR_ONLINE,pdu->caData))
//                {
//                    QMessageBox::information(this,"Search",QString("%1: online").arg(OpeWidget::getInstance().getFriend()->m_strSearchName));
//                }
//                else if(0 == strcmp(SEARCH_USR_OFFLINE,pdu->caData))
//                {
//                    QMessageBox::information(this,"Search",QString("%1: offline").arg(OpeWidget::getInstance().getFriend()->m_strSearchName));
//                }
//                break;
            }
            // 查看添加好友回复
            case ENUM_MSG_TYPE_ADD_FRIEND_REQUEST:
            {
                char caName[32] = {'\0'};
                strncpy(caName,pdu->caData+32,32);
                qDebug() << "Add friend";
                int res = QMessageBox::information(this,"Add friend",QString("%1 want to add you as friend").arg(caName),QMessageBox::Yes,QMessageBox::No);
                qDebug() << "Result:" << res;
                PDU *respdu = mkPDU(0);
                memcpy(respdu->caData,pdu->caData,64);
                if(res == QMessageBox::Yes)
                {
                    respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_AGGREE;
                }
                else
                {
                    respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_REFUSE;
                }
                m_tcpSocket.write((char*)respdu,respdu->uiPDULen);
                free(respdu);
                respdu = NULL;
                break;
            }
            case ENUM_MSG_TYPE_ADD_FRIEND_RESPOND:
            {
                QMessageBox::information(this,"Add friend",pdu->caData);
                break;
            }
            case ENUM_MSG_TYPE_ADD_FRIEND_AGGREE:
            {
                char caPerName[32] = {'\0'};
                memcpy(caPerName, pdu->caData, 32);
                QMessageBox::information(this, "Add Friend", QString("Add %1 succeed").arg(caPerName));
                break;
            }
            case ENUM_MSG_TYPE_ADD_FRIEND_REFUSE:
            {
                char caPerName[32] = {'\0'};
                memcpy(caPerName, pdu->caData, 32);
                QMessageBox::information(this, "Add Friend", QString("Add %1 failed").arg(caPerName));
                break;
            }
            // 查看刷新在线好友列表回复
            case ENUM_MSG_TYPE_FLUSH_FRIEND_RESPOND:
            {
            if(!OpeWidget::getInstance().getFriend()->auto_flush_bool){
                        OpeWidget::getInstance().getFriend()->updateFriendList(pdu);
                    }
                    else{
                        OpeWidget::getInstance().getFriend()->updateFriendList(pdu);
                        OpeWidget::getInstance().getFriend()->auto_flush_bool = false;
                    }
                    break;
//                OpeWidget::getInstance().getFriend()->updateFriendList(pdu);
//                break;
            }
            case ENUM_MSG_TYPE_ENTER_GROUP_RESPOND:
            {
                QMessageBox::information(this,"Enter Group",pdu->caData);
                break;
            }
            // 查看删除好友回复
            case ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST:
            {
                char caName[32] = {'\0'};
                memcpy(caName,pdu->caData,32);
                QMessageBox::information(this,"Delete Friend",QString("%1 delete you as his friend").arg(caName));
                break;
            }
            case ENUM_MSG_TYPE_DELETE_FRIEND_RESPOND:
            {
                QMessageBox::information(this,"Delete Friend","Seccess");
                break;
            }
            // 查看私聊回复
            case ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST:
            {
                if(PrivateChat::getInstance().isHidden())
                {
                    PrivateChat::getInstance().show();
                }
                char caSendName[32] = {'\0'};
                memcpy(caSendName,pdu->caData,32);
        //        QString strSendName = caSendName;
                char caName[32] = {'\0'};
                memcpy(caName,pdu->caData + 32,32);
                QString winTitle = QString::fromLatin1(caSendName);
                winTitle.append(" & ");
                winTitle.append(QString::fromLatin1(caName));
                //私聊被打断，开启新窗口
                if(PrivateChat::getInstance().getChatName() != caSendName
                        && PrivateChat::getInstance().getChatName() != caName){
                    PrivateChat::getInstance().clearMsg();
                    PrivateChat::getInstance().setChatName(caSendName);
                }
                PrivateChat::getInstance().updateMsg(pdu);
                PrivateChat::getInstance().setWindowTitle(winTitle);
                break;
//                if(PrivateChat::getInstance().isHidden())
//                {
//                    PrivateChat::getInstance().show();
//                }
//                char caSendName[32] = {'\0'};
//                memcpy(caSendName,pdu->caData,32);
//                QString strSendName = caSendName;
//                PrivateChat::getInstance().setChatName(caSendName);
//                PrivateChat::getInstance().updateMsg(pdu);
//                break;
            }
            //查看群聊请求
            case ENUM_MSG_TYPE_GROUP_CHAT_REQUEST:
            {
                if(GroupChat::getInstance().isHidden())
                {
                    GroupChat::getInstance().show();
                }
                char groupName[32] = {'\0'};
                memcpy(groupName,pdu->caData + 32,32);

                if((GroupChat::getInstance().getGroupName() != QString::fromLatin1(groupName))
                      ||  GroupChat::getInstance().getGroupName() == NULL ){
                    if(GroupChat::getInstance().getGroupName() != QString::fromLatin1(groupName)
                            && GroupChat::getInstance().getGroupName() != NULL){

                    }
                    GroupChat::getInstance().clearMsg();
                    GroupChat::getInstance().clearMemberList();
                    GroupChat::getInstance().setGroupName(groupName);
                    GroupChat::getInstance().setWindowTitle(groupName);
                }
                GroupChat::getInstance().updateMsg(pdu);
                break;
            }
            case ENUM_MSG_TYPE_CREAT_GROUP_RESPOND:
            {
                QMessageBox::information(this, "Create Group", pdu->caData);
                break;
            }
            case ENUM_MSG_TYPE_UPDATE_MEMBER_LIST_RESPOND:
            {
                if(!GroupChat::getInstance().auto_flush_bool){
                    GroupChat::getInstance().updateOnlineMemberList(pdu);
                }
                else{
                    GroupChat::getInstance().updateOnlineMemberList(pdu);
                    GroupChat::getInstance().auto_flush_bool = false;
                }
                break;
            }
            // 查看群聊回复
//            case ENUM_MSG_TYPE_GROUP_CHAT_REQUEST:
//            {
//                OpeWidget::getInstance().getFriend()->updateGroupMsg(pdu);
//                break;
//            }
            // 查看创建文件夹回复
            case ENUM_MSG_TYPE_CREATE_DIR_RESPOND:
            {
                QMessageBox::information(this,"Create Folder",pdu->caData);
                break;
            }
            // 查看查看所有文件回复
            case ENUM_MSG_TYPE_FLUSH_FILE_RESPOND:
            {
                OpeWidget::getInstance().getBook()->updateFileList(pdu);
                QString strEnterDir = OpeWidget::getInstance().getBook()->enterDir();
                if(!strEnterDir.isEmpty())
                {
                    m_strCurPath = m_strCurPath + "/" + strEnterDir;
                }
                break;
            }
            // 查看删除文件夹回复
            case ENUM_MSG_TYPE_DEL_DIR_RESPOND:
            {
                QMessageBox::information(this,"Delete Folder",pdu->caData);
                break;
            }
            // 查看重命名文件回复
            case ENUM_MSG_TYPE_RENAME_FILE_RESPOND:
            {
                QMessageBox::information(this,"Rename file",pdu->caData);
                break;
            }
            // 查看进入文件夹回复
            case ENUM_MSG_TYPE_ENTER_DIR_RESPOND:
            {
                OpeWidget::getInstance().getBook()->clearEnterDir();
                QMessageBox::information(this,"Enter Folder",pdu->caData);
                break;
            }
            // 查看上传文件回复
            case ENUM_MSG_TYPE_UPLOAD_FILE_RESPOND:
            {
                QMessageBox::information(this,"Upload file",pdu->caData);
                // -------------------头像更新---------------------------------
                if (strcmp(pdu->caData, UPDATE_PIC_OK) == 0){
                    emit update_ok();
                }
                // --------------------------------------------------------
                break;
            }
            // 查看删除文件回复
            case ENUM_MSG_TYPE_DEL_FILE_RESPOND:
            {
                QMessageBox::information(this,"Delete file",pdu->caData);
                break;
            }
            // 查看下载文件回复
            case ENUM_MSG_TYPE_DOWNLOAD_FILE_RESPOND:
            {
                qDebug() << pdu->caData;
                char caFileName[32] = {'\0'};
                sscanf(pdu->caData,"%s %lld",caFileName,&(OpeWidget::getInstance().getBook()->m_iTotal));
                OpeWidget::getInstance().getBook()->m_iRecved=0; //----debug-----
                if(strlen(caFileName) > 0 && OpeWidget::getInstance().getBook()->m_iTotal > 0)
                {
                    OpeWidget::getInstance().getBook()->setDownloadStatus(true);
                    m_file.setFileName(OpeWidget::getInstance().getBook()->getSaveFilePath());
                    if(!m_file.open(QIODevice::WriteOnly))
                    {
                        QMessageBox::warning(this,"Download","fail to get path");
                    }
                }
                break;
            }
            // 查看分享文件回复
            case ENUM_MSG_TYPE_SHARE_FILE_RESPOND:
            {
                QMessageBox::information(this,"Share File",pdu->caData);
                break;
            }
            // 查看分享文件通知请求
            case ENUM_MSG_TYPE_SHARE_FILE_NOTE_REQUEST:
            {
                char *pPath = new char[pdu->uiMsgLen];
                memcpy(pPath,pdu->caMsg,pdu->uiMsgLen);
                char *pos = strrchr(pPath,'/');
                if(NULL != pos)
                {
                    pos++;
                    QString strNote = QString("%1 share file->%2 \n Do you accept?").arg(pdu->caData).arg(pos);
                    int res = QMessageBox::question(this,"Share File",strNote);
                    if(QMessageBox::Yes == res)
                    {
                        PDU *respdu = mkPDU(pdu->uiMsgLen);
                        respdu->uiMsgType = ENUM_MSG_TYPE_SHARE_FILE_NOTE_RESPOND;
                        memcpy(respdu->caMsg,pdu->caMsg,pdu->uiMsgLen);
                        QString strName = TcpClient::getInstance().loginName();
                        strcpy(respdu->caData,strName.toStdString().c_str());
                        m_tcpSocket.write((char*)respdu,respdu->uiPDULen);
                    }
                }
                break;
            }
            // 查看移动文件回复
            case ENUM_MSG_TYPE_MOVE_FILE_RESPOND:
            {
                QMessageBox::information(this,"Move File",pdu->caData);
                break;
            }
            //查看清空回收站回复
            case ENUM_MSG_TYPE_CLEAR_BIN_RESPOND:
            {
                QMessageBox::information(this, "Clear Recycle Bin", pdu->caData);
                break;
            }
            //查看回收站所有文件回复
            case ENUM_MSG_TYPE_FLUSH_BIN_RESPOND:
            {
                OpeWidget::getInstance().getVIP()->updateBinList(pdu);
                break;
            }
            //回收站点击文件夹进入
            case ENUM_MSG_TYPE_ENTER_BIN_RESPOND:
            {
                OpeWidget::getInstance().getVIP()->clearEnterBin();
                QMessageBox::information(this,"Enter Bin",pdu->caData);
                break;
            }
            case ENUM_MSG_TYPE_RECOVER_RESPOND:
            {
                QMessageBox::information(this,"Recover",pdu->caData);
                break;
            }
            //VIP删除文件夹回复
            case ENUM_MSG_TYPE_VIP_DEL_DIR_RESPOND:
            {
                QMessageBox::information(this,"Delete Folder",pdu->caData);
                break;
            }
            // 注销回复
            case ENUM_MSG_TYPE_CANCEL_RESPOND:
            {
                if (0 == strcmp(pdu->caData, "Cancel successful."))
                {
                    QMessageBox::information(this, "Cancel", "You have been canceled successfully.");
                }
                else if (0 == strcmp(pdu->caData, "Cancel failed."))
                {
                    QMessageBox::warning(this, "Cancel", "Cancel failed. Please try again.");
                }
                break;
            }
            //更改密码回复
            case ENUM_MSG_TYPE_CHANGE_PWD_RESPOND:
            {
                QMessageBox::information(this,"Update PWD",pdu->caData);
                break;
            }
            //申请VIP回复
            case ENUM_MSG_TYPE_VIP_APPLY_RESPOND:
            {
                QMessageBox::information(this,"Approval Reply",pdu->caData);
                if (strcmp(pdu->caData, VIP_PASS) == 0){
                     VIP_status=1;
                     EditInfo::getInstance().set_VIP_status("true");
                }
                break;
            }
            case ENUM_MSG_TYPE_FLUSH_NONMEMBER_FRIEND_RESPOND:
            {
                if(!GroupChat::getInstance().auto_flush_addMember){
                    GroupChat::getInstance().updateFriendList(pdu);
                }
                else{
                    GroupChat::getInstance().updateFriendList(pdu);
                    GroupChat::getInstance().auto_flush_addMember = false;
                }
                break;
            }
            case ENUM_MSG_TYPE_CHANGE_GROUP_NAME_RESPOND:
            {
                char newGroupName[32] = {'\0'};
                memcpy(newGroupName,pdu->caData,32);
                char oldGroupName[32] = {'\0'};
                memcpy(oldGroupName,pdu->caData + 32,32);
                char caName[32] = {'\0'};
                memcpy(caName,pdu->caData + 64,32);
                OpeWidget::getInstance().getFriend()->changeNameinList(oldGroupName,newGroupName);
                GroupChat::getInstance().setGroupName(newGroupName);
                GroupChat::getInstance().setWindowTitle(newGroupName);
                GroupChat::getInstance().updateChangeNameMsg(caName, newGroupName);
                break;
            }
            case ENUM_MSG_TYPE_SEARCH_FILE_RESPOND:
            {
                OpeWidget::getInstance().getBook()->handleSearchFileRespond(pdu);
                break;
            }
            //压缩和解压缩
            case ENUM_MSG_TYPE_ZIP_RESPOND:
            {
                if (0 == strcmp(pdu->caData, "Zip OK"))
                {
                    QMessageBox::information(this, "Zip", "File has been ziped successfully.");
                }
                else if (0 == strcmp(pdu->caData, "Cancel failed."))
                {
                    QMessageBox::warning(this, "Zip", "ZIP failed. Please try again.");
                }
                break;

            }
            case ENUM_MSG_TYPE_UNZIP_RESPOND:
           {
               if (0 == strcmp(pdu->caData, "UnZip OK"))
               {
                   QMessageBox::information(this, "UnZip", "File has been unziped successfully.");
               }
               else if (0 == strcmp(pdu->caData, "Cancel failed."))
               {
                   QMessageBox::warning(this, "UnZip", "UnZIP failed. Please try again.");
               }
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
        // refine -------------------------解密--------------------------------
        QByteArray buff = m_tcpSocket.readAll();
        all_buff += buff;
        Book *pBook = OpeWidget::getInstance().getBook();
        pBook->m_iRecved += buff.size();

        // Update progress bar
        int progress = static_cast<int>((pBook->m_iRecved * 100) / pBook->m_iTotal);
        pBook->m_pDownloadProgressBar->setValue(progress);
        pBook->m_pPercentageLabel->setText(QString("%1%").arg(progress));
        pBook->m_pDownloadProgressBar->setVisible(true);
        pBook->m_pPercentageLabel->setVisible(true);

        // Estimate remaining time
        static QTime lastUpdateTime = QTime::currentTime();
        static qint64 lastRecvedSize = 0;

        QTime currentTime = QTime::currentTime();
        qint64 elapsedTimeMs = lastUpdateTime.msecsTo(currentTime);
        qint64 newlyReceived = pBook->m_iRecved - lastRecvedSize;

        if (elapsedTimeMs > 100 && newlyReceived > 0) // Update every 500ms
        {
            double downloadSpeed = (double)newlyReceived / (elapsedTimeMs / 1000.0); // Bytes per second
            double remainingTime = (pBook->m_iTotal - pBook->m_iRecved) / downloadSpeed; // Seconds remaining

            QString timeText = "Time Remaining: ";
            if (remainingTime > 60)
                timeText += QString("%1 min %2 sec").arg((int)remainingTime / 60).arg((int)remainingTime % 60);
            else
                timeText += QString("%1 sec").arg((int)remainingTime);

            pBook->m_pTimeRemainingLabel->setText(timeText);
            pBook->m_pTimeRemainingLabel->setVisible(true);

            lastUpdateTime = currentTime;
            lastRecvedSize = pBook->m_iRecved;
        }

        if (pBook->m_iTotal == pBook->m_iRecved) // Download complete
        {
            std::vector<unsigned char> encrypt_text(all_buff.begin(), all_buff.end());
            std::vector<unsigned char> decrypted_text = AESCBC::aesDecrypt(encrypt_text);
            QByteArray decrypted_buff(reinterpret_cast<const char*>(decrypted_text.data()), decrypted_text.size());
            m_file.write(decrypted_buff);
            all_buff.clear();
            m_file.close();
            pBook->m_iTotal = 0;
            pBook->m_iRecved = 0;
            pBook->setDownloadStatus(false);
            pBook->m_pDownloadProgressBar->setVisible(false);
            pBook->m_pPercentageLabel->setVisible(false);
            pBook->m_pTimeRemainingLabel->setVisible(false);
            QMessageBox::information(this, "Download File", "Successfully downloaded file.");
        }
        else if (pBook->m_iTotal < pBook->m_iRecved) // Error in download
        {
            m_file.close();
            pBook->m_iTotal = 0;
            pBook->m_iRecved = 0;
            pBook->setDownloadStatus(false);
            pBook->m_pDownloadProgressBar->setVisible(false);
            pBook->m_pPercentageLabel->setVisible(false);
            pBook->m_pTimeRemainingLabel->setVisible(false);
            QMessageBox::critical(this, "Download File", "Failed to download file.");
        }
    }

//    else
//    {
//        // refine -------------------------解密--------------------------------
//        QByteArray buff = m_tcpSocket.readAll();
//        all_buff += buff;
//        Book *pBook = OpeWidget::getInstance().getBook();
//        pBook->m_iRecved += buff.size();
//        if (pBook->m_iTotal==pBook->m_iRecved)
//        {
//            std::vector<unsigned char> encrypt_text(all_buff.begin(), all_buff.end());
//            std::vector<unsigned char> decrypted_text = AESCBC::aesDecrypt(encrypt_text);
//            QByteArray decrypted_buff(reinterpret_cast<const char*>(decrypted_text.data()), decrypted_text.size());
//            m_file.write(decrypted_buff);
//            all_buff.clear();
//            m_file.close();
//            pBook->m_iTotal = 0;
//            pBook->m_iRecved = 0;
//            pBook->setDownloadStatus(false);
//            QMessageBox::information(this, "Download File", "Success");
//        }
//        // ---------------------------------------------------

//        else if (pBook->m_iTotal < pBook->m_iRecved)
//        {
//            m_file.close();
//            pBook->m_iTotal = 0;
//            pBook->m_iRecved = 0;
//            pBook->setDownloadStatus(false);
//            QMessageBox::critical(this, "Download File", "Download failed");
//            }
//    }
}

void TcpClient::onDisconnected()
{
    // 当连接断开时，执行的逻辑
    OpeWidget::getInstance().setWindowTitle(loginName()+" (disconnected)");
    qDebug() << "Connection to the server has been disconnected.";
    QMessageBox::warning(this, "Disconnected", "Connection to the server lost.");
}
