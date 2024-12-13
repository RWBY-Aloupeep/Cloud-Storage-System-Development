/********************************************************************************
** Form generated from reading UI file 'tcpserver.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TCPSERVER_H
#define UI_TCPSERVER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TcpServer
{
public:
    QListWidget *listWidgetTables;
    QTableView *tableView;
    QPushButton *pushButtonRefresh;

    void setupUi(QWidget *TcpServer)
    {
        if (TcpServer->objectName().isEmpty())
            TcpServer->setObjectName(QString::fromUtf8("TcpServer"));
        TcpServer->resize(800, 600);
        listWidgetTables = new QListWidget(TcpServer);
        listWidgetTables->setObjectName(QString::fromUtf8("listWidgetTables"));
        listWidgetTables->setGeometry(QRect(20, 20, 211, 551));
        tableView = new QTableView(TcpServer);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setGeometry(QRect(250, 20, 521, 521));
        pushButtonRefresh = new QPushButton(TcpServer);
        pushButtonRefresh->setObjectName(QString::fromUtf8("pushButtonRefresh"));
        pushButtonRefresh->setGeometry(QRect(250, 550, 521, 31));

        retranslateUi(TcpServer);

        QMetaObject::connectSlotsByName(TcpServer);
    } // setupUi

    void retranslateUi(QWidget *TcpServer)
    {
        TcpServer->setWindowTitle(QApplication::translate("TcpServer", "TcpServer", nullptr));
        pushButtonRefresh->setText(QApplication::translate("TcpServer", "Refresh", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TcpServer: public Ui_TcpServer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TCPSERVER_H
