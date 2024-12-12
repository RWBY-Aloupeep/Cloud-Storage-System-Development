#ifndef GROUPCHAT_H
#define GROUPCHAT_H

#include <QWidget>
#include "protocol.h"
#include <QListWidget>
#include <QInputDialog>

namespace Ui {
class GroupChat;
}

class GroupChat : public QWidget
{
    Q_OBJECT

public:
    explicit GroupChat(QWidget *parent = nullptr);
    ~GroupChat();

    static GroupChat &getInstance();

    void setGroupName(QString strName);
    QString getGroupName();
    void updateMsg(const PDU *pdu);
    void updateOnlineMemberList(PDU *pdu);
    void flushOnlineMemberList();
    bool auto_flush_bool;
    void auto_flush_func();
    void clearMsg();
    void clearMemberList();
    bool is_new_window;

    void updateFriendList(PDU *pdu);
    bool auto_flush_addMember;

    void updateChangeNameMsg(const char* pername,const char* newname);

    void on_changeName_pb_clicked();

private slots:
    void on_sendMsg_pb_clicked();

    void on_update_pb_clicked();

    void on_at_pb_doubleClicked();

    void on_addMember_pb_clicked();
    void flushNonMemberFriend();

private:
    Ui::GroupChat *ui;
    QListWidget *m_pNonMemberFriendListWidget;
    QString m_strGroupName;
    QString m_strLoginName;
    QInputDialog *m_changeGroupNameInput;

};

#endif // GROUPCHAT_H
