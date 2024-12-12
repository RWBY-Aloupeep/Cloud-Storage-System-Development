#ifndef BOOK_H
#define BOOK_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "protocol.h"
#include <QTimer>
#include <QProgressBar>
#include <QLabel>

class Book : public QWidget
{
    Q_OBJECT
public:
    explicit Book(QWidget *parent = nullptr);
    void updateFileList(const PDU *pdu);
    void clearEnterDir();
    QString enterDir();
    void setDownloadStatus(bool status);
    qint64 m_iTotal;    // 总的文件大小
    qint64 m_iRecved;   // 已收到多少
    bool getDownloadStatus();
    QString getSaveFilePath();
    QString getShareFileName();
    // VIP递归删除文件夹的函数
    void delRegFolder(const QString& folderPath);
    QProgressBar *m_pUploadProgressBar;   // For tracking upload progress
    QProgressBar *m_pDownloadProgressBar; // For tracking download progress
    QLabel *m_pPercentageLabel;   // To display the percentage
    QLabel *m_pTimeRemainingLabel; // To display the predicted finish time

signals:
    void fileListUpdated();

public slots:
    void createDir();
    void flushFile();
    void delDir();
    void renameFile();
    void enterDir(const QModelIndex &index);
    void returnPre();
    void uploadFile();
    void uploadFileData();
    void delRegFile();
    void downloadFile();
    void shareFile();
    void moveFile();
    void selectDestDir();
    void searchFile();
    void handleSearchFileRespond(const PDU *pdu);
    void handleSearchResultClick(QListWidgetItem *item);


private:
    QListWidget *m_pBookListW;
    QPushButton *m_pReturnPB;
    QPushButton *m_pCreateDirPB;
    QPushButton *m_pDelDirPB;
    QPushButton *m_pRenamePB;
    QPushButton *m_pFlushFilePB;
    QPushButton *m_pUploadPB;
    QPushButton *m_DownLoadPB;
    QPushButton *m_pDelFilePB;
    QPushButton *m_pShareFilePB;
    QPushButton *m_pMoveFilePB;
    QPushButton *m_pSelectDirPB;

    QString m_strEnterDir;
    QString m_strUploadFilePath;
    QTimer *m_pTimer;
    QString m_strSaveFilePath;
    bool m_bDownload;
    QString m_strShareFileName;
    QString m_strMoveFileName;
    QString m_strMoveFilePath;
    QString m_strDestDir;
    //VIP删除文件夹
    QString m_strMoveFolderPath;
    QLineEdit *m_pSearchLineEdit;
    QPushButton *m_pSearchButton;

    QListWidget *m_pSearchResultList;
};

#endif // BOOK_H
