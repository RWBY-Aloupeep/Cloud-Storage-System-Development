#include "tcpserver.h"
#include "ui_tcpserver.h"
#include "mytcpserver.h"
#include <QByteArray>
#include <QDebug> // 调试
#include <QMessageBox> // 消息提示框
#include <QHostAddress>
#include <QFile>

#include <QSqlTableModel>
#include <QSqlError>
#include <QSqlDatabase>
#include <QListWidgetItem>



TcpServer::TcpServer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TcpServer)
{
    ui->setupUi(this);
    loadConfig();
    MyTcpServer::getInstance().listen(QHostAddress(m_strIP),m_usPort);
    // Load the tables from the opened database
    QStringList tables = OpeDB::getInstance().getMainDb().tables();
    tables.removeAll("sqlite_sequence");
    ui->listWidgetTables->clear();
    ui->listWidgetTables->addItems(tables);

    // Connect the list widget’s selection change to a slot
    connect(ui->listWidgetTables, &QListWidget::itemSelectionChanged,
            this, &TcpServer::onTableSelectionChanged);
    connect(ui->pushButtonRefresh, &QPushButton::clicked,
        this, &TcpServer::on_pushButtonRefresh_clicked);

    setStyleSheet(R"(
            QWidget {
                background-color: rgba(245, 245, 245, 1); /* 主背景浅灰 */
                font-family: "Arial";
                font-size: 16px;
            }

            QListWidget {
                background-color: rgba(64, 83, 97, 0.9); /* 列表背景颜色 */
                color: #FFFFFF; /* 列表文字颜色 */
                font-size: 20px;
                border: 2px solid rgba(64, 83, 97, 1); /* 边框颜色 */
                border-radius: 5px;
                padding: 5px;
            }

            QListWidget::item {
                background: rgba(64, 83, 97, 0.9);
                border: none;
                padding: 8px;
                margin: 2px;
            }

            QListWidget::item:hover {
                background: rgba(90, 110, 120, 1); /* 鼠标悬浮背景 */
            }

            QListWidget::item:selected {
                background: rgba(128, 160, 190, 1); /* 选中项背景 */
                color: #000000; /* 选中项文字颜色 */
            }

          /* 禁用项的样式 */
          QListWidget::item:disabled {
              background: rgba(200, 200, 200, 1); /* 禁用项背景 */
              color: #A0A0A0; /* 禁用项文字颜色 */
              border: none; /* 如果需要可以设置边框 */
          }
            QStackedWidget {
                background-color: rgba(255, 255, 255, 1); /* 主视图区域背景颜色 */
                border: 2px solid rgba(64, 83, 97, 1); /* 主视图区域边框 */
                border-radius: 5px;
                padding: 10px;
            }
            QPushButton {
                  background-color: rgba(64, 83, 97, 0.9); /* 按钮背景颜色 */
                  color: #FFFFFF; /* 按钮文字颜色 */
                  font-size: 18px;
              }

              QPushButton:hover {
                  background-color: rgba(90, 110, 120, 1); /* 鼠标悬浮背景 */
                  color: #E6E6E6; /* 悬浮时文字颜色 */
                  border: 2px solid rgba(64, 83, 97, 1); /* 保持边框颜色 */
              }

              QPushButton:pressed {
                  background-color: rgba(128, 160, 190, 1); /* 按下时背景 */
                  color: #000000; /* 按下时文字颜色 */
                  border: 2px solid rgba(90, 110, 120, 1); /* 按下时边框颜色 */
              }

        )");
}


void TcpServer::onTableSelectionChanged()
{
    QList<QListWidgetItem*> selectedItems = ui->listWidgetTables->selectedItems();
    if (selectedItems.isEmpty()) {
        return;
    }

    QString selectedTable = selectedItems.first()->text();

    // Create a new model for the selected table
    QSqlTableModel *model = new QSqlTableModel(this, OpeDB::getInstance().getMainDb());
    model->setTable(selectedTable);
    model->setEditStrategy(QSqlTableModel::OnRowChange);

    if (!model->select()) {
        QMessageBox::warning(this, "Database Error", model->lastError().text());
        return;
    }

    ui->tableView->setModel(model);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
}

void TcpServer::on_pushButtonRefresh_clicked()
{
    QList<QListWidgetItem*> selectedItems = ui->listWidgetTables->selectedItems();
    if (selectedItems.isEmpty()) {
        // No table selected, nothing to refresh
        return;
    }

    QString selectedTable = selectedItems.first()->text();

    // Retrieve the model currently in use
    QSqlTableModel *model = qobject_cast<QSqlTableModel*>(ui->tableView->model());
    if (!model) {
        // If no model is currently set, just try loading from scratch
        onTableSelectionChanged();
        return;
    }

    // Refresh the data by re-selecting from the database
    if (!model->select()) {
        QMessageBox::warning(this, "Database Error", model->lastError().text());
    }
}


TcpServer::~TcpServer()
{
    delete ui;
}

void TcpServer::loadConfig()
{
    QFile file(":/server.config"); // 文件对象，读取资源文件 ':' + "前缀" + "文件名"
    if(file.open(QIODevice::ReadOnly)) // file.open() 参数：打开方式：只读（注意，这里只读是写在QIODevice下的枚举，所以调用要声明命名空间） 返回true则打开成功
    {
        QByteArray baData = file.readAll(); // 读出所有数据，返回字节数组QByteArray
        QString strData = baData.toStdString().c_str(); // 转换为字符串 注意std::string不能自动转为QString，还需转为char*
        file.close();

        strData.replace("\r\n", " "); // 替换IP地址与端口号之间\r\n
        QStringList strList = strData.split(" ");
        m_strIP = strList.at(0);
        m_usPort = strList.at(1).toUShort(); // 无符号短整型
        qDebug() << "IP:" << m_strIP << " port:" << m_usPort; // 打印结果
    }
    else // 文件打开失败则弹出提示窗口
    {
        QMessageBox::critical(this, "open config", "open config failed"); // 严重
    }
}

TcpServer &TcpServer::getInstance()
{
    static TcpServer instance;
    return instance;
}
