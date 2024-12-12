#include "vip.h"
#include <QLabel>
#include <QPixmap>
#include <QListWidgetItem>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "tcpclient.h"
#include <QTimer>

VIP::VIP(QWidget *parent) : QWidget(parent)
{
    m_strEnterBin.clear();
    m_pBinListWidget = new QListWidget;
    m_pFlushBin = new QPushButton("Flush Bin");
    m_pClearBin = new QPushButton("Clear Bin");
    m_pRecoverFile = new QPushButton("Recover");
    m_pzipFile = new QPushButton("Zip File");
    m_pUnzipFile = new QPushButton("UnZip File");

    // 创建按钮布局
    QVBoxLayout *buttonLayout = new QVBoxLayout;
    buttonLayout->addWidget(m_pFlushBin);
    buttonLayout->addWidget(m_pClearBin);
    buttonLayout->addWidget(m_pRecoverFile);
    buttonLayout->addWidget(m_pzipFile);
    buttonLayout->addWidget(m_pUnzipFile);

    // 创建主布局
    QHBoxLayout *pMain = new QHBoxLayout;
    pMain->addWidget(m_pBinListWidget);
    pMain->addLayout(buttonLayout);

    // 设置布局
    setLayout(pMain);
    connect(m_pClearBin,SIGNAL(clicked(bool)),this,SLOT(on_clearBin_pb_clicked()));
    connect(m_pFlushBin,SIGNAL(clicked(bool)),this,SLOT(flushBin()));
    connect(m_pRecoverFile,SIGNAL(clicked(bool)),this,SLOT(on_recoverfile_pb_clicked()));
    connect(m_pBinListWidget,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(enterBin(QModelIndex)));
    connect(m_pzipFile,SIGNAL(clicked(bool)),this,SLOT(on_zipfile_pb_clicked()));
    connect(m_pUnzipFile,SIGNAL(clicked(bool)),this,SLOT(on_unzipfile_pb_clicked()));
}

void VIP::updateBinList(const PDU *pdu)
{
    if(NULL == pdu)
    {
        return;
    }
    QListWidgetItem *pItemTemp = NULL;
    int row = m_pBinListWidget->count();
    while(m_pBinListWidget->count() > 0)
    {
        pItemTemp = m_pBinListWidget->item(row - 1);
        m_pBinListWidget->removeItemWidget(pItemTemp);
        delete pItemTemp;
        row -= 1;
    }
    FileInfo *pFileInfo = NULL;
    int iCount = pdu->uiMsgLen / sizeof(FileInfo);
    for(int i = 0;i<iCount;i++)
    {
        pFileInfo = (FileInfo*)(pdu->caMsg) + i;

        // 跳过特殊文件夹
        QString fileName = pFileInfo->caFileName;
        if (fileName == "." || fileName == "..")
        {
            continue;
        }

        QListWidgetItem *pItem = new QListWidgetItem;
        if(0 == pFileInfo->iFileType)
        {
            pItem->setIcon(QIcon(QPixmap(":/icon/dir.png")));
        }
        else if(1 == pFileInfo->iFileType)
        {
            if (fileName.endsWith(".zip", Qt::CaseInsensitive))
            {
                pItem->setIcon(QIcon(QPixmap(":/icon/zip.png")));
            }
            else
            {
                pItem->setIcon(QIcon(QPixmap(":/icon/reg.png")));
            }
        }
        pItem->setText(pFileInfo->caFileName);
        m_pBinListWidget->addItem(pItem);
    }
}

void VIP::clearEnterBin()
{
    m_strEnterBin.clear();
}

QString VIP::enterBin()
{
    return m_strEnterBin;
}

void VIP::on_clearBin_pb_clicked()
{
    QString userName = TcpClient::getInstance().loginName();

    // 创建并发送清空回收站的请求
    PDU *pdu = mkPDU(userName.size() + 1);
    pdu->uiMsgType = ENUM_MSG_TYPE_CLEAR_BIN_REQUEST;
    strncpy(pdu->caData, userName.toStdString().c_str(), userName.size());
    TcpClient::getInstance().getTcpSocket().write((char*)pdu, pdu->uiPDULen);
    free(pdu);
    pdu = NULL;
}

void VIP::on_recoverfile_pb_clicked()
{
    QListWidgetItem *pItem = m_pBinListWidget->currentItem();
    if (NULL == pItem) {
        QMessageBox::warning(this, "Recover", "Please select an item to recover.");
        return;
    }
    else
    {
        QString fileName = pItem->text();
        QString userName = TcpClient::getInstance().loginName();
        QString recoverPath = "./" + userName + "/RecycleBin/" + fileName;
        qDebug()<<recoverPath;

        QString strCurPath = TcpClient::getInstance().curPath();
        QString m_strDestDir = "./" + userName;
        int srcLen = recoverPath.size();
        int destLen = m_strDestDir.size();
        PDU *pdu = mkPDU(srcLen + destLen + 2);
        pdu->uiMsgType = ENUM_MSG_TYPE_RECOVER_REQUEST;
        sprintf(pdu->caData,"%d %d %s",srcLen,destLen,fileName.toStdString().c_str());
        memcpy(pdu->caMsg,recoverPath.toStdString().c_str(),srcLen);
        memcpy((char*)(pdu->caMsg) + srcLen + 1,m_strDestDir.toStdString().c_str(),destLen);
        TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu = NULL;
    }
}

void VIP::flushBin()
{
    QString strCurPath = TcpClient::getInstance().curPath() + "/RecycleBin";
    PDU *pdu = mkPDU(strCurPath.size()+1);
    pdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_BIN_REQUEST;
    strncpy((char*)(pdu->caMsg),strCurPath.toStdString().c_str(),strCurPath.size());
    TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
    free(pdu);
    pdu = NULL;
}

void VIP::enterBin(const QModelIndex &index)
{
    QString BinDirName = index.data().toString();
    m_strEnterBin = BinDirName;
    QString strCurPath = TcpClient::getInstance().curPath();
    PDU *pdu = mkPDU(strCurPath.size() + 1);
    pdu->uiMsgType = ENUM_MSG_TYPE_ENTER_BIN_REQUEST;
    strncpy(pdu->caData,BinDirName.toStdString().c_str(),BinDirName.size());
    memcpy(pdu->caMsg,strCurPath.toStdString().c_str(),strCurPath.size());
    TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
    qDebug()<<strCurPath;
    free(pdu);
    pdu = NULL;
}

void VIP::on_zipfile_pb_clicked()
{
    QListWidgetItem *pItem = m_pBinListWidget->currentItem();
    if (NULL == pItem) {
        QMessageBox::warning(this, "Zip", "Please select an item to zip.");
        return;
    }
    else
    {
        QString fileName = pItem->text();
        if (fileName.endsWith(".zip", Qt::CaseInsensitive)) {
            QMessageBox::information(this, "Zip", "The selected file is already a zip file.");
            return;
        }
        QString userName = TcpClient::getInstance().loginName();
        QString zipPath = "./" + userName + "/RecycleBin/" + fileName;
        qDebug()<<zipPath;
        int srcLen = zipPath.size();
        PDU *pdu = mkPDU(srcLen+1);
        pdu->uiMsgType = ENUM_MSG_TYPE_ZIP_REQUEST;
        memcpy(pdu->caMsg,zipPath.toStdString().c_str(),srcLen);
        TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu = NULL;
    }
}

void VIP::on_unzipfile_pb_clicked()
{
    QListWidgetItem *pItem = m_pBinListWidget->currentItem();
    if (NULL == pItem) {
        QMessageBox::warning(this, "UnZip", "Please select an item to unzip.");
        return;
    }
    else
    {
        QString fileName = pItem->text();
        if (! fileName.endsWith(".zip", Qt::CaseInsensitive)) {
            QMessageBox::information(this, "UnZip", "The selected file is not a zip file.");
            return;
        }
        QString userName = TcpClient::getInstance().loginName();
        QString zipPath = "./" + userName + "/RecycleBin/" + fileName;
        qDebug()<<zipPath;
        int srcLen = zipPath.size();
        PDU *pdu = mkPDU(srcLen+1);
        pdu->uiMsgType = ENUM_MSG_TYPE_UNZIP_REQUEST;
        memcpy(pdu->caMsg,zipPath.toStdString().c_str(),srcLen);
        TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu = NULL;
    }

}
