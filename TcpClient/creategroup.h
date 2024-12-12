#ifndef CREATEGROUP_H
#define CREATEGROUP_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QScrollArea>
#include <QCheckBox>
#include <QListWidget>

class CreateGroup : public QWidget
{
    Q_OBJECT
public:
    explicit CreateGroup(QWidget *parent = nullptr);
    static CreateGroup &getInstance();
    void updateFriend(QListWidget *pFriendList);

signals:

public slots:
    void cancelSelect();
    void selectAll();
    void okCreate();
    void cancelCreate();


private:
    QPushButton *m_pSelectAllPB;
    QPushButton *m_pCancelSelectPB;
    QPushButton *m_pOKPB;
    QPushButton *m_pCancelPB;
    QScrollArea *m_pSA;
    QWidget *m_pFriendW;
    QVBoxLayout *m_pFriendWVBL;
    QButtonGroup *m_pButtonGroup;
};

#endif // CREATEGROUP_H
