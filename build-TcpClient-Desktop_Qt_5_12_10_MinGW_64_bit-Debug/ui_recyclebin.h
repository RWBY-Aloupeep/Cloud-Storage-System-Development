/********************************************************************************
** Form generated from reading UI file 'recyclebin.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RECYCLEBIN_H
#define UI_RECYCLEBIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RecycleBin
{
public:
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QListWidget *allfile_lw;
    QPushButton *flushfile_pb;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *back_pb;
    QPushButton *deleFile_pb;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QListWidget *bin_lw;
    QPushButton *flushbin_pb;
    QHBoxLayout *horizontalLayout;
    QPushButton *clearBin_pb;
    QPushButton *recov_pb;

    void setupUi(QWidget *RecycleBin)
    {
        if (RecycleBin->objectName().isEmpty())
            RecycleBin->setObjectName(QString::fromUtf8("RecycleBin"));
        RecycleBin->resize(384, 301);
        verticalLayout_5 = new QVBoxLayout(RecycleBin);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(RecycleBin);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setFamily(QString::fromUtf8("Times New Roman"));
        font.setPointSize(10);
        font.setBold(false);
        font.setWeight(50);
        label->setFont(font);

        verticalLayout->addWidget(label);


        verticalLayout_4->addLayout(verticalLayout);

        allfile_lw = new QListWidget(RecycleBin);
        allfile_lw->setObjectName(QString::fromUtf8("allfile_lw"));

        verticalLayout_4->addWidget(allfile_lw);

        flushfile_pb = new QPushButton(RecycleBin);
        flushfile_pb->setObjectName(QString::fromUtf8("flushfile_pb"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Times New Roman"));
        font1.setPointSize(10);
        flushfile_pb->setFont(font1);

        verticalLayout_4->addWidget(flushfile_pb);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        back_pb = new QPushButton(RecycleBin);
        back_pb->setObjectName(QString::fromUtf8("back_pb"));
        back_pb->setFont(font);

        horizontalLayout_2->addWidget(back_pb);

        deleFile_pb = new QPushButton(RecycleBin);
        deleFile_pb->setObjectName(QString::fromUtf8("deleFile_pb"));
        deleFile_pb->setFont(font);

        horizontalLayout_2->addWidget(deleFile_pb);


        verticalLayout_4->addLayout(horizontalLayout_2);


        horizontalLayout_3->addLayout(verticalLayout_4);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_2 = new QLabel(RecycleBin);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font);

        verticalLayout_2->addWidget(label_2);

        bin_lw = new QListWidget(RecycleBin);
        bin_lw->setObjectName(QString::fromUtf8("bin_lw"));

        verticalLayout_2->addWidget(bin_lw);

        flushbin_pb = new QPushButton(RecycleBin);
        flushbin_pb->setObjectName(QString::fromUtf8("flushbin_pb"));
        flushbin_pb->setFont(font1);

        verticalLayout_2->addWidget(flushbin_pb);


        verticalLayout_3->addLayout(verticalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        clearBin_pb = new QPushButton(RecycleBin);
        clearBin_pb->setObjectName(QString::fromUtf8("clearBin_pb"));
        clearBin_pb->setFont(font1);

        horizontalLayout->addWidget(clearBin_pb);

        recov_pb = new QPushButton(RecycleBin);
        recov_pb->setObjectName(QString::fromUtf8("recov_pb"));
        recov_pb->setFont(font);

        horizontalLayout->addWidget(recov_pb);


        verticalLayout_3->addLayout(horizontalLayout);


        horizontalLayout_3->addLayout(verticalLayout_3);


        verticalLayout_5->addLayout(horizontalLayout_3);


        retranslateUi(RecycleBin);

        QMetaObject::connectSlotsByName(RecycleBin);
    } // setupUi

    void retranslateUi(QWidget *RecycleBin)
    {
        RecycleBin->setWindowTitle(QApplication::translate("RecycleBin", "Form", nullptr));
        label->setText(QApplication::translate("RecycleBin", "Current File", nullptr));
        flushfile_pb->setText(QApplication::translate("RecycleBin", "Flush Current File", nullptr));
        back_pb->setText(QApplication::translate("RecycleBin", "Back", nullptr));
        deleFile_pb->setText(QApplication::translate("RecycleBin", "Delete", nullptr));
        label_2->setText(QApplication::translate("RecycleBin", "Recycle Bin", nullptr));
        flushbin_pb->setText(QApplication::translate("RecycleBin", "Flush Bin", nullptr));
        clearBin_pb->setText(QApplication::translate("RecycleBin", "Clear Bin", nullptr));
        recov_pb->setText(QApplication::translate("RecycleBin", "Recover", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RecycleBin: public Ui_RecycleBin {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RECYCLEBIN_H
