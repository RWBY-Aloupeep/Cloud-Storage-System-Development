#include "addmember.h"
#include "tcpclient.h"
#include "opewidget.h"
#include "groupchat.h"

AddMember::AddMember(QWidget *parent) : QWidget(parent)
{
    m_pSelectAllPB = new QPushButton("Select All");
    m_pCancelSelectPB = new QPushButton("Cancel Select");
    m_pOKPB = new QPushButton("Choose");
    m_pCancelPB = new QPushButton("Cancel");
    m_pSA = new QScrollArea;
    m_pFriendW = new QWidget;
    m_pFriendWVBL = new QVBoxLayout(m_pFriendW);
    m_pButtonGroup = new QButtonGroup(m_pFriendW);
    m_pButtonGroup->setExclusive(false);
    QHBoxLayout *pTopHBL = new QHBoxLayout;
    pTopHBL->addWidget(m_pSelectAllPB);
    pTopHBL->addWidget(m_pCancelSelectPB);
    pTopHBL->addStretch();
    QHBoxLayout *pDownHBL = new QHBoxLayout;
    pDownHBL->addWidget(m_pOKPB);
    pDownHBL->addWidget(m_pCancelPB);
    QVBoxLayout *pMainVBL = new QVBoxLayout;
    pMainVBL->addLayout(pTopHBL);
    pMainVBL->addWidget(m_pSA);
    pMainVBL->addLayout(pDownHBL);
    setLayout(pMainVBL);
    connect(m_pCancelSelectPB,SIGNAL(clicked(bool)),this,SLOT(cancelSelect()));
    connect(m_pSelectAllPB,SIGNAL(clicked(bool)),this,SLOT(selectAll()));
    connect(m_pOKPB,SIGNAL(clicked(bool)),this,SLOT(okCreate()));
    connect(m_pCancelPB,SIGNAL(clicked(bool)),this,SLOT(cancelCreate()));

    QString styleSheet = R"(
        QWidget {
            background-color: rgba(245, 245, 245, 1); /* 主背景浅灰 */
            font-family: 'Arial', Times, serif; /* 字体设置为 Times New Roman */
            font-size: 18px; /* 字体增大 */
        }

        QPushButton {
            background-color: rgba(64, 83, 97, 0.9); /* 按钮背景颜色 */
            color: #FFFFFF; /* 按钮文字颜色 */
            font-size: 18px; /* 字体大小 */
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

AddMember &AddMember::getInstance()
{
    static AddMember instance;
    return instance;
}

void AddMember::updateNonMemberFriend(QListWidget *pFriendList)
{
    if(NULL == pFriendList)
    {
        return;
    }
    QAbstractButton* temp = NULL;
    QList<QAbstractButton*> preFriendList = m_pButtonGroup->buttons();
    for(int i = 0;i<preFriendList.size();i++)
    {
        temp = preFriendList[i];
        m_pFriendWVBL->removeWidget(temp);
        m_pButtonGroup->removeButton(temp);
        preFriendList.removeOne(temp);
        delete temp;
        temp = NULL;
    }
    QCheckBox *pCB = NULL;
    for(int i = 0;i<pFriendList->count();i++)
    {
        pCB = new QCheckBox(pFriendList->item(i)->text());
        m_pFriendWVBL->addWidget(pCB);
        m_pButtonGroup->addButton(pCB);
    }
    m_pSA->setWidget(m_pFriendW);
}

void AddMember::cancelSelect()
{
    QList<QAbstractButton*> cbList =  m_pButtonGroup->buttons();
    for(int i = 0;i<cbList.size();i++)
    {
        if(cbList[i]->isChecked())
        {
            cbList[i]->setChecked(false);
        }
    }
}

void AddMember::selectAll()
{
    QList<QAbstractButton*> cbList =  m_pButtonGroup->buttons();
    for(int i = 0;i<cbList.size();i++)
    {
        if(!cbList[i]->isChecked())
        {
            cbList[i]->setChecked(true);
        }
    }
}

void AddMember::okCreate()
{
//    QString caName = TcpClient::getInstance().loginName();
    QString groupName = GroupChat::getInstance().getGroupName();
    QList<QAbstractButton*> cbList =  m_pButtonGroup->buttons();
    int num = 0;
    for(int i = 0;i<cbList.size();i++)
    {
        if(cbList[i]->isChecked())
        {
            num++;
        }
    }
    PDU *pdu = mkPDU(32 * num +  + 1);
    pdu->uiMsgType = ENUM_MSG_TYPE_ADD_MEMBER_REQUEST;
//    memcpy(pdu->caData,caName.toStdString().c_str(),caName.size());
    memcpy(pdu->caData,groupName.toStdString().c_str(),groupName.size());
    sprintf(pdu->caData + 32,"%d",num);
    int j = 0;
    for(int i = 0;i<cbList.size();i++)
    {
        if(cbList[i]->isChecked())
        {
            memcpy((char*)(pdu->caMsg) + j * 32,cbList[i]->text().toStdString().c_str(),cbList[i]->text().size());
            j++;
        }
    }
    TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
    free(pdu);
    pdu = NULL;
    hide();
}

void AddMember::cancelCreate()
{
    hide();
}
