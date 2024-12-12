#ifndef OPEDB_H
#define OPEDB_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStringList>
#include <QThread>

class OpeDB : public QObject
{
    Q_OBJECT
public:
    explicit OpeDB(QObject *parent = nullptr);
    static OpeDB& getInstance();
    void init();
    ~OpeDB();
    // refine 多线程---------------------
    QSqlDatabase getDatabase();              // 获取当前线程的数据库连接
    //----------------------------------
    //群聊
    QStringList handleGetOnlineGroupMember(const char* groupname);            //处理查询在线群聊成员请求
    void handleAddGroup(int groupID,const char *name);
    int handleGroupID();
    //----------------------------------
    bool handleRegist(const char *name,const char *pwd);
    bool handleLogin(const char *name, const char *pwd);
    void handleOffline(const char *name);
    QStringList handleAllOnline();
    int handleSearchUsr(const char *name);
    int handleAddFriend(const char *pername,const char *name);
    void handleAgreeAddFriend(const char *pername, const char *name);
    QStringList handleFlushFriend(const char *name);
    bool handleDelFriend(const char *name, const char *friendName);
    bool handleCancelUsr(const char *name,const char *pwd);
    int handleChangePWD(const char *name, const char *pwd, const char *newname);
    bool handleAddVIP(const char *name);
    int handleGetVIP(const char *name);

    QStringList handleGetNonMemberFriend(const char* pername,const char* groupname); //处理查询非群成员的在线好友请求
    void handleAddGroup(const char* groupname,const char *name);

    void handleChangeGroupName(const char* oldname,const char* newname); //处理群改名请求

    void handleAddToRecycleBin(const char *username, const char *filePath); //处理VIP删除文件

    QSqlDatabase getMainDb();

signals:

public slots:

private:
    QSqlDatabase m_db; // 连接主线程数据库 （暂时未用）
    QList<QSqlDatabase> thread_dbs; //储存线程对应数据库（暂时未用）

};

#endif // OPEDB_H
