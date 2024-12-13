/********************************************************************************
** Form generated from reading UI file 'groupchat.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GROUPCHAT_H
#define UI_GROUPCHAT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GroupChat
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QTextEdit *showMsg_te;
    QListWidget *onlineMemList_lw;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *inputMsg_le;
    QPushButton *sendMsg_pb;
    QHBoxLayout *horizontalLayout;
    QPushButton *addMember_pb;
    QSpacerItem *horizontalSpacer;
    QPushButton *update_pb;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *changeName_pb;

    void setupUi(QWidget *GroupChat)
    {
        if (GroupChat->objectName().isEmpty())
            GroupChat->setObjectName(QString::fromUtf8("GroupChat"));
        GroupChat->resize(414, 279);
        GroupChat->setStyleSheet(QString::fromUtf8(""));
        verticalLayout_2 = new QVBoxLayout(GroupChat);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        showMsg_te = new QTextEdit(GroupChat);
        showMsg_te->setObjectName(QString::fromUtf8("showMsg_te"));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(10);
        showMsg_te->setFont(font);
        showMsg_te->setReadOnly(true);

        horizontalLayout_3->addWidget(showMsg_te);

        onlineMemList_lw = new QListWidget(GroupChat);
        onlineMemList_lw->setObjectName(QString::fromUtf8("onlineMemList_lw"));
        onlineMemList_lw->setMaximumSize(QSize(128, 16777215));

        horizontalLayout_3->addWidget(onlineMemList_lw);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        inputMsg_le = new QLineEdit(GroupChat);
        inputMsg_le->setObjectName(QString::fromUtf8("inputMsg_le"));

        horizontalLayout_2->addWidget(inputMsg_le);

        sendMsg_pb = new QPushButton(GroupChat);
        sendMsg_pb->setObjectName(QString::fromUtf8("sendMsg_pb"));
        sendMsg_pb->setFont(font);

        horizontalLayout_2->addWidget(sendMsg_pb);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        addMember_pb = new QPushButton(GroupChat);
        addMember_pb->setObjectName(QString::fromUtf8("addMember_pb"));
        addMember_pb->setFont(font);

        horizontalLayout->addWidget(addMember_pb);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        update_pb = new QPushButton(GroupChat);
        update_pb->setObjectName(QString::fromUtf8("update_pb"));
        update_pb->setFont(font);

        horizontalLayout->addWidget(update_pb);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        changeName_pb = new QPushButton(GroupChat);
        changeName_pb->setObjectName(QString::fromUtf8("changeName_pb"));
        changeName_pb->setFont(font);

        horizontalLayout->addWidget(changeName_pb);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(GroupChat);

        QMetaObject::connectSlotsByName(GroupChat);
    } // setupUi

    void retranslateUi(QWidget *GroupChat)
    {
        GroupChat->setWindowTitle(QApplication::translate("GroupChat", "Form", nullptr));
        sendMsg_pb->setText(QApplication::translate("GroupChat", "Send", nullptr));
        addMember_pb->setText(QApplication::translate("GroupChat", "Add", nullptr));
        update_pb->setText(QApplication::translate("GroupChat", "Update", nullptr));
        changeName_pb->setText(QApplication::translate("GroupChat", "Rename", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GroupChat: public Ui_GroupChat {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GROUPCHAT_H
