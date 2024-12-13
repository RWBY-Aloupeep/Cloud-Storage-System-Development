/********************************************************************************
** Form generated from reading UI file 'tcpclient.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TCPCLIENT_H
#define UI_TCPCLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TcpClient
{
public:
    QHBoxLayout *horizontalLayout_5;
    QFrame *frame;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer_2;
    QLabel *label_2;
    QSpacerItem *verticalSpacer_4;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *name_lab;
    QLineEdit *name_le;
    QHBoxLayout *horizontalLayout_2;
    QLabel *pwd_lab;
    QLineEdit *pwd_le;
    QSpacerItem *verticalSpacer;
    QPushButton *login_pb;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *regist_pb;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancel_pb;
    QSpacerItem *verticalSpacer_3;

    void setupUi(QWidget *TcpClient)
    {
        if (TcpClient->objectName().isEmpty())
            TcpClient->setObjectName(QString::fromUtf8("TcpClient"));
        TcpClient->resize(1068, 580);
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        TcpClient->setFont(font);
        TcpClient->setStyleSheet(QString::fromUtf8("* {\n"
"    background: rgb(255, 255, 255);\n"
"    font-size: 20px;\n"
"    font-family: \"Arial\"; /* \345\255\227\344\275\223\350\256\276\347\275\256 */\n"
"}\n"
"\n"
"QLineEdit {\n"
"    color: rgb(245,245,245);\n"
"    background-color: rgba(64, 83, 97, 0.8); /* \345\215\212\351\200\217\346\230\216\350\203\214\346\231\257 */\n"
"    font-size: 20px;\n"
"    border-style: outset;\n"
"    border-radius: 10px;\n"
"	height: 30px;\n"
"    font-family: \"Arial\";\n"
"    padding: 5px; /* \345\206\205\350\276\271\350\267\235\344\275\277\345\206\205\345\256\271\344\270\215\350\264\264\350\276\271 */\n"
"    border: 2px solid #8d98a1;\n"
"}\n"
"\n"
"/* \351\274\240\346\240\207\351\200\211\344\270\255 QLineEdit \345\274\200\345\247\213\350\276\223\345\205\245\346\227\266\346\224\271\345\217\230\350\203\214\346\231\257\350\211\262 */\n"
"QLineEdit:focus {\n"
"    background-color: rgba(100, 150, 200, 0.9); /* \350\276\223\345\205\245\346\227\266\350\203\214\346\231\257\345\217\230\350\223\235 */\n"
"    border: 2px "
                        "solid #0056b3; /* \350\276\271\346\241\206\351\242\234\350\211\262\345\217\230\350\223\235 */\n"
"    color: white; /* \350\276\223\345\205\245\346\226\207\345\255\227\351\242\234\350\211\262\345\217\230\347\231\275 */\n"
"}\n"
"\n"
"QPushButton {\n"
"    background: rgba(206, 209, 216, 0.8); /* \345\215\212\351\200\217\346\230\216\350\203\214\346\231\257 */\n"
"    border-style: outset;\n"
"    border-radius: 20px;\n"
"    font-family: \"Arial\";\n"
"    font-size: 20px; /* \350\260\203\346\225\264\345\255\227\344\275\223\345\244\247\345\260\217\344\270\27225px */\n"
"    border: 2px solid rgba(64, 64, 64, 0.5); /* \345\215\212\351\200\217\346\230\216\350\276\271\346\241\206 */\n"
"    padding: 10px; /* \345\206\205\350\276\271\350\267\235 */\n"
"}\n"
"\n"
"/* \351\274\240\346\240\207\346\202\254\345\201\234\346\225\210\346\236\234 */\n"
"QPushButton:hover {\n"
"    background: rgba(224, 224, 224, 1); /* \351\274\240\346\240\207\346\202\254\345\201\234\345\217\230\344\272\256 */\n"
"    border-color: rgba(0, "
                        "120, 215, 0.8); /* \350\276\271\346\241\206\351\242\234\350\211\262\345\217\230\350\223\235 */\n"
"    color: rgba(0, 0, 0, 0.8); /* \346\226\207\345\255\227\351\242\234\350\211\262\345\212\240\346\267\261 */\n"
"}\n"
"\n"
"/* \346\214\211\344\270\213\346\225\210\346\236\234 */\n"
"QPushButton:pressed {\n"
"    background-color: rgba(100, 150, 200, 0.9);\n"
"    border-style: inset;\n"
"    color: white; /* \346\214\211\344\270\213\346\227\266\346\226\207\345\255\227\345\217\230\347\231\275 */\n"
"}\n"
"\n"
"QLabel {\n"
"    background: rgba(85, 170, 255, 0.0); /* \345\256\214\345\205\250\351\200\217\346\230\216\350\203\214\346\231\257 */\n"
"    color: black;\n"
"    font-family: \"Arial\";\n"
"    font-size: 20px;\n"
"    font-weight: bold; /* \345\212\240\347\262\227 */\n"
"    text-align: center;\n"
"}\n"
"\n"
"/* \351\274\240\346\240\207\346\202\254\345\201\234\346\227\266\346\226\207\345\255\227\351\242\234\350\211\262\345\217\230\345\214\226 */\n"
"QLabel:hover {\n"
"    color: #0056b3; /* \351\274\240\346"
                        "\240\207\346\202\254\345\201\234\346\227\266\346\226\207\345\255\227\345\217\230\350\223\235 */\n"
"}\n"
"\n"
"#label_2 {\n"
"    color: #FFA500; /* \344\277\256\346\224\271\346\226\207\345\255\227\351\242\234\350\211\262 */\n"
"    font-size: 26px; /* \345\242\236\345\244\247\345\255\227\344\275\223\345\244\247\345\260\217 */\n"
"    font-family: \"Arial\"; /* \350\256\276\347\275\256\345\255\227\344\275\223 */\n"
"    font-weight: bold; /* \345\212\240\347\262\227\345\255\227\344\275\223 */\n"
"	text-align: center;\n"
"}"));
        horizontalLayout_5 = new QHBoxLayout(TcpClient);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        frame = new QFrame(TcpClient);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(frame);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));
        label->setPixmap(QPixmap(QString::fromUtf8("../../Desktop/1733561653626.png")));

        horizontalLayout_4->addWidget(label);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);

        label_2 = new QLabel(frame);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout_2->addWidget(label_2);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_4);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        name_lab = new QLabel(frame);
        name_lab->setObjectName(QString::fromUtf8("name_lab"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setBold(true);
        font1.setWeight(75);
        name_lab->setFont(font1);

        horizontalLayout->addWidget(name_lab);

        name_le = new QLineEdit(frame);
        name_le->setObjectName(QString::fromUtf8("name_le"));
        name_le->setFont(font);

        horizontalLayout->addWidget(name_le);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        pwd_lab = new QLabel(frame);
        pwd_lab->setObjectName(QString::fromUtf8("pwd_lab"));
        pwd_lab->setFont(font1);

        horizontalLayout_2->addWidget(pwd_lab);

        pwd_le = new QLineEdit(frame);
        pwd_le->setObjectName(QString::fromUtf8("pwd_le"));
        pwd_le->setFont(font);
        pwd_le->setEchoMode(QLineEdit::Password);

        horizontalLayout_2->addWidget(pwd_le);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        login_pb = new QPushButton(frame);
        login_pb->setObjectName(QString::fromUtf8("login_pb"));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Arial"));
        font2.setBold(false);
        font2.setWeight(50);
        login_pb->setFont(font2);

        verticalLayout->addWidget(login_pb);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        regist_pb = new QPushButton(frame);
        regist_pb->setObjectName(QString::fromUtf8("regist_pb"));

        horizontalLayout_3->addWidget(regist_pb);

        horizontalSpacer = new QSpacerItem(28, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        cancel_pb = new QPushButton(frame);
        cancel_pb->setObjectName(QString::fromUtf8("cancel_pb"));
        cancel_pb->setFont(font2);

        horizontalLayout_3->addWidget(cancel_pb);


        verticalLayout->addLayout(horizontalLayout_3);


        verticalLayout_2->addLayout(verticalLayout);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_3);


        horizontalLayout_4->addLayout(verticalLayout_2);


        verticalLayout_3->addLayout(horizontalLayout_4);


        horizontalLayout_5->addWidget(frame);


        retranslateUi(TcpClient);

        QMetaObject::connectSlotsByName(TcpClient);
    } // setupUi

    void retranslateUi(QWidget *TcpClient)
    {
        TcpClient->setWindowTitle(QApplication::translate("TcpClient", "TcpClient", nullptr));
        label->setText(QString());
        label_2->setText(QApplication::translate("TcpClient", "Welcome to Cloud System!", nullptr));
        name_lab->setText(QApplication::translate("TcpClient", "User Name", nullptr));
        pwd_lab->setText(QApplication::translate("TcpClient", "Password", nullptr));
        login_pb->setText(QApplication::translate("TcpClient", "Log in", nullptr));
        regist_pb->setText(QApplication::translate("TcpClient", "Register", nullptr));
        cancel_pb->setText(QApplication::translate("TcpClient", "Deregister", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TcpClient: public Ui_TcpClient {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TCPCLIENT_H
