#include "friend.h"
#include "protocol.h"
#include "tcpclient.h"
#include <QInputDialog>
#include <QDebug>
#include "privatechat.h"
#include <QMessageBox>
#include "creategroup.h"
#include <QEventLoop>
#include <QTimer>
#include "groupchat.h"

Friend &Friend::getInstance()
{
    static Friend instance;
    return instance;
}

Friend::Friend(QWidget *parent) : QWidget(parent)
{
    m_pFriendListWidget = new QListWidget;
    m_pGroupListWidget = new QListWidget; // 初始化群组列表

    m_pDelFriendPB = new QPushButton("Delete");
    m_pFlushFriendPB = new QPushButton("Flush Friend");
    m_pShowOnlineUsrPB = new QPushButton("Show Online User");
    m_pSearchUsrPB = new QPushButton("Search User");
    m_pPrivateChatPB = new QPushButton("Chat");
    m_pCreateGroupPB = new QPushButton("Create Group");

    QVBoxLayout *pRightPBVBL = new QVBoxLayout;
    pRightPBVBL->addWidget(m_pDelFriendPB);
    pRightPBVBL->addWidget(m_pFlushFriendPB);
    pRightPBVBL->addWidget(m_pShowOnlineUsrPB);
    pRightPBVBL->addWidget(m_pSearchUsrPB);
    pRightPBVBL->addWidget(m_pPrivateChatPB);
    pRightPBVBL->addWidget(m_pCreateGroupPB);

    QVBoxLayout *pFriendGroupVBL = new QVBoxLayout; // 垂直布局存放好友列表和群组列表
    pFriendGroupVBL->addWidget(m_pFriendListWidget); // 添加好友列表
    pFriendGroupVBL->addWidget(m_pGroupListWidget);  // 添加群组列表

    QHBoxLayout *pTopHBL = new QHBoxLayout;
    pTopHBL->addLayout(pFriendGroupVBL); // 左侧布局
    pTopHBL->addLayout(pRightPBVBL);     // 右侧按钮布局

    QHBoxLayout *pMsgHBL = new QHBoxLayout;

    m_pOnline = new Online;

    QVBoxLayout *pMain = new QVBoxLayout;
    pMain->addLayout(pTopHBL);
    pMain->addLayout(pMsgHBL);
    pMain->addWidget(m_pOnline);
    m_pOnline->hide();
    setLayout(pMain);

    connect(m_pShowOnlineUsrPB, SIGNAL(clicked(bool)), this, SLOT(showOnline()));
    connect(m_pSearchUsrPB, SIGNAL(clicked(bool)), this, SLOT(searchUsr()));
    connect(m_pFlushFriendPB, SIGNAL(clicked(bool)), this, SLOT(flushFriend()));
    connect(m_pDelFriendPB, SIGNAL(clicked(bool)), this, SLOT(deleteFriend()));
    connect(m_pPrivateChatPB, SIGNAL(clicked(bool)), this, SLOT(privateChat()));
    connect(m_pCreateGroupPB, SIGNAL(clicked(bool)), this, SLOT(createGroup()));

    connect(m_pFriendListWidget, &QListWidget::itemClicked, this, &Friend::onItemClicked);
    connect(m_pGroupListWidget, &QListWidget::itemClicked, this, &Friend::onItemClicked);
}

void Friend::showAllOnlineUsr(PDU *pdu)
{
    if(NULL == pdu)
    {
        return;
    }
    m_pOnline->showUsr(pdu);
}

void Friend::updateFriendList(PDU *pdu)
{
    if (NULL == pdu)
        {
            return;
        }

        uint uiSize = pdu->uiMsgLen / 32;
        char caName[32] = {'\0'};
        QIcon friendIcon(QPixmap(":/icon/individual.png"));
        QIcon groupIcon(QPixmap(":/icon/group.png"));

        m_pFriendListWidget->clear(); // 清空好友列表
        m_pGroupListWidget->clear();  // 清空群组列表

        for (uint i = 0; i < uiSize; i++)
        {
            memcpy(caName, (char *)(pdu->caMsg) + i * 32, 32);

            QString name = QString(caName);
            QListWidgetItem *item;

            if (name.startsWith("group:"))
            {
                QString groupName = name.mid(6); // 获取群组名称
                item = new QListWidgetItem(groupIcon, groupName);
                m_pGroupListWidget->addItem(item); // 添加到群组列表
            }
            else
            {
                item = new QListWidgetItem(friendIcon, name);
                m_pFriendListWidget->addItem(item); // 添加到好友列表
            }
        }
//    if(NULL == pdu)
//        {
//            return;
//        }
//        uint uiSize = pdu->uiMsgLen / 32;
//        char typeName[32] = {'\0'};
//        char caName[32] = {'\0'};
//        QString groupstr = "This_is_beginning_of_group";

//        for(uint i = 0;i<uiSize;i ++)
//        {
//            memcpy(caName,(char*)(pdu->caMsg) + i * 32,32);
//            if(groupstr.compare(caName) == 0){
//                firstIndexOfGroup = i;
//            }else{
//                m_pFriendListWidget->addItem(caName);
//            }
//        }
//        qDebug() << "更新";
//    if (NULL == pdu)
//       {
//           return;
//       }

//       uint uiSize = pdu->uiMsgLen / 32;
//       char caName[32] = {'\0'};
//       QIcon friendIcon(QPixmap(":/icon/individual.png"));
//       QIcon groupIcon(QPixmap(":/icon/group.png"));

//       for (uint i = 0; i < uiSize; i++)
//       {
//           memcpy(caName, (char*)(pdu->caMsg) + i * 32, 32);

//           QString name = QString(caName);
//           QListWidgetItem *item;

//           if (name.startsWith("group_"))
//           {
//               item = new QListWidgetItem(groupIcon, name.mid(6));
//           }
//           else if (name.startsWith("friend_"))
//           {
//               item = new QListWidgetItem(friendIcon, name.mid(7));
//           }
//           else
//           {
//               continue;
//           }

//           m_pFriendListWidget->addItem(item);
//       }
//    QIcon friendIcon(QPixmap(":/icon/individual.png"));

//        if (NULL == pdu)
//        {
//            return;
//        }

//        uint uiSize = pdu->uiMsgLen / 32;
//        char caName[32] = {'\0'};

//        for (uint i = 0; i < uiSize; i++)
//        {
//            memcpy(caName, (char*)(pdu->caMsg) + i * 32, 32);
//            QListWidgetItem *item = new QListWidgetItem(friendIcon, caName);
//            m_pFriendListWidget->addItem(item);
//        }
}

//void Friend::updateGroupMsg(PDU *pdu)
//{
//    QString strMsg = QString("%1: %2").arg(pdu->caData).arg((char*)pdu->caMsg);
//    m_pShowMsgTE->append(strMsg);
//}

QListWidget *Friend::getFriendList()
{
    return m_pFriendListWidget;
}

void Friend::changeNameinList(const char *oldname, const char *newname)
{
    for (int i = 0; i < m_pFriendListWidget->count(); ++i) {
        QListWidgetItem *item = m_pFriendListWidget->item(i);
        //找到该群聊
        if(item->text().compare(oldname) == 0){
            item->setText(newname);
            return;
        }
    }
}

uint Friend::getFirstIndexOfGroup()
{
    return firstIndexOfGroup;
}

void Friend::handleAddFriend(QString friendname)
{
    QString strLoginName = TcpClient::getInstance().loginName();

    // 创建PDU包
    PDU *pdu = mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_REQUEST;
    memcpy(pdu->caData, friendname.toStdString().c_str(), friendname.size());
    memcpy(pdu->caData + 32, strLoginName.toStdString().c_str(), strLoginName.size());

    // 发送PDU
    TcpClient::getInstance().getTcpSocket().write((char*)pdu, pdu->uiPDULen);

    free(pdu);  // 释放内存
    pdu = NULL;
}

//自动刷新好友列表
void Friend::auto_flush_func()
{
    this->auto_flush_bool=true;
    qDebug()<< "auto_flush_bool "<< this->auto_flush_bool;
    this->flushFriend();
    while (this->auto_flush_bool) {
       QEventLoop loop; // 创建一个临时事件循环
       QTimer::singleShot(100, &loop, &QEventLoop::quit); // 0.1 秒后退出循环
       loop.exec(); // 执行事件循环（阻塞当前线程）
    }
}

void Friend::showOnline()
{
    if(m_pOnline->isHidden())
    {
        m_pOnline->show();
        PDU *pdu = mkPDU(0);
        pdu->uiMsgType = ENUM_MSG_TYPE_ALL_ONLINE_REQUEST;
        TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu = NULL;
    }
    else
    {
        m_pOnline->hide();
    }
}

void Friend::searchUsr()
{
    QString strLoginName = TcpClient::getInstance().loginName();
        m_strSearchName = QInputDialog::getText(this,"Search","User Name");
        if(!m_strSearchName.isEmpty())
        {
            qDebug() << "待查找的用户名为：" << m_strSearchName;
            this->auto_flush_func();
            bool isInList = false;
            for (int i = 0; i < m_pFriendListWidget->count(); ++i) {
                QListWidgetItem *item = m_pFriendListWidget->item(i);
                //找到该好友/群聊
                if(item->text().compare(m_strSearchName) == 0){
                    m_pFriendListWidget->setCurrentItem(item); //选中该好友/群聊

                    QMessageBox msgBox;
                    msgBox.setText(QString("%1: Online").arg(m_strSearchName));
                    QPushButton *chat_Button = new QPushButton("Chat");
                    QPushButton *delete_Button = new QPushButton("Delete");
                    msgBox.addButton(chat_Button, QMessageBox::ActionRole);
                    msgBox.addButton(delete_Button, QMessageBox::ActionRole);
                    connect(chat_Button, &QPushButton::clicked, [this]() {
                        this->privateChat();
                    });
                    connect(delete_Button, &QPushButton::clicked, [this]() {
                        this->deleteFriend();
                    });

                    isInList = true;
                    m_strSearchName.clear();
                    return;
                }
            }
            //查询自已
            if(m_strSearchName.compare(strLoginName) == 0){
               QMessageBox::information(this,"Search Friend",QString("It's YOU"));
               return;
            }
            if(!isInList){
                PDU *pdu = mkPDU(0);
                memcpy(pdu->caData, m_strSearchName.toStdString().c_str(), m_strSearchName.size());
                pdu->uiMsgType = ENUM_MSG_TYPE_SEARCH_USR_REQUEST;
                TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
                free(pdu);
                pdu = NULL;
            }
        }
        return;
//    m_strSearchName = QInputDialog::getText(this,"Search","User Name");
//    if(!m_strSearchName.isEmpty())
//    {
//        qDebug() << "Search user name:" << m_strSearchName;
//        PDU *pdu = mkPDU(0);
//        memcpy(pdu->caData, m_strSearchName.toStdString().c_str(), m_strSearchName.size());
//        pdu->uiMsgType = ENUM_MSG_TYPE_SEARCH_USR_REQUEST;
//        TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
//        free(pdu);
//        pdu = NULL;
//    }
}

void Friend::flushFriend()
{
    m_pFriendListWidget->clear();
    QString strName = TcpClient::getInstance().loginName();
    PDU *pdu = mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_FRIEND_REQUEST;
    memcpy(pdu->caData,strName.toStdString().c_str(),strName.size());
    TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
    free(pdu);
    pdu = NULL;
}

void Friend::deleteFriend()
{
    if(NULL != m_pFriendListWidget->currentItem())
    {
        QString strFriendName = m_pFriendListWidget->currentItem()->text();
        PDU *pdu = mkPDU(0);
        pdu->uiMsgType = ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST;
        QString strSelName = TcpClient::getInstance().loginName();
        memcpy(pdu->caData,strSelName.toStdString().c_str(),strSelName.size());
        memcpy(pdu->caData + 32,strFriendName.toStdString().c_str(),strFriendName.size());
        TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu = NULL;
    }
    else if ( NULL != m_pGroupListWidget->currentItem())
    {
        QString strFriendName = m_pGroupListWidget->currentItem()->text();
        PDU *pdu = mkPDU(0);
        pdu->uiMsgType = ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST;
        QString strSelName = TcpClient::getInstance().loginName();
        memcpy(pdu->caData,strSelName.toStdString().c_str(),strSelName.size());
        memcpy(pdu->caData + 32,strFriendName.toStdString().c_str(),strFriendName.size());
        TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu = NULL;
    }
}

void Friend::privateChat()
{
    if (lastClickedListWidget == nullptr) {
            QMessageBox::warning(this, "Chat", "Please select group or friend");
            return;
        }

        // 如果点击的是好友列表
        if (lastClickedListWidget == m_pFriendListWidget) {
            QListWidgetItem* currentItem = m_pFriendListWidget->currentItem();
            QString strChatName = currentItem->text();

            // 开启私聊
            QString chatName = TcpClient::getInstance().loginName();
            chatName.append(" & ");
            chatName.append(strChatName);
            if (PrivateChat::getInstance().getChatName() != strChatName) {
                PrivateChat::getInstance().clearMsg();
                PrivateChat::getInstance().setChatName(strChatName);
                PrivateChat::getInstance().setWindowTitle(chatName);
            }
            if (PrivateChat::getInstance().isHidden()) {
                PrivateChat::getInstance().show();
            }
        }
        // 如果点击的是群组列表
        else if (lastClickedListWidget == m_pGroupListWidget) {
            QListWidgetItem* currentItem = m_pGroupListWidget->currentItem();
            QString strChatName = currentItem->text();

            // 开启群聊
            GroupChat::getInstance().setGroupName(strChatName);
            GroupChat::getInstance().setWindowTitle(strChatName);
            GroupChat::getInstance().auto_flush_func();
            GroupChat::getInstance().clearMsg();
            if (GroupChat::getInstance().isHidden()) {
                GroupChat::getInstance().show();
            }
        }
//    if(NULL != m_pFriendListWidget->currentItem())
//    {
//        QString strChatName = m_pFriendListWidget->currentItem()->text();
//        PrivateChat::getInstance().setChatName(strChatName);
//        if(PrivateChat::getInstance().isHidden())
//        {
//            PrivateChat::getInstance().show();
//        }
//    }
//    else
//    {
//        QMessageBox::warning(this,"Private Chat","Select frient");
//    }
}

void Friend::createGroup()
{
    this->auto_flush_func();
    QListWidget *pFriendList = this->getFriendList();
    CreateGroup::getInstance().updateFriend(pFriendList);
    if(CreateGroup::getInstance().isHidden())
    {
        CreateGroup::getInstance().show();
    }
}

void Friend::onItemClicked(QListWidgetItem *item)
{
    if (m_pFriendListWidget->currentItem() == item) {
        lastClickedListWidget = m_pFriendListWidget;
    } else if (m_pGroupListWidget->currentItem() == item) {
        lastClickedListWidget = m_pGroupListWidget;
    }
}

//void Friend::groupChat()
//{
//    QString strMsg = m_pInputMsgLE->text();
//    if(!strMsg.isEmpty())
//    {
//        PDU *pdu = mkPDU(strMsg.size() + 1);
//        pdu->uiMsgType = ENUM_MSG_TYPE_GROUP_CHAT_REQUEST;
//        QString strName = TcpClient::getInstance().loginName();
//        strncpy(pdu->caData,strName.toStdString().c_str(),strName.size());
//        strncpy((char*)(pdu->caMsg),strMsg.toStdString().c_str(),strMsg.size());

//        // 显示用户自己发送的消息
//        QString displayMsg = QString("%1: %2").arg(strName, strMsg);
//        m_pShowMsgTE->append(displayMsg);

//        TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
//    }
//    else
//    {
//        QMessageBox::warning(this,"Group Chat","Message cannot be empty");
//    }
//}
