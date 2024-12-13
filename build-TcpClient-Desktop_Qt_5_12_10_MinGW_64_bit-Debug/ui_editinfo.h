/********************************************************************************
** Form generated from reading UI file 'editinfo.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDITINFO_H
#define UI_EDITINFO_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EditInfo
{
public:
    QHBoxLayout *horizontalLayout_6;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer;
    QGraphicsView *pic_gv;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLabel *name_lb;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_5;
    QLabel *vip_lb;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *opwd_le;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QLineEdit *npwd_le;
    QPushButton *changepwd_pb;
    QPushButton *changepic_pb;
    QPushButton *VipApply_pb;

    void setupUi(QWidget *EditInfo)
    {
        if (EditInfo->objectName().isEmpty())
            EditInfo->setObjectName(QString::fromUtf8("EditInfo"));
        EditInfo->resize(400, 445);
        EditInfo->setStyleSheet(QString::fromUtf8("QWidget {\n"
"            background-color: rgba(245, 245, 245, 1); /* \344\270\273\350\203\214\346\231\257\346\265\205\347\201\260 */\n"
"            font-family: \"Times New Roman\";\n"
"            font-size: 30px;\n"
"        }\n"
"QGraphicsView {\n"
"    border: 1px solid #cccccc; /* \350\256\276\347\275\256\350\276\271\346\241\206\351\242\234\350\211\262 */\n"
"    border-radius: 10px; /* \350\256\276\347\275\256\345\234\206\350\247\222 */\n"
"}\n"
"\n"
"        QListWidget {\n"
"            background-color: rgba(64, 83, 97, 0.9); /* \345\210\227\350\241\250\350\203\214\346\231\257\351\242\234\350\211\262 */\n"
"            color: #FFFFFF; /* \345\210\227\350\241\250\346\226\207\345\255\227\351\242\234\350\211\262 */\n"
"            font-size: 20px;\n"
"            border: 2px solid rgba(64, 83, 97, 1); /* \350\276\271\346\241\206\351\242\234\350\211\262 */\n"
"            border-radius: 5px;\n"
"            padding: 5px;\n"
"        }\n"
"\n"
"        QListWidget::item {\n"
"            background: rgb"
                        "a(64, 83, 97, 0.9);\n"
"            border: none;\n"
"            padding: 8px;\n"
"            margin: 2px;\n"
"        }\n"
"\n"
"        QListWidget::item:hover {\n"
"            background: rgba(90, 110, 120, 1); /* \351\274\240\346\240\207\346\202\254\346\265\256\350\203\214\346\231\257 */\n"
"        }\n"
"\n"
"        QListWidget::item:selected {\n"
"            background: rgba(128, 160, 190, 1); /* \351\200\211\344\270\255\351\241\271\350\203\214\346\231\257 */\n"
"            color: #000000; /* \351\200\211\344\270\255\351\241\271\346\226\207\345\255\227\351\242\234\350\211\262 */\n"
"        }\n"
"\n"
"        QStackedWidget {\n"
"            background-color: rgba(255, 255, 255, 1); /* \344\270\273\350\247\206\345\233\276\345\214\272\345\237\237\350\203\214\346\231\257\351\242\234\350\211\262 */\n"
"            border: 2px solid rgba(64, 83, 97, 1); /* \344\270\273\350\247\206\345\233\276\345\214\272\345\237\237\350\276\271\346\241\206 */\n"
"            border-radius: 5px;\n"
"            padding:"
                        " 10px;\n"
"        }\n"
"        QPushButton {\n"
"              background-color: rgba(64, 83, 97, 0.9); /* \346\214\211\351\222\256\350\203\214\346\231\257\351\242\234\350\211\262 */\n"
"              color: #FFFFFF; /* \346\214\211\351\222\256\346\226\207\345\255\227\351\242\234\350\211\262 */\n"
"              font-size: 22px;\n"
"              border: 2px solid rgba(64, 83, 97, 1); /* \346\214\211\351\222\256\350\276\271\346\241\206\351\242\234\350\211\262 */\n"
"              border-radius: 5px;\n"
"              padding: 8px 15px; /* \346\214\211\351\222\256\345\206\205\350\276\271\350\267\235 */\n"
"              margin: 5px; /* \345\242\236\345\212\240\345\244\226\350\276\271\350\267\235\350\256\251\351\230\264\345\275\261\346\230\276\345\276\227\346\230\216\346\230\276 */\n"
"          }\n"
"\n"
"          QPushButton:hover {\n"
"              background-color: rgba(90, 110, 120, 1); /* \351\274\240\346\240\207\346\202\254\346\265\256\350\203\214\346\231\257 */\n"
"              color: #E6E6E6; /* \346"
                        "\202\254\346\265\256\346\227\266\346\226\207\345\255\227\351\242\234\350\211\262 */\n"
"              border: 2px solid rgba(64, 83, 97, 1); /* \344\277\235\346\214\201\350\276\271\346\241\206\351\242\234\350\211\262 */\n"
"          }\n"
"\n"
"          QPushButton:pressed {\n"
"              background-color: rgba(128, 160, 190, 1); /* \346\214\211\344\270\213\346\227\266\350\203\214\346\231\257 */\n"
"              color: #000000; /* \346\214\211\344\270\213\346\227\266\346\226\207\345\255\227\351\242\234\350\211\262 */\n"
"              border: 2px solid rgba(90, 110, 120, 1); /* \346\214\211\344\270\213\346\227\266\350\276\271\346\241\206\351\242\234\350\211\262 */\n"
"          }\n"
"          QPushButton:disabled {\n"
"              background-color: rgba(200, 200, 200, 1); /* \347\246\201\347\224\250\346\227\266\350\203\214\346\231\257 */\n"
"              color: #A0A0A0; /* \347\246\201\347\224\250\346\227\266\346\226\207\345\255\227\351\242\234\350\211\262 */\n"
"              border: 2px solid rgba("
                        "180, 180, 180, 1); /* \347\246\201\347\224\250\346\227\266\350\276\271\346\241\206\351\242\234\350\211\262 */\n"
"          }\n"
"QLabel {\n"
"    background: rgba(85, 170, 255, 0.0); /* \345\256\214\345\205\250\351\200\217\346\230\216\350\203\214\346\231\257 */\n"
"    color: black;\n"
"    font-family: \"Times New Roman\";\n"
"    font-size: 25px;\n"
"    font-weight: bold; /* \345\212\240\347\262\227 */\n"
"}"));
        horizontalLayout_6 = new QHBoxLayout(EditInfo);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        pic_gv = new QGraphicsView(EditInfo);
        pic_gv->setObjectName(QString::fromUtf8("pic_gv"));

        horizontalLayout_4->addWidget(pic_gv);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);

        horizontalLayout_4->setStretch(0, 1);
        horizontalLayout_4->setStretch(1, 1);
        horizontalLayout_4->setStretch(2, 1);

        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(EditInfo);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setFamily(QString::fromUtf8("Times New Roman"));
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        horizontalLayout->addWidget(label);

        name_lb = new QLabel(EditInfo);
        name_lb->setObjectName(QString::fromUtf8("name_lb"));

        horizontalLayout->addWidget(name_lb);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_5 = new QLabel(EditInfo);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_5->addWidget(label_5);

        vip_lb = new QLabel(EditInfo);
        vip_lb->setObjectName(QString::fromUtf8("vip_lb"));

        horizontalLayout_5->addWidget(vip_lb);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(EditInfo);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        opwd_le = new QLineEdit(EditInfo);
        opwd_le->setObjectName(QString::fromUtf8("opwd_le"));

        horizontalLayout_2->addWidget(opwd_le);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_3 = new QLabel(EditInfo);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_3->addWidget(label_3);

        npwd_le = new QLineEdit(EditInfo);
        npwd_le->setObjectName(QString::fromUtf8("npwd_le"));

        horizontalLayout_3->addWidget(npwd_le);


        verticalLayout->addLayout(horizontalLayout_3);

        changepwd_pb = new QPushButton(EditInfo);
        changepwd_pb->setObjectName(QString::fromUtf8("changepwd_pb"));

        verticalLayout->addWidget(changepwd_pb);

        changepic_pb = new QPushButton(EditInfo);
        changepic_pb->setObjectName(QString::fromUtf8("changepic_pb"));

        verticalLayout->addWidget(changepic_pb);

        VipApply_pb = new QPushButton(EditInfo);
        VipApply_pb->setObjectName(QString::fromUtf8("VipApply_pb"));

        verticalLayout->addWidget(VipApply_pb);


        horizontalLayout_6->addLayout(verticalLayout);


        retranslateUi(EditInfo);

        QMetaObject::connectSlotsByName(EditInfo);
    } // setupUi

    void retranslateUi(QWidget *EditInfo)
    {
        EditInfo->setWindowTitle(QApplication::translate("EditInfo", "Form", nullptr));
        label->setText(QApplication::translate("EditInfo", "UserName:", nullptr));
        name_lb->setText(QApplication::translate("EditInfo", "usrname", nullptr));
        label_5->setText(QApplication::translate("EditInfo", "VIP:", nullptr));
        vip_lb->setText(QApplication::translate("EditInfo", "VIP_status", nullptr));
        label_2->setText(QApplication::translate("EditInfo", "Old Pwd:", nullptr));
        label_3->setText(QApplication::translate("EditInfo", "New Pwd:", nullptr));
        changepwd_pb->setText(QApplication::translate("EditInfo", "Change Pwd", nullptr));
        changepic_pb->setText(QApplication::translate("EditInfo", "Change Pic", nullptr));
        VipApply_pb->setText(QApplication::translate("EditInfo", "VIP Apply", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EditInfo: public Ui_EditInfo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITINFO_H
