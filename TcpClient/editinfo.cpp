#include "editinfo.h"
#include "ui_editinfo.h"

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMessageBox>
#include "tcpclient.h"
#include <QFileDialog>
#include "protocol.h"
#include <QInputDialog>
#include "vip.h"
#include "opewidget.h"

EditInfo::EditInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditInfo)
{
    ui->setupUi(this);
    QString usrname=TcpClient::getInstance().loginName();
    set_usrname(usrname);
    int VIP_status=TcpClient::getInstance().getVIPStatus();
    if (VIP_status==1){
        set_VIP_status("true");

    }else{
        set_VIP_status("false");
    }
    setWindowTitle("Personal Info");
//    set_pic();
    e_qtimer = new QTimer;

    connect(e_qtimer,SIGNAL(timeout()),this,SLOT(updatePicData())); //避免粘包
//    connect(Tcpclient::getInstance(),SIGNAL(update_ok()), this, SLOT(updatePicRespond()));
    connect(&TcpClient::getInstance(), &TcpClient::update_ok, this, &EditInfo::updatePicRespond);
}

EditInfo::~EditInfo()
{
    delete ui;
}

EditInfo &EditInfo::getInstance()
{
    static EditInfo instance;
    return instance;
}

void EditInfo::set_usrname(QString usrname)
{
    ui->name_lb->setText(usrname);
}

void EditInfo::set_VIP_status(QString VIP_status) //"true"或"false"
{
    ui->vip_lb->setText(VIP_status);
    if (VIP_status=="true"){
        ui->VipApply_pb->setEnabled(false);
        OpeWidget::getInstance().init_draw();
        OpeWidget::getInstance().getVIP()->setEnabled(true);

    }else{
        ui->VipApply_pb->setEnabled(true);
    }
    this->update();
}


void EditInfo::on_changepwd_pb_clicked()
{
     QString old_pwd = ui->opwd_le->text();
     QString new_pwd = ui->npwd_le->text();
     QString usrname=TcpClient::getInstance().loginName();
     if (old_pwd=="") {
         QMessageBox::warning(this,"Warning", "Old Password cannot be empty");
         return;
     }

     if (new_pwd=="") {
         QMessageBox::warning(this, "Warning", "New Password cannot be empty");
         return;
     }
     if (new_pwd==old_pwd) {
         QMessageBox::warning(this, "Warning", "Old Password and New Password cannot be the same");
         return;
     }

     // 构造 PDU 发送请求
     PDU *pdu = mkPDU(new_pwd.size());  // 这里的 64 是我们设定的消息长度，可以根据实际需要调整

     // 设置消息类型，假设 ENUM_MSG_TYPE_CHANGE_PWD_REQUEST 是你定义的修改密码请求的消息类型
     pdu->uiMsgType = ENUM_MSG_TYPE_CHANGE_PWD_REQUEST;
     // 将用户名和旧密码存入 caData
     memcpy(pdu->caData, usrname.toStdString().c_str(), std::min(usrname.size(), 32));  // 拷贝用户名，最多32个字符
     memcpy(pdu->caData + 32, old_pwd.toStdString().c_str(), std::min(old_pwd.size(), 32));  // 拷贝旧密码，最多32个字符

     // 将新密码存入 caMsg
     strncpy((char*)pdu->caMsg, new_pwd.toStdString().c_str(), new_pwd.size());  // 新密码存入 caMsg
     TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);

     // 清理
     free(pdu);
     pdu = NULL;
}

void EditInfo::set_pic(QString filePath)
{
    qDebug() <<"Change PIC"<<filePath;
    QPixmap pixmap(filePath);
    set_pic_by_pixmap(pixmap);
}

void EditInfo::set_pic_by_pixmap(QPixmap pixmap)
{
    ui->pic_gv->setFixedSize(200, 200);
    qDebug() << pixmap <<pixmap.isNull();
    if (!pixmap.isNull()) {
        // 如果当前场景已经存在，清空它
        QGraphicsScene *currentScene = ui->pic_gv->scene();
        if (currentScene) {
            currentScene->clear();  // 清空当前场景中的所有项
            delete currentScene;    // 释放场景内存
            qDebug() <<"清空本来的scene";
        }

        QGraphicsScene *scene = new QGraphicsScene();
        QGraphicsPixmapItem *pixmapItem = scene->addPixmap(pixmap);
        ui->pic_gv->setScene(scene);
        // 禁用 QGraphicsView 的滚动条
        ui->pic_gv->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->pic_gv->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        ui->pic_gv->setDragMode(QGraphicsView::NoDrag);  // 禁用平移
        ui->pic_gv->setInteractive(false);               // 禁用所有交互

        pixmapItem->setPixmap(pixmap.scaled(ui->pic_gv->size(),Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        qDebug() <<"添加scene完成";
    }

}

void EditInfo::updatePicData()
{
    // 不加密
    e_qtimer->stop();
    QFile file(Pic_UploadFilePath);
    qDebug() << "开始上传头像";
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"上传头像","打开头像文件失败");
        return;
    }
    char *pBuffer = new char[4096];
    qint64 res = 0;
    while(true)
    {
        res = file.read(pBuffer,4096);
        qDebug()<<"上传大小"<<res;
        if(res > 0 && res <= 4096)
        {
            TcpClient::getInstance().getTcpSocket().write(pBuffer,res);
        }
        else if(0 == res)
        {
            break;
        }
        else
        {
            QMessageBox::warning(this,"上传头像","打开头像失败：读头像文件失败");
            break;
        }
    }
    file.close();
    delete []pBuffer;
    pBuffer = NULL;

}

void EditInfo::updatePicRespond()
{
    set_pic(Pic_UploadFilePath);
//    Tcpclient::getInstance().setPicStatus(true);


}



void EditInfo::on_changepic_pb_clicked()
{
    Pic_UploadFilePath = QFileDialog::getOpenFileName(
                    this,                       // 父窗口
                    tr("Open Image File"),      // 标题
                    "",                         // 默认路径（空表示没有默认路径）
                    tr("Image Files (*.jpg *.png)")
                );
    //jpeg文件显示会有问题，所以忽略
    if(!Pic_UploadFilePath.isEmpty())
    {

        QPixmap pixmap(Pic_UploadFilePath);
        if (pixmap.isNull()){
            QMessageBox::warning(this, "warning","图片似乎有损坏，请上传其他图片");
            return;
        }
//        QStringList imageExtensions = {".jpg", ".jpeg", ".png", ".bmp", ".gif"};

//        QString fileExtension = QFileInfo(Pic_UploadFilePath).suffix().toLower();
        QString usrname=TcpClient::getInstance().loginName();

//        QString usrname_file =usrname+"."+fileExtension;
        QString usrname_file =usrname+".png"; //限定都为png
        QFile file(Pic_UploadFilePath);
        qint64 fileSize = file.size();
        QString strCurPath = PIC_DIR_PATH;
        PDU *pdu = mkPDU(strCurPath.toUtf8().size()+ 1); //会把 QString 转换为一个 QByteArray，并且这个 QByteArray 的内容是以 UTF-8 编码的字节流。
        pdu->uiMsgType = ENUM_MSG_TYPE_UPLOAD_FILE_REQUEST;
        memcpy(pdu->caMsg,strCurPath.toStdString().c_str(),strCurPath.size());
        sprintf(pdu->caData,"%s %lld",usrname_file.toStdString().c_str(),fileSize); //不加密
        TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu = NULL;
        e_qtimer->start(1000);
    }
    else
    {
        QMessageBox::warning(this,"上传头像","上传头像名字不能为空");
    }

}

void EditInfo::on_VipApply_pb_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("VIP Application"), tr("Please fill in the application information:"), QLineEdit::Normal, "", &ok);

   // 检查用户是否点击了“确定”按钮
   if (ok && !text.isEmpty()) {
       // 如果用户输入了文字，进行处理
//       QMessageBox::information(this, tr("输入的内容"), tr("你输入的内容是: ") + text);

       PDU *pdu = mkPDU(text.toUtf8().size()); //会把 QString 转换为一个 QByteArray，并且这个 QByteArray 的内容是以 UTF-8 编码的字节流。
       pdu->uiMsgType = ENUM_MSG_TYPE_VIP_APPLY_REQUEST;
       QString usrname=TcpClient::getInstance().loginName();
       memcpy(pdu->caMsg,text.toStdString().c_str(),text.toUtf8().size());
       strncpy(pdu->caData,usrname.toStdString().c_str(),usrname.size());
       TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
       free(pdu);
       pdu = NULL;

       // 在这里你可以处理用户输入的内容，比如保存到数据库、做其他逻辑处理等
   } else {
       // 如果用户取消了或者输入为空
       QMessageBox::information(this, tr("取消"), tr("你没有输入任何内容"));
   }
}
