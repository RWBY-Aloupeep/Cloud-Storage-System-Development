#include "privatechat.h"
#include "ui_privatechat.h"
#include "protocol.h"
#include "tcpclient.h"
#include <QMessageBox>

PrivateChat::PrivateChat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrivateChat)
{
    ui->setupUi(this);

    setFixedSize(600, 600);
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

PrivateChat::~PrivateChat()
{
    delete ui;
}

void PrivateChat::setChatName(QString strName)
{
    m_strChatName = strName;
    m_strLoginName = TcpClient::getInstance().loginName();
}

PrivateChat &PrivateChat::getInstance()
{
    static PrivateChat instance;
    return instance;
}

void PrivateChat::updateMsg(const PDU *pdu)
{
    //    qDebug() << "updateMsg这里";
        if (NULL == pdu)
        {
            return;
        }
        char caSendName[32] = {'\0'};
        memcpy(caSendName,pdu->caData,32);
    //    if(qstrcmp(caSendName, m_strLoginName.toStdString().c_str()) == 0){
    //        //发送者为自己
    //        QString strMsg = QString("   I say: %1").arg((char*)(pdu->caSmg));
    //        ui->showMsg_te->append(strMsg);
    //    }else {
            QString strMsg = QString("%1: %2").arg(caSendName).arg((char*)(pdu->caMsg));
            ui->showMsg_te->append(strMsg);
            //    }
//    if(NULL == pdu)
//    {
//        return;
//    }
//    char caSendName[32] = {'\0'};
//    memcpy(caSendName,pdu->caData,32);
//    QString strMsg = QString("%1: %2").arg(caSendName).arg((char*)(pdu->caMsg));
//    ui->showMsg_te->append(strMsg);
}

QString PrivateChat::getChatName()
{
    return m_strChatName;
}

void PrivateChat::clearMsg()
{
    ui->showMsg_te->clear();
}

void PrivateChat::on_sendMsg_pb_clicked()
{
    QString strMsg = ui->inputMsg_le->text();
    ui->inputMsg_le->clear();
    if (!strMsg.isEmpty())
    {
        QString strMyMsg = QString("You: %1").arg(strMsg);



        ui->showMsg_te->append(strMyMsg);
        PDU *pdu = mkPDU(strMsg.size() + 1);
        pdu->uiMsgType = ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST;
        memcpy(pdu->caData,m_strLoginName.toStdString().c_str(),m_strLoginName.size());
        memcpy(pdu->caData + 32,m_strChatName.toStdString().c_str(),m_strChatName.size());
        strcpy((char*)(pdu->caMsg),strMsg.toStdString().c_str());
        TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu = NULL;
    }
    else
    {
        QMessageBox::warning(this, "private chat","Message sent cannot be empty");
    }
//    QString strMsg = ui->inputMsg_le->text();
//    ui->inputMsg_le->clear();
//    if(!strMsg.isEmpty())
//    {
//        PDU *pdu = mkPDU(strMsg.size() + 1);
//        pdu->uiMsgType = ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST;
//        memcpy(pdu->caData,m_strLoginName.toStdString().c_str(),m_strLoginName.size());
//        memcpy(pdu->caData + 32,m_strChatName.toStdString().c_str(),m_strChatName.size());
//        strcpy((char*)(pdu->caMsg),strMsg.toStdString().c_str());
//        TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
//        free(pdu);
//        pdu = NULL;
//    }
//    else
//    {
//        QMessageBox::warning(this,"Private Chat","Message cannot be empty");
//    }
}
