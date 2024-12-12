#ifndef VIP_H
#define VIP_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "protocol.h"
#include <QDir>
#include <QMessageBox>
#include <QTimer>


class VIP : public QWidget
{
    Q_OBJECT
public:
    explicit VIP(QWidget *parent = nullptr);
    void updateBinList(const PDU *pdu);
    void clearEnterBin();
    QString enterBin();

signals:

public slots:
    void on_clearBin_pb_clicked();
    void on_recoverfile_pb_clicked();
    void flushBin();
    void enterBin(const QModelIndex &index);
    void on_zipfile_pb_clicked();
    void on_unzipfile_pb_clicked();


private:
    QListWidget *m_pBinListWidget;
    QPushButton *m_pFlushBin;
    QPushButton *m_pClearBin;
    QPushButton *m_pRecoverFile;
    QString m_strEnterBin;
    QPushButton *m_pzipFile;
    QPushButton *m_pUnzipFile;
};

#endif // VIP_H
