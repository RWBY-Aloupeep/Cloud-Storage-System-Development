/********************************************************************************
** Form generated from reading UI file 'privatechat.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRIVATECHAT_H
#define UI_PRIVATECHAT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PrivateChat
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QTextEdit *showMsg_te;
    QHBoxLayout *horizontalLayout;
    QLineEdit *inputMsg_le;
    QPushButton *sendMsg_pb;

    void setupUi(QWidget *PrivateChat)
    {
        if (PrivateChat->objectName().isEmpty())
            PrivateChat->setObjectName(QString::fromUtf8("PrivateChat"));
        PrivateChat->resize(335, 259);
        verticalLayout_2 = new QVBoxLayout(PrivateChat);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        showMsg_te = new QTextEdit(PrivateChat);
        showMsg_te->setObjectName(QString::fromUtf8("showMsg_te"));
        showMsg_te->setReadOnly(true);

        verticalLayout->addWidget(showMsg_te);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        inputMsg_le = new QLineEdit(PrivateChat);
        inputMsg_le->setObjectName(QString::fromUtf8("inputMsg_le"));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(18);
        inputMsg_le->setFont(font);

        horizontalLayout->addWidget(inputMsg_le);

        sendMsg_pb = new QPushButton(PrivateChat);
        sendMsg_pb->setObjectName(QString::fromUtf8("sendMsg_pb"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(20);
        sendMsg_pb->setFont(font1);

        horizontalLayout->addWidget(sendMsg_pb);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(PrivateChat);

        QMetaObject::connectSlotsByName(PrivateChat);
    } // setupUi

    void retranslateUi(QWidget *PrivateChat)
    {
        PrivateChat->setWindowTitle(QApplication::translate("PrivateChat", "Form", nullptr));
        sendMsg_pb->setText(QApplication::translate("PrivateChat", "Send", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PrivateChat: public Ui_PrivateChat {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRIVATECHAT_H
