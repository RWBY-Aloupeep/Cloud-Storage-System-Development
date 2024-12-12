#include "opewidget.h"
#include "editinfo.h"
#include "tcpclient.h"
#include "privatechat.h"
#include "groupchat.h"
#include "sharefile.h"
#include "creategroup.h"
#include <QRandomGenerator>
#include <QPainter>

// 操作主界面UI
OpeWidget::OpeWidget(QWidget *parent) : QWidget(parent)
{
    m_pListW = new QListWidget(this);
    m_pListW->addItem("Friends");
    m_pListW->addItem("Collections");
    m_pListW->addItem("VIP");

    // ------------------------增添编辑信息按钮---------------------------------------------------
       // 创建按钮
       m_pEditUsrInfo = new QPushButton("Edit UsrInfo", this);

       // 创建布局并添加QListWidget和按钮
       QVBoxLayout *listLayout = new QVBoxLayout;
       listLayout->addWidget(m_pListW);
       listLayout->addWidget(m_pEditUsrInfo);  // 添加按钮到列表下方

       // -----------------------------------------------------------------------------------------

    m_pFriend = new Friend;
    m_pBook = new Book;
    m_pVIP = new VIP;
    m_pSW = new QStackedWidget;
    m_pSW->addWidget(m_pFriend);
    m_pSW->addWidget(m_pBook);
    m_pSW->addWidget(m_pVIP);
    QHBoxLayout *pMain = new QHBoxLayout;
    pMain->addLayout(listLayout);    //修改
    pMain->addWidget(m_pSW);

    setLayout(pMain);
    connect(m_pListW,SIGNAL(currentRowChanged(int)),m_pSW,SLOT(setCurrentIndex(int)));
    // -------------------vip---------------------------------------------------
    connect(m_pEditUsrInfo,SIGNAL(clicked(bool)),this,SLOT(EditUsrInfo()));
    connect(m_pListW, &QListWidget::currentRowChanged, this, [this](int currentRow) {
        // 切换页面
        m_pSW->setCurrentIndex(currentRow);

        // 判断是否是 VIP
        if (currentRow == 2) { // 假设 "VIP" 在列表中的索引为 2
            // 如果不是 VIP，弹出提示框
            if (TcpClient::getInstance().getVIPStatus() != 1) {
                QMessageBox::information(this, "Reminder", "You are not a VIP yet.\nPlease upgrade to VIP.");
          }
        }
    });
    // -------------------------------------------------------------------------

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
                  border: 2px solid rgba(64, 83, 97, 1); /* 按钮边框颜色 */
                  border-radius: 5px;
                  padding: 8px 15px; /* 按钮内边距 */
                  margin: 5px; /* 增加外边距让阴影显得明显 */
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
              QPushButton:disabled {
                  background-color: rgba(200, 200, 200, 1); /* 禁用时背景 */
                  color: #A0A0A0; /* 禁用时文字颜色 */
                  border: 2px solid rgba(180, 180, 180, 1); /* 禁用时边框颜色 */
              }
        )");
}

// 生成OpeWidget实例对象
OpeWidget &OpeWidget::getInstance()
{
    static OpeWidget instance;
    return instance;
}

// 返回当前好友的操作对象
Friend *OpeWidget::getFriend()
{
    return m_pFriend;
}

// 获取图书对象
Book *OpeWidget::getBook()
{
    return m_pBook;
}

//获取VIP对象
VIP *OpeWidget::getVIP()
{
    return m_pVIP;
}

void OpeWidget::EditUsrInfo()
{
    if(EditInfo::getInstance().isHidden())
        {
            EditInfo::getInstance().show();
        }
        QPixmap temp_pixmap=TcpClient::getInstance().getpixmap();
        if (!temp_pixmap.isNull()){
            qDebug()<<"Have an initial avatar";
            EditInfo::getInstance().set_pic_by_pixmap(temp_pixmap);
        }else{
            EditInfo::getInstance().set_pic();
        }
}

void OpeWidget::closeEvent(QCloseEvent *event)
{
    // 自行决定哪些要关闭，记得加入头文件
    EditInfo::getInstance().close();
    PrivateChat::getInstance().close();
    ShareFile::getInstance().close();
    GroupChat::getInstance().close();
    CreateGroup::getInstance().close();
    event->accept();
}

void OpeWidget::init_draw(){
    if (TcpClient::getInstance().getVIPStatus()==1){
        // 初始化雪花定时器
        snowfallTimer = new QTimer(this);
        connect(snowfallTimer, &QTimer::timeout, this, &OpeWidget::updateSnowflakes);
        snowfallTimer->start(50);  // 每50毫秒更新一次

        generateSnowflakes();  // 生成雪花
    }
}



void OpeWidget::generateSnowflakes() {
    // 生成一些初始雪花
    int numberOfSnowflakes = 100;
    for (int i = 0; i < numberOfSnowflakes; ++i) {
        Snowflake snowflake;
        snowflake.x = QRandomGenerator::global()->bounded(width());
        snowflake.y = QRandomGenerator::global()->bounded(height());
        snowflake.speed = QRandomGenerator::global()->bounded(1, 4);    // 雪花飘落的速度
        snowflake.color = QColor(255, 255, 255);  // 白色雪花
        snowflake.color = QColor(255, 223, 0); // 金色
        snowflakes.append(snowflake);
    }
}

void OpeWidget::drawSnowflakes(QPainter &painter) {
    // 绘制雪花
//    for (const Snowflake &snowflake : snowflakes) {
//        painter.setPen(Qt::NoPen);
//        painter.setBrush(snowflake.color);
//        painter.drawEllipse(QPointF(snowflake.x, snowflake.y), 3, 3);  // 绘制一个圆形雪花
//    }
    // 绘制VIP
//    QFont font = painter.font();
//        font.setPointSize(12);  // 设置字体大小
//        painter.setFont(font);
//        painter.setPen(QPen(Qt::black));  // 字母颜色，可以根据需要设置为金色
//        painter.setBrush(Qt::yellow);  // 使用金色填充字母

//        for (const Snowflake &snowflake : snowflakes) {
//            // 绘制"VIP"字母
//            painter.drawText(QPointF(snowflake.x, snowflake.y), "Offer");
//        }


        QPolygonF star;  // 定义五角星形状
            star << QPointF(0, -10)
                 << QPointF(3, -3)
                 << QPointF(10, -3)
                 << QPointF(5, 3)
                 << QPointF(7, 10)
                 << QPointF(0, 5)
                 << QPointF(-7, 10)
                 << QPointF(-5, 3)
                 << QPointF(-10, -3)
                 << QPointF(-3, -3)
                 << QPointF(0, -10);  // 关闭五角星路径

            painter.setPen(Qt::NoPen);  // 不画边框
            painter.setBrush(QColor(255, 223, 0));  // 金色雪花

            for (const Snowflake &snowflake : snowflakes) {
                QTransform transform;
                transform.translate(snowflake.x, snowflake.y);  // 移动到雪花的位置
                painter.setTransform(transform);  // 设置绘制变换
                painter.drawPolygon(star);  // 绘制五角星
            }
}

void OpeWidget::updateSnowflakes() {
    // 更新雪花的位置
//    qDebug() << "Updating snowflakes...";
    for (Snowflake &snowflake : snowflakes) {
        snowflake.y += snowflake.speed;  // 每次更新时让雪花下落
        if (snowflake.y > height()) {  // 如果雪花超出了窗口底部，重新设置其位置
            snowflake.y = -3;
            snowflake.x = QRandomGenerator::global()->bounded(width());
        }
//        qDebug() << snowflake.x <<snowflake.y;
    }
    update();  // 重新绘制界面
}

void OpeWidget::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // 开启抗锯齿
//    painter.fillRect(rect(), QColor(0, 0, 0));  // 用黑色填充背景
    painter.fillRect(rect(), QColor(255, 255, 255));
    // 设置合成模式，确保雪花绘制时不会被遮挡
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    drawSnowflakes(painter);  // 绘制雪花
}
