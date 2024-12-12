#include "sharefile.h"
#include "tcpclient.h"
#include "opewidget.h"
#include "friend.h"

ShareFile::ShareFile(QWidget *parent) : QWidget(parent)
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
    setFixedSize(600, 600);
    connect(m_pCancelSelectPB,SIGNAL(clicked(bool)),this,SLOT(cancelSelect()));
    connect(m_pSelectAllPB,SIGNAL(clicked(bool)),this,SLOT(selectAll()));
    connect(m_pOKPB,SIGNAL(clicked(bool)),this,SLOT(okShare()));
    connect(m_pCancelPB,SIGNAL(clicked(bool)),this,SLOT(cancelShare()));

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

         QCheckBox::indicator {
           width: 20px; /* 勾选框宽度 */
           height: 20px; /* 勾选框高度 */
           border: 2px solid rgba(64, 83, 97, 1); /* 勾选框边框颜色 */
           border-radius: 3px; /* 勾选框圆角 */
           background-color: rgba(255, 255, 255, 1); /* 勾选框背景颜色 */
       }


       QCheckBox::indicator:checked {
           border-color: rgba(64, 83, 97, 1); /* 勾选状态边框颜色 */
           background-color: rgba(64, 83, 97, 0.2); /* 勾选状态背景颜色 */
           image: url('://icon//checkmark.jpg');
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

// 返回对象的单例
ShareFile &ShareFile::getInstance()
{
    static ShareFile instance;
    return instance;
}

// 更新当前用户的好友列表
void ShareFile::updateFriend(QListWidget *pFriendList)
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

// 取消选择
void ShareFile::cancelSelect()
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

// 全选
void ShareFile::selectAll()
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

// 取消
void ShareFile::cancelShare()
{
    hide();
}

// 确定
void ShareFile::okShare()
{
    QString strName = TcpClient::getInstance().loginName();
    QString strCurPath = TcpClient::getInstance().curPath();
    QString strShareFileName = OpeWidget::getInstance().getBook()->getShareFileName();
    QString strPath = strCurPath + "/" + strShareFileName;
    QList<QAbstractButton*> cbList =  m_pButtonGroup->buttons();
    int num = 0;
    for(int i = 0;i<cbList.size();i++)
    {
        if(cbList[i]->isChecked())
        {
            num++;
        }
    }
    PDU *pdu = mkPDU(32 * num + strPath.size() + 1);
    pdu->uiMsgType = ENUM_MSG_TYPE_SHARE_FILE_REQUEST;
    sprintf(pdu->caData,"%s %d",strName.toStdString().c_str(),num);
    int j = 0;
    for(int i = 0;i<cbList.size();i++)
    {
        if(cbList[i]->isChecked())
        {
            memcpy((char*)(pdu->caMsg) + j * 32,cbList[i]->text().toStdString().c_str(),cbList[i]->text().size());
            j++;
        }
    }
    memcpy((char*)(pdu->caMsg) + num * 32,strPath.toStdString().c_str(),strPath.size());
    TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
    free(pdu);
    pdu = NULL;

    hide();
}
