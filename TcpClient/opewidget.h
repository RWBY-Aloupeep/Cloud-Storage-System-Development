#ifndef OPEWIDGET_H
#define OPEWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QLabel> // 添加 QLabel
#include "friend.h"
#include "book.h"
#include <QStackedWidget>
#include "vip.h"

class OpeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OpeWidget(QWidget *parent = nullptr);
    static OpeWidget &getInstance();
    Friend *getFriend();
    Book *getBook();
    VIP *getVIP();  
    void init_draw();

public slots:
    void EditUsrInfo();

signals:

protected:
   void closeEvent(QCloseEvent *event) override;
   // 声明 paintEvent 方法
    void paintEvent(QPaintEvent *event) override;

private:
    QListWidget *m_pListW;
    Friend *m_pFriend;
    Book *m_pBook;
    QStackedWidget *m_pSW;
    VIP *m_pVIP;
    QPushButton *m_pEditUsrInfo;

    // 雪花效果相关变量
   struct Snowflake {
       float x, y;
       float speed;
       QColor color;
   };

   QVector<Snowflake> snowflakes;  // 存储雪花的容器
   QTimer *snowfallTimer;  // 定时器
   void generateSnowflakes();  // 生成雪花
   void drawSnowflakes(QPainter &painter);  // 绘制雪花
   void updateSnowflakes();  // 更新雪花位置
};


#endif // OPEWIDGET_H
