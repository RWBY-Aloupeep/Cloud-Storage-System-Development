#include "groupchat.h"
#include "ui_groupchat.h"
#include "protocol.h"
#include "tcpclient.h"
#include <QMessageBox>
#include <QTimer>
#include "addmember.h"
#include <QEventLoop>

GroupChat::GroupChat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupChat)

{
    ui->setupUi(this);
    auto_flush_bool = false;

    this->resize(800, 500);
    connect(ui->onlineMemList_lw,&QListWidget::itemDoubleClicked,this,&GroupChat::on_at_pb_doubleClicked);
    connect(ui->changeName_pb, &QPushButton::clicked, this, &GroupChat::on_changeName_pb_clicked);

    m_pNonMemberFriendListWidget = new QListWidget;

    QString styleSheet = R"(
        QWidget {
            background-color: rgba(255, 255, 255, 1); /* 主背景白色 */
            font-family: 'Arial', Times, serif; /* 字体设置为 Arial */
            font-size: 20px; /* 字体增大 */
             border: 2px solid rgba(64, 83, 97, 1); /* 边框颜色 */
             border-radius: 5px; /* 边框圆角 */
             padding: 8px; /* 内边距 */
             margin: 8px; /* 外边距 */
        }

        QPushButton {
            background-color: rgba(64, 83, 97, 0.9); /* 按钮背景颜色 */
            color: #FFFFFF; /* 按钮文字颜色 */
            font-size: 24px; /* 字体大小 */
            border: 2px solid rgba(64, 83, 97, 1); /* 按钮边框颜色 */
            border-radius: 5px;
            padding: 10px 20px; /* 增大按钮内边距 */
            margin: 8px; /* 增加外边距 */
        }

        QPushButton:hover {
            background-color: rgba(90, 110, 120, 1); /* 悬浮时背景颜色 */
            color: #E6E6E6; /* 悬浮时文字颜色 */
            border: 2px solid rgba(64, 83, 97, 1); /* 悬浮时边框颜色 */
        }

        QPushButton:pressed {
            background-color: rgba(128, 160, 190, 1); /* 按下时背景颜色 */
            color: #000000; /* 按下时文字颜色 */
            border: 2px solid rgba(90, 110, 120, 1); /* 按下时边框颜色 */
        }

        QPushButton:disabled {
            background-color: rgba(200, 200, 200, 1); /* 禁用时背景颜色 */
            color: #A0A0A0; /* 禁用时文字颜色 */
            border: 2px solid rgba(180, 180, 180, 1); /* 禁用时边框颜色 */
        }

        QScrollArea {
            background-color: rgba(255, 255, 255, 1); /* 滚动区域背景颜色 */
            border: 2px solid rgba(64, 83, 97, 1); /* 边框颜色 */
            border-radius: 5px;
            padding: 10px;
        }

     QLineEdit {
             font-family: "Arial";
             font-size: 20px;
             color: #333333; /* 字体颜色 */
             background-color: rgba(64, 83, 97, 0.2); /* 背景浅蓝色 */
             border: 2px solid rgba(64, 83, 97, 1); /* 边框颜色 */
             border-radius: 5px; /* 边框圆角 */
             padding: 8px; /* 内边距 */
             margin: 8px; /* 外边距 */
             height: 40px; /* 设置高度 */
         }

         QTextEdit {
             font-family: "Arial";
             font-size: 20px;
             color: #333333; /* 字体颜色 */
             background-color: rgba(64, 83, 97, 0.2); /* 背景浅蓝色 */
             border: 2px solid rgba(64, 83, 97, 1); /* 边框颜色 */
             border-radius: 5px; /* 边框圆角 */
             padding: 8px; /* 内边距 */
             margin: 8px; /* 外边距 */
         }

        QCheckBox {
            font-size: 18px; /* 字体大小 */
            padding: 10px;
            margin: 8px;
        }

        QCheckBox::indicator {
            width: 24px;
            height: 24px;
        }

        QCheckBox::indicator:checked {
            background-color: rgba(64, 83, 97, 0.9); /* 选中时背景颜色 */
            border-radius: 5px;
        }

        QCheckBox::indicator:unchecked {
            background-color: rgba(200, 200, 200, 1); /* 未选中时背景颜色 */
            border-radius: 5px;
        }

        QVBoxLayout {
            margin: 0;
            padding: 0;
        }

        QHBoxLayout {
            margin: 0;
            padding: 0;
        }
    )";
    this->setStyleSheet(styleSheet);
}

GroupChat::~GroupChat()
{
    delete ui;
}

GroupChat &GroupChat::getInstance()
{
    static GroupChat instance;
    return instance;
}

void GroupChat::setGroupName(QString strName)
{
    m_strGroupName = strName;
    m_strLoginName = TcpClient::getInstance().loginName();
//    ui->showMsg_te->clear();  //clear the msg from previous group
}

QString GroupChat::getGroupName()
{
    return m_strGroupName;
}

void GroupChat::updateMsg(const PDU *pdu)
{
    if (NULL == pdu)
    {
        return;
    }
    char caSendName[32] = {'\0'};
    memcpy(caSendName,pdu->caData,32);
    if(qstrcmp(caSendName, m_strLoginName.toStdString().c_str()) == 0){
        QString strMsg = QString("I: %2").arg((char*)(pdu->caMsg));
        ui->showMsg_te->append(strMsg);
    }else {
        QString strMsg = QString("%1: %2").arg(caSendName).arg((char*)(pdu->caMsg));
        ui->showMsg_te->append(strMsg);
    }
}

void GroupChat::updateOnlineMemberList(PDU *pdu)
{
    if(NULL == pdu)
    {
        return;
    }
    uint uiSize = pdu->uiMsgLen / 32;
    char caName[32] = {'\0'};
    for(uint i = 0;i<uiSize;i++)
    {
        memcpy(caName,(char*)(pdu->caMsg) + i * 32,32);
        ui->onlineMemList_lw->addItem(caName);
    }
    qDebug() << "Update Group Member";
}

void GroupChat::flushOnlineMemberList()
{
    ui->onlineMemList_lw->clear();
    QString strName = this->getGroupName();
    PDU *pdu = mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_UPDATE_MEMBER_LIST_REQUEST;
    memcpy(pdu->caData,strName.toStdString().c_str(),strName.size());
    TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
    free(pdu);
    pdu = NULL;
}


void GroupChat::auto_flush_func()
{
    this->auto_flush_bool=true;
    qDebug()<< "auto_flush_bool "<< this->auto_flush_bool;
    this->flushOnlineMemberList();
    while (this->auto_flush_bool) {
       QEventLoop loop; // 创建一个临时事件循环
       QTimer::singleShot(100, &loop, &QEventLoop::quit); // 0.1 秒后退出循环
       loop.exec(); // 执行事件循环（阻塞当前线程）
    }
}

void GroupChat::clearMsg()
{
    ui->showMsg_te->clear();
}

void GroupChat::clearMemberList()
{
    ui->onlineMemList_lw->clear();
}

void GroupChat::on_sendMsg_pb_clicked()
{
    auto_flush_func();
    QString strMsg = ui->inputMsg_le->text();
    ui->inputMsg_le->clear();
    if (!strMsg.isEmpty())
    {
        PDU *pdu = mkPDU(strMsg.size() + 1);
        pdu->uiMsgType = ENUM_MSG_TYPE_GROUP_CHAT_REQUEST;
        memcpy(pdu->caData,m_strLoginName.toStdString().c_str(),m_strLoginName.size());
        memcpy(pdu->caData + 32,m_strGroupName.toStdString().c_str(),m_strGroupName.size());
        strcpy((char*)(pdu->caMsg),strMsg.toStdString().c_str());
        TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu = NULL;
    }
    else
    {
        QMessageBox::warning(this, "group chat","Message sent cannot be empty");
    }
}

void GroupChat::on_update_pb_clicked()
{
    this->flushOnlineMemberList();
}

void GroupChat::on_at_pb_doubleClicked()
{
    if( NULL != ui->onlineMemList_lw->currentItem() )
    {
        QString targetName = ui->onlineMemList_lw->currentItem()->text();
        auto_flush_func();
        bool itemExit = false;
        for(int i = 0 ; i < ui->onlineMemList_lw->count(); i++){
            if(qstrcmp(targetName.toUtf8().data(),ui->onlineMemList_lw->item(i)->text().toUtf8().data()) == 0){
                itemExit = true;
                break;
            }
        }
        if(!itemExit){
            QMessageBox::warning(this,"at","Select friend again");
            return;
        }
        QString atStr = "@";
        atStr.append(targetName);
        atStr.append(" ");
        ui->inputMsg_le->insert(atStr);
    }
    else
    {
        QMessageBox::warning(this,"at","Select friend");
    }
}

//更新非群成员的在线好友列表
void GroupChat::updateFriendList(PDU *pdu)
{
    if(NULL == pdu)
    {
        return;
    }
    uint uiSize = pdu->uiMsgLen / 32;
    char caName[32] = {'\0'};
    for(uint i = 0;i<uiSize;i++)
    {
        memcpy(caName,(char*)(pdu->caMsg) + i * 32,32);
        m_pNonMemberFriendListWidget->addItem(caName);
    }
    qDebug() << "更新";
}

void GroupChat::on_addMember_pb_clicked()
{
    //更新非成员好友列表
    auto_flush_addMember = true;
    qDebug()<< "auto_flush_addMember "<< this->auto_flush_addMember;
    this->flushNonMemberFriend();
    while (this->auto_flush_addMember) {
       QEventLoop loop; // 创建一个临时事件循环
       QTimer::singleShot(100, &loop, &QEventLoop::quit); // 0.1 秒后退出循环
       loop.exec(); // 执行事件循环（阻塞当前线程）
    }

    QListWidget *pFriendList = this->m_pNonMemberFriendListWidget;
    AddMember::getInstance().updateNonMemberFriend(pFriendList);
    if(AddMember::getInstance().isHidden())
    {
        AddMember::getInstance().show();
    }
}

void GroupChat::flushNonMemberFriend()
{
    this->m_pNonMemberFriendListWidget->clear();
    QString strName = TcpClient::getInstance().loginName();
    QString groupName = this->getGroupName();
    PDU *pdu = mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_NONMEMBER_FRIEND_REQUEST;
    memcpy(pdu->caData,strName.toStdString().c_str(),strName.size());
    memcpy(pdu->caData + 32,groupName.toStdString().c_str(),groupName.size());
    TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
    free(pdu);
    pdu = NULL;
}

void GroupChat::updateChangeNameMsg(const char *pername, const char *newname)
{
    if (NULL == pername || NULL == newname)
    {
        return;
    }
    QString strMsg;
    if(TcpClient::getInstance().loginName().compare(pername) == 0){
       strMsg = QString("I changed the group name to \"%2\".").arg(newname);
    }else{
       strMsg = QString("\"%1\" changed the group name to \"%2\".").arg(pername).arg(newname);
    }
    ui->showMsg_te->append(strMsg);
    return;
}

void GroupChat::on_changeName_pb_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Change Group Name"), tr("You want to change the group name into:"), QLineEdit::Normal, "", &ok);
    if (ok && !text.isEmpty()) {
        qDebug() << "You entered:" << text;
        //发送改群名请求
        QString strName = TcpClient::getInstance().loginName();
        QString groupName = this->getGroupName();
        PDU *pdu = mkPDU(0);
        pdu->uiMsgType = ENUM_MSG_TYPE_CHANGE_GROUP_NAME_REQUEST;
        memcpy(pdu->caData,strName.toStdString().c_str(),strName.size());
        memcpy(pdu->caData + 32,groupName.toStdString().c_str(),groupName.size());
        memcpy(pdu->caData + 64,text.toStdString().c_str(),text.size());
        TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu = NULL;
    } else {
        qDebug() << "No text entered or dialog canceled";
    }
}

