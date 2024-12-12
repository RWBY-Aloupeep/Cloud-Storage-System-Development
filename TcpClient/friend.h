#ifndef FRIEND_H
#define FRIEND_H

#include <QWidget>
#include <QTextEdit>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "online.h"


class Friend : public QWidget
{
    Q_OBJECT
public:
    static Friend &getInstance();
    explicit Friend(QWidget *parent = nullptr);
    void showAllOnlineUsr(PDU *pdu);
    QString m_strSearchName;
    void updateFriendList(PDU *pdu);
//    void updateGroupMsg(PDU *pdu);
    QListWidget *getFriendList();
    void changeNameinList(const char* oldname,const char* newname);
    uint getFirstIndexOfGroup();
    void handleAddFriend(QString friendname);

    //群聊创建
    void updateGroupMsg(PDU *pdu);
    bool auto_flush_bool;
    void auto_flush_func();    

signals:

public slots:
    void showOnline();
    void searchUsr();
    void flushFriend();
    void deleteFriend();
    void privateChat();
//    void groupChat();
    void createGroup();

private:
//    QTextEdit *m_pShowMsgTE;
    QListWidget *m_pFriendListWidget;
    QLineEdit *m_pInputMsgLE;
    QPushButton *m_pDelFriendPB;
    QPushButton *m_pFlushFriendPB;
    QPushButton *m_pShowOnlineUsrPB;
    QPushButton *m_pSearchUsrPB;
//    QPushButton *m_pMsgSendPB;
    QPushButton *m_pPrivateChatPB;
    Online *m_pOnline;

    QPushButton *m_pCreateGroupPB;
    QListWidget *m_pGroupListWidget;
    uint firstIndexOfGroup;
    QListWidget* lastClickedListWidget;

private slots:
    void onItemClicked(QListWidgetItem* item);

signals:

};

#endif // FRIEND_H
