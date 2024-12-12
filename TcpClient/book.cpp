#include "book.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>
#include "opewidget.h"
#include "tcpclient.h"
#include "sharefile.h"
#include <aescbc.h>
#include <QString>
#include <QStringList>
#include <QtCore>

Book::Book(QWidget *parent) : QWidget(parent)
{
    m_strEnterDir.clear();
    m_bDownload = false;
    m_pTimer = new QTimer;
    m_pBookListW = new QListWidget;
    m_pReturnPB = new QPushButton("Back");
    m_pCreateDirPB = new QPushButton("Create Folder");
    m_pDelDirPB = new QPushButton("Delete Folder");
    m_pRenamePB = new QPushButton("Rename File");
    m_pFlushFilePB = new QPushButton("Flush File");
    // Existing initialization of the widgets
    m_pSelectDirPB = new QPushButton("target dictionary");
    m_pSelectDirPB->setEnabled(false);
    // New initialization
    m_pSearchLineEdit = new QLineEdit;
    m_pSearchLineEdit->setPlaceholderText("Enter file name keyword...");
    m_pSearchButton = new QPushButton("Search");
    QVBoxLayout *pDirVBL = new QVBoxLayout;
    m_pSearchResultList = new QListWidget;
    m_pSearchResultList->hide();  // Initially hidden

    pDirVBL->addWidget(m_pReturnPB);
    pDirVBL->addWidget(m_pCreateDirPB);
    pDirVBL->addWidget(m_pDelDirPB);
    pDirVBL->addWidget(m_pRenamePB);
    pDirVBL->addWidget(m_pFlushFilePB);
    m_pUploadPB = new QPushButton("Upload File");
    m_DownLoadPB = new QPushButton("Download File");
    m_pDelFilePB = new QPushButton("Delete File");
    m_pShareFilePB = new QPushButton("Share File");
    m_pMoveFilePB = new QPushButton("Move File");
    m_pSelectDirPB = new QPushButton("Target Path");
    m_pSelectDirPB->setEnabled(false);
    QVBoxLayout *pFileVBL = new QVBoxLayout;
    pFileVBL->addWidget(m_pUploadPB);
    pFileVBL->addWidget(m_DownLoadPB);
    pFileVBL->addWidget(m_pDelFilePB);
    pFileVBL->addWidget(m_pShareFilePB);
    pFileVBL->addWidget(m_pMoveFilePB);
    pFileVBL->addWidget(m_pSelectDirPB);
    QHBoxLayout *pMain = new QHBoxLayout;
    pMain->addWidget(m_pBookListW);
    pMain->addLayout(pDirVBL);
    pMain->addLayout(pFileVBL);
    //setLayout(pMain);
    // New search bar layout
    QHBoxLayout *pSearchLayout = new QHBoxLayout;
    pSearchLayout->addWidget(m_pSearchLineEdit);
    pSearchLayout->addWidget(m_pSearchButton);

    // Combine layouts
    QVBoxLayout *pMainLayout = new QVBoxLayout;
    pMainLayout->addLayout(pMain);       // Now pMain can be added safely
    pMainLayout->addLayout(pSearchLayout);
    pMainLayout->addWidget(m_pSearchResultList);  // Add search results list

    // Initialize upload progress bar
    m_pUploadProgressBar = new QProgressBar;
    m_pUploadProgressBar->setRange(0, 100); // Progress goes from 0% to 100%
    m_pUploadProgressBar->setValue(0);      // Start at 0%
    m_pUploadProgressBar->setVisible(false); // Hidden initially
    // Initialize download progress bar
    m_pDownloadProgressBar = new QProgressBar;
    m_pDownloadProgressBar->setRange(0, 100); // Progress goes from 0% to 100%
    m_pDownloadProgressBar->setValue(0);      // Start at 0%
    m_pDownloadProgressBar->setVisible(false); // Hidden initially
    // Initialize percentage label
    m_pPercentageLabel = new QLabel("0%");
    m_pPercentageLabel->setAlignment(Qt::AlignCenter);
    m_pPercentageLabel->setVisible(false); // Hidden initially
    // Initialize time remaining label
    m_pTimeRemainingLabel = new QLabel("Time Remaining: Calculating...");
    m_pTimeRemainingLabel->setAlignment(Qt::AlignCenter);
    m_pTimeRemainingLabel->setVisible(false); // Hidden initially


    QHBoxLayout *pLabelLayout = new QHBoxLayout;
    pLabelLayout->addWidget(m_pPercentageLabel);
    pLabelLayout->addWidget(m_pTimeRemainingLabel);

    // Add progress bars to the main layout
    pMainLayout->addWidget(m_pUploadProgressBar);   // Add upload progress bar
    pMainLayout->addWidget(m_pDownloadProgressBar); // Add download progress bar
    pMainLayout->addLayout(pLabelLayout);
    // Set the main layout once
    setLayout(pMainLayout);

    connect(m_pCreateDirPB,SIGNAL(clicked(bool)),this,SLOT(createDir()));
    connect(m_pFlushFilePB,SIGNAL(clicked(bool)),this,SLOT(flushFile()));
    connect(m_pDelDirPB,SIGNAL(clicked(bool)),this,SLOT(delDir()));
    connect(m_pRenamePB,SIGNAL(clicked(bool)),this,SLOT(renameFile()));
    connect(m_pBookListW,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(enterDir(QModelIndex)));
    connect(m_pReturnPB,SIGNAL(clicked(bool)),this,SLOT(returnPre()));
    connect(m_pUploadPB,SIGNAL(clicked(bool)),this,SLOT(uploadFile()));
    connect(m_pTimer,SIGNAL(timeout()),this,SLOT(uploadFileData()));
    connect(m_pDelFilePB,SIGNAL(clicked(bool)),this,SLOT(delRegFile()));
    connect(m_DownLoadPB,SIGNAL(clicked(bool)),this,SLOT(downloadFile()));
    connect(m_pShareFilePB,SIGNAL(clicked(bool)),this,SLOT(shareFile()));
    connect(m_pMoveFilePB,SIGNAL(clicked(bool)),this,SLOT(moveFile()));
    connect(m_pSelectDirPB,SIGNAL(clicked(bool)),this,SLOT(selectDestDir()));
    connect(m_pSearchButton, SIGNAL(clicked()), this, SLOT(searchFile()));
    connect(m_pSearchLineEdit, SIGNAL(returnPressed()), this, SLOT(searchFile()));
    connect(m_pSearchResultList, &QListWidget::itemClicked, this, &Book::handleSearchResultClick);
}

void Book::updateFileList(const PDU *pdu)
{
    if (pdu == nullptr)
        {
            return;
        }

        // Clear the file list
        m_pBookListW->clear();

        FileInfo *pFileInfo = nullptr;
        int iCount = pdu->uiMsgLen / sizeof(FileInfo);
        for (int i = 0; i < iCount; i++)
        {
            pFileInfo = (FileInfo *)(pdu->caMsg) + i;
            // 跳过特殊文件夹
            QString fileName = pFileInfo->caFileName;
            if (fileName == "." || fileName == ".." || fileName == "RecycleBin")
            {
                continue;
            }
            QListWidgetItem *pItem = new QListWidgetItem;
            if (0 == pFileInfo->iFileType)
            {
                pItem->setIcon(QIcon(QPixmap(":/icon/dir.png")));
            }
            else if (1 == pFileInfo->iFileType)
            {
                pItem->setIcon(QIcon(QPixmap(":/icon/reg.png")));
            }
            pItem->setText(pFileInfo->caFileName);
            m_pBookListW->addItem(pItem);
        }

        // Emit the signal indicating the file list has been updated
        emit fileListUpdated();
//    if(NULL == pdu)
//    {
//        return;
//    }
//    QListWidgetItem *pItemTemp = NULL;
//    int row = m_pBookListW->count();
//    while(m_pBookListW->count() > 0)
//    {
//        pItemTemp = m_pBookListW->item(row - 1);
//        m_pBookListW->removeItemWidget(pItemTemp);
//        delete pItemTemp;
//        row -= 1;
//    }
//    FileInfo *pFileInfo = NULL;
//    int iCount = pdu->uiMsgLen / sizeof(FileInfo);
//    for(int i = 0;i<iCount;i++)
//    {
//        pFileInfo = (FileInfo*)(pdu->caMsg) + i;

//        // 跳过特殊文件夹
//        QString fileName = pFileInfo->caFileName;
//        if (fileName == "." || fileName == ".." || fileName == "RecycleBin")
//        {
//            continue;
//        }

//        QListWidgetItem *pItem = new QListWidgetItem;
//        if(0 == pFileInfo->iFileType)
//        {
//            pItem->setIcon(QIcon(QPixmap(":/icon/dir.png")));
//        }
//        else if(1 == pFileInfo->iFileType)
//        {
//            pItem->setIcon(QIcon(QPixmap(":/icon/reg.png")));
//        }
//        pItem->setText(pFileInfo->caFileName);
//        m_pBookListW->addItem(pItem);
//    }
}

void Book::clearEnterDir()
{
    m_strEnterDir.clear();
}

QString Book::enterDir()
{
    return m_strEnterDir;
}

void Book::setDownloadStatus(bool status)
{
    m_bDownload = status;
}

bool Book::getDownloadStatus()
{
    return m_bDownload;
}

QString Book::getSaveFilePath()
{
    return m_strSaveFilePath;
}

QString Book::getShareFileName()
{
    return m_strShareFileName;
}

void Book::delRegFolder(const QString &folderPath)
{
    QDir folderDir(folderPath);
    if (folderDir.exists()) {
        QFileInfoList fileList = folderDir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

        for (const QFileInfo &fileInfo : fileList) {
            QString srcFilePath = fileInfo.absoluteFilePath();
            QString destFilePath = m_strDestDir + "/" + fileInfo.fileName();
            qDebug() << "Moving file/folder: " << srcFilePath << " to " << destFilePath;

            if (fileInfo.isDir()) {
                delRegFolder(srcFilePath);  // 递归处理子文件夹
            } else {
                QFile::rename(srcFilePath, destFilePath);
            }
        }
        folderDir.rmdir(folderPath);
    }
}

void Book::createDir()
{
    QString strNewDir = QInputDialog::getText(this,"Create folder","Folder name");
    if(!strNewDir.isEmpty())
    {
        if(strNewDir.size() > 32)
        {
            QMessageBox::warning(this,"Create folder","Folder name no longer than 32");
        }
        else
        {
            QString strName = TcpClient::getInstance().loginName();
            QString strCurPath = TcpClient::getInstance().curPath();
            PDU *pdu = mkPDU(strCurPath.size() + 1);
            pdu->uiMsgType = ENUM_MSG_TYPE_CREATE_DIR_REQUEST;
            strncpy(pdu->caData,strName.toStdString().c_str(),strName.size());
            strncpy(pdu->caData + 32,strNewDir.toStdString().c_str(),strNewDir.size());
            memcpy(pdu->caMsg,strCurPath.toStdString().c_str(),strCurPath.size());
            TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
            free(pdu);
            pdu = NULL;
        }
    }
    else
    {
        QMessageBox::warning(this,"Create folder","Folder name cannot be empty");
    }
}

void Book::flushFile()
{
    QString strCurPath = TcpClient::getInstance().curPath();
    PDU *pdu = mkPDU(strCurPath.size() + 1);
    pdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_FILE_REQUEST;
    strncpy((char*)(pdu->caMsg),strCurPath.toStdString().c_str(),strCurPath.size());
    TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
    free(pdu);
    pdu = NULL;
}

void Book::delDir()
{
    QString strCurPath = TcpClient::getInstance().curPath();
    QString userName = TcpClient::getInstance().loginName();
    QListWidgetItem *pItem = m_pBookListW->currentItem();
    QString strDelFolderName = pItem ? pItem->text() : "";
    if(NULL == pItem)
    {
        QMessageBox::warning(this,"Delete Folder","Please select the folder");
    }
    else
    {
        //VIP删除文件
        if (TcpClient().getInstance().getVIPStatus() == 1)
        {
            m_strMoveFolderPath = strCurPath + '/' + strDelFolderName;
            qDebug() << "Folder to move: " << m_strMoveFolderPath;
            m_strDestDir = "./" + userName + "/RecycleBin";

            QDir folderDir(m_strMoveFolderPath);
            if (folderDir.exists()) {
                QFileInfoList fileList = folderDir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
                for (const QFileInfo &fileInfo : fileList) {
                    QString srcFilePath = fileInfo.absoluteFilePath();
                    QString destFilePath = m_strDestDir + "/" + fileInfo.fileName();
                    qDebug() << "Moving file/folder: " << srcFilePath << " to " << destFilePath;
                    if (fileInfo.isDir()) {
                        delRegFolder(fileInfo.absoluteFilePath());
                    } else {
                        QFile::rename(srcFilePath, destFilePath);
                    }
                }
                QString destFolderPath = m_strDestDir + "/" + strDelFolderName;
                QFile::rename(m_strMoveFolderPath, destFolderPath);
            }
            int srcLen = m_strMoveFolderPath.size();
            int destLen = m_strDestDir.size();
            PDU *pdu = mkPDU(srcLen + destLen + 2);
            pdu->uiMsgType = ENUM_MSG_TYPE_VIP_DEL_DIR_REQUEST;
            sprintf(pdu->caData, "%d %d %s", srcLen, destLen, strDelFolderName.toStdString().c_str());
            memcpy(pdu->caMsg, m_strMoveFolderPath.toStdString().c_str(), srcLen);
            memcpy((char *)(pdu->caMsg) + srcLen + 1, m_strDestDir.toStdString().c_str(), destLen);
            TcpClient::getInstance().getTcpSocket().write((char *)pdu, pdu->uiPDULen);
            free(pdu);
            pdu = NULL;
        }
        else{
            QString strDelName = pItem->text();
            PDU *pdu = mkPDU(strCurPath.size() + 1);
            pdu->uiMsgType = ENUM_MSG_TYPE_DEL_DIR_REQUEST;
            strncpy(pdu->caData,strDelName.toStdString().c_str(),strDelName.size());
            memcpy(pdu->caMsg,strCurPath.toStdString().c_str(),strCurPath.size());
            TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
            free(pdu);
            pdu = NULL;
        }
    }
}

void Book::renameFile()
{
    QString strCurPath = TcpClient::getInstance().curPath();
    QListWidgetItem *pItem = m_pBookListW->currentItem();
    if(NULL == pItem)
    {
        QMessageBox::warning(this,"Rename Folder","select folder");
    }
    else
    {
        QString strOldName = pItem->text();
        QString strNewName = QInputDialog::getText(this,"Rename Folder","Input new name");
        if(!strNewName.isEmpty())
        {
            PDU *pdu = mkPDU(strCurPath.size() + 1);
            pdu->uiMsgType = ENUM_MSG_TYPE_RENAME_FILE_REQUEST;
            strncpy(pdu->caData,strOldName.toStdString().c_str(),strOldName.size());
            strncpy(pdu->caData + 32,strNewName.toStdString().c_str(),strNewName.size());
            memcpy(pdu->caMsg,strCurPath.toStdString().c_str(),strCurPath.size());
            TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
            free(pdu);
            pdu = NULL;
        }
        else
        {
            QMessageBox::warning(this,"Rename Folder","Name cannot be empty");
        }
    }
}

void Book::enterDir(const QModelIndex &index)
{
    QString strDirName = index.data().toString();
    m_strEnterDir = strDirName;
    QString strCurPath = TcpClient::getInstance().curPath();
    PDU *pdu = mkPDU(strCurPath.size() + 1);
    pdu->uiMsgType = ENUM_MSG_TYPE_ENTER_DIR_REQUEST;
    strncpy(pdu->caData,strDirName.toStdString().c_str(),strDirName.size());
    memcpy(pdu->caMsg,strCurPath.toStdString().c_str(),strCurPath.size());
    TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
    free(pdu);
    pdu = NULL;
}

void Book::returnPre()
{
    QString strCurPath = TcpClient::getInstance().curPath();
    QString strRootPath = "./" + TcpClient::getInstance().loginName();
    if(strCurPath == strRootPath)
    {
       QMessageBox::warning(this,"Back","Return failed: already in the primary folder");
    }
    else
    {
        int index = strCurPath.lastIndexOf('/');
        strCurPath.remove(index,strCurPath.size() - index);
        TcpClient::getInstance().setCurPath(strCurPath);
        clearEnterDir();
        flushFile();
    }
}

void Book::uploadFile()
{
    m_strUploadFilePath = QFileDialog::getOpenFileName();
    if(!m_strUploadFilePath.isEmpty())
    {
        int index = m_strUploadFilePath.lastIndexOf('/');
        QString strFileName = m_strUploadFilePath.right(m_strUploadFilePath.size() - index - 1);
        QFile file(m_strUploadFilePath);
        qint64 fileSize = file.size();
        QString strCurPath = TcpClient::getInstance().curPath();
        PDU *pdu = mkPDU(strCurPath.size() + 1);
        pdu->uiMsgType = ENUM_MSG_TYPE_UPLOAD_FILE_REQUEST;
        memcpy(pdu->caMsg,strCurPath.toStdString().c_str(),strCurPath.size());
        sprintf(pdu->caData,"%s %lld",strFileName.toStdString().c_str(),(fileSize + 32 - (fileSize % 16))); //由于加密
        TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu = NULL;
        m_pTimer->start(1000);
    }
    else
    {
        QMessageBox::warning(this,"Upload File","File name cannot be empty");
    }
}

void Book::uploadFileData()
{
    m_pTimer->stop();
    QFile file(m_strUploadFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Upload File", "Open file failed");
        return;
    }

    // Get total file size
    qint64 totalSize = file.size();
    qint64 uploadedSize = 0; // Track how much data has been uploaded

    // Show and reset progress bar
    m_pUploadProgressBar->setVisible(true);
    m_pUploadProgressBar->setValue(0);

    // Read and encrypt the file
    QByteArray fileData = file.readAll();
    file.close();

    // Encrypt the file data
    std::string strFileData(fileData.constData(), fileData.size());
    std::vector<unsigned char> encryptedData = AESCBC::aesEncrypt(strFileData);

    qDebug() << "Encrypted total size to send to server:" << encryptedData.size();

    // Upload encrypted data in chunks
    const int chunkSize = 4096; // Each chunk size
    size_t totalEncryptedSize = encryptedData.size();
    size_t offset = 0;

    while (offset < totalEncryptedSize) {
        // Determine the size of the current chunk
        size_t currentChunkSize = (chunkSize < totalEncryptedSize - offset) ? chunkSize : (totalEncryptedSize - offset);

        // Get the pointer to the current chunk data
        const unsigned char *chunkData = encryptedData.data() + offset;

        // Send the current chunk
        TcpClient::getInstance().getTcpSocket().write(
            reinterpret_cast<const char *>(chunkData), currentChunkSize);

        // Update offset and uploaded size
        offset += currentChunkSize;
        uploadedSize += currentChunkSize;

        // Update progress bar
        int progress = static_cast<int>((uploadedSize * 100) / totalEncryptedSize);
        m_pUploadProgressBar->setValue(progress);
    }

    // Hide progress bar when upload is complete
    m_pUploadProgressBar->setVisible(false);
}
//void Book::uploadFileData()
//{
//    m_pTimer->stop();
//    QFile file(m_strUploadFilePath);
//    if(!file.open(QIODevice::ReadOnly))
//    {
//        QMessageBox::warning(this,"Upload File","open file failed");
//        return;
//    }

//    // refine 加密------------------------------------------------------------------------
//    QByteArray fileData = file.readAll();
//    file.close();
//    // Step : 加密整个文件内容
//    std::string strFileData(fileData.constData(), fileData.size());
//    std::vector<unsigned char> encryptedData = AESCBC::aesEncrypt(strFileData);

//    qDebug() << "加密后的总大小且需发送到服务端:" << encryptedData.size();

//    // Step 3: 分块上传
//    const int chunkSize = 4096; // 每次传输的块大小
//    size_t totalSize = encryptedData.size();
//    size_t offset = 0;

//    while (offset < totalSize) {
//        // 获取当前块的大小
//        size_t currentChunkSize = (chunkSize < totalSize - offset) ? chunkSize : (totalSize - offset);

//        // 获取当前块的数据指针
//        const unsigned char *chunkData = encryptedData.data() + offset;

//        // 发送当前块
//        TcpClient::getInstance().getTcpSocket().write(
//            reinterpret_cast<const char *>(chunkData), currentChunkSize);

////        qDebug() << "发送了块大小:" << currentChunkSize;

//        offset += currentChunkSize; // 更新偏移量
//    }
//    // 加密-----------------------------------------------------------------------------
//}

void Book::delRegFile()
{
    //删除的文件所在的路径
    QString strCurPath = TcpClient::getInstance().curPath();
    QString userName = TcpClient::getInstance().loginName();
    QListWidgetItem *pItem = m_pBookListW->currentItem();
    QString strDelName = pItem->text();
    if(NULL == pItem)
    {
        QMessageBox::warning(this,"Delete File","Select File");
    }
    else
    {
//VIP删除文件
        if (TcpClient().getInstance().getVIPStatus() == 1)
        {
            m_strMoveFilePath = strCurPath + '/' + strDelName;
            qDebug() << m_strMoveFilePath;
            m_strDestDir = "./" + userName + "/RecycleBin";
            qDebug()<< m_strDestDir;
            int srcLen = m_strMoveFilePath.size();
            int destLen = m_strDestDir.size();
            PDU *pdu = mkPDU(srcLen + destLen + 2);
            pdu->uiMsgType = ENUM_MSG_TYPE_MOVE_FILE_REQUEST;
            sprintf(pdu->caData,"%d %d %s",srcLen,destLen,strDelName.toStdString().c_str());
            memcpy(pdu->caMsg,m_strMoveFilePath.toStdString().c_str(),srcLen);
            memcpy((char*)(pdu->caMsg) + srcLen + 1,m_strDestDir.toStdString().c_str(),destLen);
            TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
            free(pdu);
            pdu = NULL;
        }
//普通人删除文件
        else
        {
            PDU *pdu = mkPDU(strCurPath.size() + 1);
            pdu->uiMsgType = ENUM_MSG_TYPE_DEL_FILE_REQUEST;
            strncpy(pdu->caData,strDelName.toStdString().c_str(),strDelName.size());
            memcpy(pdu->caMsg,strCurPath.toStdString().c_str(),strCurPath.size());
            TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
            free(pdu);
            pdu = NULL;
        }
    }
}

void Book::downloadFile()
{
    QListWidgetItem *pItem = m_pBookListW->currentItem();
    if(NULL == pItem)
    {
        QMessageBox::warning(this,"Download","Select the file");
    }
    else
    {
        QString strSaveFilePath = QFileDialog::getSaveFileName();
        if(strSaveFilePath.isEmpty())
        {
            QMessageBox::warning(this,"Download","Select path");
            m_strSaveFilePath.clear();
        }
        else
        {
            m_strSaveFilePath = strSaveFilePath;
        }
        QString strCurPath = TcpClient::getInstance().curPath();
        PDU *pdu = mkPDU(strCurPath.size() + 1);
        pdu->uiMsgType = ENUM_MSG_TYPE_DOWNLOAD_FILE_REQUEST;
        QString strFileName = pItem->text();
        strcpy(pdu->caData,strFileName.toStdString().c_str());
        memcpy(pdu->caMsg,strCurPath.toStdString().c_str(),strCurPath.size());

        // Reset and show progress bar
        m_pDownloadProgressBar->setVisible(true);
        m_pDownloadProgressBar->setValue(0);

        TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
    }

//    QListWidgetItem *pItem = m_pBookListW->currentItem();
//    if(NULL == pItem)
//    {
//        QMessageBox::warning(this,"Download","Select the file");
//    }
//    else
//    {
//        QString strSaveFilePath = QFileDialog::getSaveFileName();
//        if(strSaveFilePath.isEmpty())
//        {
//            QMessageBox::warning(this,"Download","Select path");
//            m_strSaveFilePath.clear();
//        }
//        else
//        {
//            m_strSaveFilePath = strSaveFilePath;
//        }
//        QString strCurPath = TcpClient::getInstance().curPath();
//        PDU *pdu = mkPDU(strCurPath.size() + 1);
//        pdu->uiMsgType = ENUM_MSG_TYPE_DOWNLOAD_FILE_REQUEST;
//        QString strFileName = pItem->text();
//        strcpy(pdu->caData,strFileName.toStdString().c_str());
//        memcpy(pdu->caMsg,strCurPath.toStdString().c_str(),strCurPath.size());
//        TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
//    }
}

void Book::shareFile()
{
    QListWidgetItem *pItem = m_pBookListW->currentItem();
    if(NULL == pItem)
    {
        QMessageBox::warning(this,"Share File","Select file");
        return;
    }
    else
    {
        m_strShareFileName = pItem->text();
    }
    Friend *pFriend = OpeWidget::getInstance().getFriend();
    QListWidget *pFriendList = pFriend->getFriendList();
    ShareFile::getInstance().updateFriend(pFriendList);
    if(ShareFile::getInstance().isHidden())
    {
        ShareFile::getInstance().show();
    }
}

void Book::moveFile()
{
    QListWidgetItem *pCurItem = m_pBookListW->currentItem();
    if(NULL != pCurItem)
    {
        m_strMoveFileName = pCurItem->text();
        QString strCurPath = TcpClient::getInstance().curPath();
        m_strMoveFilePath = strCurPath + '/' + m_strMoveFileName;
        m_pSelectDirPB->setEnabled(true);
    }
    else
    {
        QMessageBox::warning(this,"Move File","Select file");
    }
}

void Book::selectDestDir()
{
    QListWidgetItem *pCurItem = m_pBookListW->currentItem();
    if(NULL != pCurItem)
    {
        QString strDestDir = pCurItem->text();
        QString strCurPath = TcpClient::getInstance().curPath();
        m_strDestDir = strCurPath + '/' +strDestDir;
        int srcLen = m_strMoveFilePath.size();
        int destLen = m_strDestDir.size();
        PDU *pdu = mkPDU(srcLen + destLen + 2);
        pdu->uiMsgType = ENUM_MSG_TYPE_MOVE_FILE_REQUEST;
        sprintf(pdu->caData,"%d %d %s",srcLen,destLen,m_strMoveFileName.toStdString().c_str());
        memcpy(pdu->caMsg,m_strMoveFilePath.toStdString().c_str(),srcLen);
        memcpy((char*)(pdu->caMsg) + srcLen + 1,m_strDestDir.toStdString().c_str(),destLen);
        TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu = NULL;
    }
    else
    {
        QMessageBox::warning(this,"Move file","Select file");
    }
    m_pSelectDirPB->setEnabled(false);
}

void Book::searchFile()
{
    QString keyword = m_pSearchLineEdit->text().trimmed();
    if (keyword.isEmpty())
    {
        QMessageBox::warning(this, "Search Files", "Please enter a keyword to search.");
        return;
    }

    QString strName = TcpClient::getInstance().loginName();
    QString strCurPath = TcpClient::getInstance().curPath();

    // Create a PDU to send the search request
    PDU *pdu = mkPDU(strCurPath.size() + 1);
    pdu->uiMsgType = ENUM_MSG_TYPE_SEARCH_FILE_REQUEST;
    strncpy(pdu->caData, strName.toStdString().c_str(), strName.size());
    strncpy(pdu->caData + 32, keyword.toStdString().c_str(), keyword.size());
    memcpy(pdu->caMsg, strCurPath.toStdString().c_str(), strCurPath.size());

    // Send the search request to the server
    TcpClient::getInstance().getTcpSocket().write((char*)pdu, pdu->uiPDULen);

    free(pdu);
    pdu = NULL;
}

void Book::handleSearchFileRespond(const PDU *pdu)
{
    if (pdu == nullptr || pdu->uiMsgLen == 0)
    {
        QMessageBox::information(this, "Search Files", "No matching files or folders found.");
        return;
    }

    // Show the search results list and clear its contents
    m_pSearchResultList->clear();
    m_pSearchResultList->show();

    // Extract matching file/folder paths from the PDU
    QString resultData = QString::fromUtf8((char *)pdu->caMsg);
    QStringList results = resultData.split(';', QString::SkipEmptyParts);

    for (const QString &absPath : results)
    {
        if (!absPath.isEmpty())
        {
            // Extract the file or folder name from the absolute path
            QFileInfo fileInfo(absPath);
            QString fileName = fileInfo.fileName();

            // Create a new list item with the file/folder name
            QListWidgetItem *item = new QListWidgetItem(fileName);
            m_pSearchResultList->addItem(item);

            // Store the absolute path in item data for later use
            item->setData(Qt::UserRole, absPath);
        }
    }
}

void Book::handleSearchResultClick(QListWidgetItem *item)
{
    if (!item)
            return;

        // Retrieve the absolute path from item data
        QString absPath = item->data(Qt::UserRole).toString();

        // Extract the directory and file/folder name
        int index = absPath.lastIndexOf('/');
        QString dirPath = absPath.left(index);
        QString fileName = absPath.mid(index + 1);

        // Ensure the directory path starts with "./username"
        QString userName = TcpClient::getInstance().loginName();
        QString rootPath = "./" + userName;

        // If dirPath doesn't start with the rootPath, adjust it
        if (!dirPath.startsWith(rootPath)) {
            // Attempt to convert the absolute path to a relative path that starts from "./username".
            int pos = dirPath.indexOf(userName);
            if (pos != -1) {
                // Extract the portion from "username" onwards and add "./"
                QString relativePath = "./" + dirPath.mid(pos);
                dirPath = relativePath;
            }
        }

        // Set the normalized current directory
        TcpClient::getInstance().setCurPath(dirPath);
        flushFile();

        // Connect to the fileListUpdated signal
        connect(this, &Book::fileListUpdated, [this, fileName]() {
            // Highlight the file or folder
            for (int i = 0; i < m_pBookListW->count(); ++i)
            {
                QListWidgetItem *listItem = m_pBookListW->item(i);
                if (listItem->text() == fileName)
                {
                    m_pBookListW->setCurrentItem(listItem);
                    break;
                }
            }
            // Disconnect this lambda to prevent it from running multiple times
            disconnect(this, &Book::fileListUpdated, nullptr, nullptr);
        });

        // Hide the search results after navigation
        m_pSearchResultList->hide();
}


