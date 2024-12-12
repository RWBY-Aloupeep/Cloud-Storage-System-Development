#ifndef EDITINFO_H
#define EDITINFO_H

#include <QWidget>
#include <QResizeEvent>
#include <QTimer>

namespace Ui {
class EditInfo;
}

class EditInfo : public QWidget
{
    Q_OBJECT

public:
    explicit EditInfo(QWidget *parent = nullptr);
    ~EditInfo();
    static EditInfo &getInstance();
    void set_usrname(QString usrname);
    void set_VIP_status(QString VIP_status);
    void set_pic(QString filePath = "://icon//person.jpg");
    void set_pic_by_pixmap(QPixmap pixmap);

private slots:
    void on_changepwd_pb_clicked();
    void on_changepic_pb_clicked();
    void updatePicData(); // 进行文件上传

    void on_VipApply_pb_clicked();

public slots:
    void updatePicRespond(); //处理成功请求
private:
    Ui::EditInfo *ui;
    QString Pic_UploadFilePath;
    QTimer *e_qtimer;
};

#endif // EDITINFO_H
