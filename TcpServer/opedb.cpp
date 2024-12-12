#include "opedb.h"
#include <QMessageBox>
#include <QDebug>

OpeDB::OpeDB(QObject *parent) : QObject(parent)
{
//    m_db = QSqlDatabase::addDatabase("QSQLITE");
}

// 生成实例
OpeDB &OpeDB::getInstance()
{
    static OpeDB instance;
    return instance;
}

// 数据库连接初始化
void OpeDB::init()
{
//    m_db.setHostName("localhost");
   // 不同的数据库存放的位置，要更改为对应的位置
//    m_db.setDatabaseName("C:\\Users\\Lenovo\\Documents\\TcpServer\\cloud.db");
//    if(m_db.open())
//    {
//        QSqlQuery query;
//        query.exec("select * from usrInfo");
//        while(query.next())
//        {
//            QString data = QString("%1,%2,%3").arg(query.value(0).toString()).arg(query.value(1).toString()).arg(query.value(2).toString());
//            qDebug() << data;
//        }
//    }
//    else
//    {
//        QMessageBox::critical(NULL,"打开数据库","打开数据库失败");
//    }

       m_db = getDatabase(); // 创建或获取主线程的数据库连接
       if (!m_db.isOpen()) {
           QMessageBox::critical(nullptr, "Open Database", "failed");
       } else {
           QSqlQuery query(m_db);
           if (query.exec("SELECT * FROM usrInfo")) {
               while (query.next()) {
                   QString data = QString("%1,%2,%3")
                                      .arg(query.value(0).toString())
                                      .arg(query.value(1).toString())
                                      .arg(query.value(2).toString());
                   qDebug() << data;
               }
           }
       }
}

QSqlDatabase OpeDB::getDatabase() {
    QString connectionName = QString::number((quintptr)QThread::currentThreadId());
    qDebug()<<"connection name"<<connectionName;
    if (!QSqlDatabase::contains(connectionName)) {
        // 创建当前线程的数据库连接
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        db.setHostName("localhost");
        db.setDatabaseName("C:\\Users\\Lenovo\\Documents\\TcpServer\\cloud.db");
        if (!db.open()) {
            qDebug() << "Failed to open database in thread:" << QThread::currentThreadId();
        }
    }
    qDebug()<<"add database:ok";
    QSqlDatabase thread_db= QSqlDatabase::database(connectionName);
    thread_dbs.push_back(thread_db);
    return thread_db;
}

QStringList OpeDB::handleGetOnlineGroupMember(const char *groupname)
{
    QString dataStr = QString("select name from usrInfo where online = 1 and id in ( select member_id from groupTbl where group_name = \'%1\' )").arg(groupname);
    QSqlQuery qs(QSqlDatabase::database(QString::number((quintptr)QThread::currentThreadId())));
    qs.exec(dataStr);
    QStringList result;
    result.clear();

    while(qs.next())
    {

        result.append(qs.value("name").toString());
    }
    return result;
}

// 析构函数用来关闭数据库连接
OpeDB::~OpeDB()
{
//    m_db.close();
    // 清理线程本地的数据库连接
    QString connectionName = QString::number((quintptr)QThread::currentThreadId());
    if (QSqlDatabase::contains(connectionName)) {
        QSqlDatabase::removeDatabase(connectionName);
    }
}

// 处理客户端的用户注册信息
bool OpeDB::handleRegist(const char *name, const char *pwd)
{
    if(NULL == name || NULL == pwd)
    {
        return false;
    }
    QString data = QString("insert into usrInfo(name,pwd) values(\'%1\',\'%2\')").arg(name).arg(pwd);
    QSqlQuery query(QSqlDatabase::database(QString::number((quintptr)QThread::currentThreadId())));
    return query.exec(data);
}

// 处理客户端的用户登录信息
bool OpeDB::handleLogin(const char *name, const char *pwd)
{
    if(NULL == name || NULL == pwd)
    {
        return false;
    }
    QString data = QString("select * from usrInfo where name = \'%1\' and pwd = \'%2\' and online = 0").arg(name).arg(pwd);
    QSqlQuery query(QSqlDatabase::database(QString::number((quintptr)QThread::currentThreadId())));
    query.exec(data);
    if(query.next())
    {
        data = QString("update usrInfo set online = 1 where name = \'%1\' and pwd = \'%2\'").arg(name).arg(pwd);
        QSqlQuery query(QSqlDatabase::database(QString::number((quintptr)QThread::currentThreadId())));
        query.exec(data);
        return true;
    }
    else
    {
        return false;
    }
}

// 处理客户端的用户退出信息
void OpeDB::handleOffline(const char *name)
{
    if(NULL == name)
    {
        return;
    }
    QString data = QString("update usrInfo set online = 0 where name = \'%1\'").arg(name);
    QSqlQuery query(QSqlDatabase::database(QString::number((quintptr)QThread::currentThreadId())));
    query.exec(data);
}

// 查询所有在线用户的用户名列表
QStringList OpeDB::handleAllOnline()
{
    QString data = QString("select name from usrInfo where online = 1");
    QSqlQuery query(QSqlDatabase::database(QString::number((quintptr)QThread::currentThreadId())));
    query.exec(data);
    QStringList result;
    result.clear();
    while(query.next())
    {
        result.append(query.value(0).toString());
    }
    return result;
}

// 搜索用户
int OpeDB::handleSearchUsr(const char *name)
{
    if(NULL == name)
    {
        return -1;
    }
    QString data = QString("select online from usrInfo where name = \'%1\'").arg(name);
    QSqlQuery query(QSqlDatabase::database(QString::number((quintptr)QThread::currentThreadId())));
    query.exec(data);
    if(query.next())
    {
        int res = query.value(0).toInt();
        if(1 == res)
        {
            return 1;
        }
        else if(0 == res)
        {
            return 0;
        }
    }
    else
    {
        return -1;
    }
}

// 查看当前用户和待添加好友之间的关系
int OpeDB::handleAddFriend(const char *pername, const char *name)
{
    if(NULL == pername || NULL == name)
    {
        return -1;
    }
    QString data = QString("select * from friend where (id = (select id from usrInfo where name=\'%1\') and friendId = (select id from usrInfo where name=\'%2\')) or (id = (select id from usrInfo where name=\'%3\') and friendId = (select id from usrInfo where name=\'%4\'))").arg(pername).arg(name).arg(name).arg(pername);
    qDebug() << data;
    QSqlQuery query(QSqlDatabase::database(QString::number((quintptr)QThread::currentThreadId())));
    query.exec(data);
    if(query.next())
    {
        return 0;
    }
    else
    {
        QString data = QString("select online from usrInfo where name = \'%1\'").arg(pername);
        QSqlQuery query(QSqlDatabase::database(QString::number((quintptr)QThread::currentThreadId())));
        query.exec(data);
        if(query.next())
        {
            int res = query.value(0).toInt();
            if(1 == res)
            {
                return 1;
            }
            else if(0 == res)
            {
                return 2;
            }
        }
        else
        {
            return 3;
        }
    }
}

// 在同意添加好友之后向数据库中写入相关数据
void OpeDB::handleAgreeAddFriend(const char *pername, const char *name)
{
    if (NULL == pername || NULL == name)
    {
        return;
    }
    QString data = QString("insert into friend(id, friendId) values((select id from usrInfo where name=\'%1\'), (select id from usrInfo where name=\'%2\'))").arg(pername).arg(name);
    QSqlQuery query(QSqlDatabase::database(QString::number((quintptr)QThread::currentThreadId())));
    query.exec(data);
}

// 获取当前用户的好友列表
//QStringList OpeDB::handleFlushFriend(const char *name)
//{
//    QStringList strFriendList;
//    strFriendList.clear();
//    if(NULL == name)
//    {
//        return strFriendList;
//    }
//    QString data = QString("select name from usrInfo where online=1 and id in (select id from friend where friendId=(select id from usrInfo where name=\'%1\'))").arg(name);
//    QSqlQuery query(QSqlDatabase::database(QString::number((quintptr)QThread::currentThreadId())));
//    query.exec(data);
//    while(query.next())
//    {
//        strFriendList.append(query.value(0).toString());
//        qDebug() << query.value(0).toString();
//    }
//    data = QString("select name from usrInfo where online=1 and id in (select friendId from friend where id=(select id from usrInfo where name=\'%1\'))").arg(name);
//    query.exec(data);
//    while(query.next())
//    {
//        strFriendList.append(query.value(0).toString());
//        qDebug() << query.value(0).toString();
//    }
//    return strFriendList;
//}

// 数据库删除好友操作
//bool OpeDB::handleDelFriend(const char *name, const char *friendName)
//{
//    if(NULL == name || NULL == friendName)
//    {
//        return false;
//    }
//    QString data = QString("delete from friend where id = (select id from usrInfo where name = \'%1\') and friendId = (select id from usrInfo where name = \'%2\')").arg(name).arg(friendName);
//    QSqlQuery query(QSqlDatabase::database(QString::number((quintptr)QThread::currentThreadId())));
//    query.exec(data);
//    data = QString("delete from friend where id = (select id from usrInfo where name = \'%1\') and friendId = (select id from usrInfo where name = \'%2\')").arg(friendName).arg(name);
//    query.exec(data);
//    return true;
//}

 //获取当前用户的好友 以及群聊列表
QStringList OpeDB::handleFlushFriend(const char *name)
{
    QStringList strFriendList;
        strFriendList.clear();
        if(NULL == name)
        {
            return strFriendList;
        }
        QString data = QString("select name from usrInfo where online=1 and id in (select id from friend where friendId=(select id from usrInfo where name=\'%1\'))").arg(name);
        QSqlQuery query(QSqlDatabase::database(QString::number((quintptr)QThread::currentThreadId())));
        query.exec(data);
        while(query.next())
        {
            strFriendList.append(query.value(0).toString());
            qDebug() << query.value(0).toString();
        }
        data = QString("select name from usrInfo where online=1 and id in (select friendId from friend where id=(select id from usrInfo where name=\'%1\'))").arg(name);
        query.exec(data);
        while(query.next())
        {
            strFriendList.append(query.value(0).toString());
            qDebug() << query.value(0).toString();
        }
        //查询该用户所在的群聊
        data = QString("select group_name from groupTbl where member_id = (select id from usrInfo where name=\'%1\')").arg(name);
        query.exec(data);
        while(query.next())
        {
            strFriendList.append("group:"+query.value(0).toString());
            qDebug() << query.value(0).toString();
        }
        return strFriendList;

//    QStringList strFriendList;
//        strFriendList.clear();

//        if (NULL == name)
//        {
//            return strFriendList;
//        }

//        // 查询当前用户的好友
//        QString data = QString("select name from usrInfo where online=1 and id in (select id from friend where friendId=(select id from usrInfo where name=\'%1\'))").arg(name);
//        QSqlQuery query(QSqlDatabase::database(QString::number((quintptr)QThread::currentThreadId())));
//        query.exec(data);

//        while (query.next())
//        {
//            strFriendList.append("friend_" + query.value(0).toString());
//            qDebug() << "Friend: " << query.value(0).toString();
//        }

//        data = QString("select name from usrInfo where online=1 and id in (select friendId from friend where id=(select id from usrInfo where name=\'%1\'))").arg(name);
//        query.exec(data);

//        while (query.next())
//        {
//            strFriendList.append("friend_" + query.value(0).toString());
//            qDebug() << "Friend: " << query.value(0).toString();
//        }
//        data = QString("select group_name from groupTbl where member_id = (select id from usrInfo where name=\'%1\')").arg(name);
//        query.exec(data);

//        while (query.next())
//        {
//            strFriendList.append("group_" + query.value(0).toString());
//            qDebug() << "Group: " << query.value(0).toString();
//        }

//        return strFriendList;

//    QStringList strFriendList;
//    strFriendList.clear();
//    if(NULL == name)
//    {
//        return strFriendList;
//    }
//    QString data = QString("select name from usrInfo where online=1 and id in (select id from friend where friendId=(select id from usrInfo where name=\'%1\'))").arg(name);
//    QSqlQuery query(QSqlDatabase::database(QString::number((quintptr)QThread::currentThreadId())));
//    query.exec(data);
//    while(query.next())
//    {
//        strFriendList.append(query.value(0).toString());
//        qDebug() << query.value(0).toString();
//    }
//    data = QString("select name from usrInfo where online=1 and id in (select friendId from friend where id=(select id from usrInfo where name=\'%1\'))").arg(name);
//    query.exec(data);
//    while(query.next())
//    {
//        strFriendList.append(query.value(0).toString());
//        qDebug() << query.value(0).toString();
//    }
//    //查询该用户所在的群聊
//    data = QString("select group_name from groupTbl where member_id = (select id from usrInfo where name=\'%1\')").arg(name);
//    query.exec(data);
//    while(query.next())
//    {
//        strFriendList.append(query.value(0).toString());
//        qDebug() << query.value(0).toString();
//    }
//    return strFriendList;
}

// 数据库删除好友/退出群聊操作
bool OpeDB::handleDelFriend(const char *name, const char *friendName)
{
    if(NULL == name || NULL == friendName)
    {
        return false;
    }
    QString data = QString("delete from friend where id = (select id from usrInfo where name = \'%1\') and friendId = (select id from usrInfo where name = \'%2\')").arg(name).arg(friendName);
    QSqlQuery query(QSqlDatabase::database(QString::number((quintptr)QThread::currentThreadId())));
    query.exec(data);
    data = QString("delete from friend where id = (select id from usrInfo where name = \'%1\') and friendId = (select id from usrInfo where name = \'%2\')").arg(friendName).arg(name);
    query.exec(data);
    //退出群聊
    data = QString("delete from groupTbl where group_name = \'%1\' and member_id = (select id from usrInfo where name = \'%2\')").arg(friendName).arg(name);
    query.exec(data);
    return true;
}

//注销用户
bool OpeDB::handleCancelUsr(const char *name, const char *pwd)
{
    if (NULL == name || NULL == pwd)
    {
        return false;
    }

    // 检查用户名和密码是否匹配
    QSqlQuery query(QSqlDatabase::database(QString::number((quintptr)QThread::currentThreadId())));
    QString checkUserQuery = QString("SELECT COUNT(*), online FROM usrInfo WHERE name = \'%1\' AND pwd = \'%2\'").arg(name).arg(pwd);
    query.exec(checkUserQuery);
    if (query.next())
    {
        int count = query.value(0).toInt();
        int onlineStatus = query.value(1).toInt();

        // 如果用户在线，无法注销
        if (onlineStatus == 1)
        {
            return false;
        }

        // 如果用户不存在
        if (count == 0)
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    QSqlQuery deleteQuery(QSqlDatabase::database(QString::number((quintptr)QThread::currentThreadId())));
    QString deleteUserQuery = QString("DELETE FROM usrInfo WHERE name = '%1'").arg(name);
    if (!deleteQuery.exec(deleteUserQuery))
    {
        return false;
    }

    QString deleteFriendQuery = QString("DELETE FROM friend WHERE id = (SELECT id FROM usrInfo WHERE name = '%1') OR friendId = (SELECT id FROM usrInfo WHERE name = '%2')").arg(name).arg(name);
    if (!deleteQuery.exec(deleteFriendQuery))
    {
        return false;
    }
    QString deleteGroupQuery = QString("DELETE FROM groupTbl WHERE member_id = (SELECT id FROM usrInfo WHERE name = '%1')").arg(name);
    if (!deleteQuery.exec(deleteGroupQuery))
    {
        return false;
    }

    return true;
}

int OpeDB::handleChangePWD(const char *name, const char *pwd, const char *newPwd)
{
//    返回 0：密码更新成功。
//    返回 1：用户名不存在。
//    返回 2：旧密码不匹配。
//    返回 3：密码更新失败（数据库操作失败）

    // 构造 SQL 查询，检查用户是否存在并且旧密码是否正确
    QString usr_sql = QString("select * from usrInfo where name = \'%1\'").arg(name);
    QSqlQuery query(QSqlDatabase::database(QString::number((quintptr)QThread::currentThreadId())));

    // 执行查询
    if (!query.exec(usr_sql)) {
        return 3;  // 查询失败，返回密码更新失败
    }
    if (!query.next()) {
        return 1;  // 用户不存在，返回错误码 1
    } else {
        // 用户存在，获取密码字段
        QString pwdFromDb = query.value("pwd").toString();  // 获取 'pwd' 字段的值
        qDebug() << "Password from database:" << pwdFromDb;

        if (pwd == pwdFromDb) {
            qDebug() << "Old password is correct, updating password...";

            QString update_sql = QString("update usrInfo set pwd = \'%1\' where name = \'%2\'").arg(newPwd).arg(name);

            QSqlQuery updateQuery(QSqlDatabase::database(QString::number((quintptr)QThread::currentThreadId())));
            bool success = updateQuery.exec(update_sql);

            if (success) {
                qDebug() << "Password updated successfully for user:" << name;
                return 0;
            } else {
                qDebug() << "Failed to update password for user:" << name;
                return 3;
            }
        } else {
            return 2;
        }
    }




}

bool OpeDB::handleAddVIP(const char *name)
{
    QString VIP_sql = QString("UPDATE usrInfo SET VIP = 1 WHERE name = '%1'").arg(name);
    QSqlQuery query(QSqlDatabase::database(QString::number((quintptr)QThread::currentThreadId())));
    if (!query.exec(VIP_sql)) {
        return false;  // 查询失败，返回密码更新失败
    }else{
        return true;
    }

}

int OpeDB::handleGetVIP(const char *name)
{
    QString VIP_get_sql = QString("SELECT VIP FROM usrInfo WHERE name = '%1'").arg(name);
    QSqlQuery query(QSqlDatabase::database(QString::number((quintptr)QThread::currentThreadId())));


    if (!query.exec(VIP_get_sql)) {
        return -1;  // 查询失败
    } else {
        if (query.next()) {
            int vipStatus = query.value(0).toInt();
            qDebug() << "VIP status for" << name << ":" << vipStatus;
            return vipStatus;
        } else {
            return -1;
        }
    }

}

//创建群聊
void OpeDB::handleAddGroup(int groupID, const char *name)
{
    if(NULL == name)
    {
        return ;
    }
    QString groupName = "group " + QString::number(groupID);
    QString data = "INSERT INTO groupTbl (group_id, group_name, member_id) VALUES (" + QString::number(groupID) + " , '" + groupName + "', (SELECT id FROM usrInfo WHERE name = '" + QString::fromLocal8Bit(name) +"'))";
    QSqlQuery query(QSqlDatabase::database(QString::number((quintptr)QThread::currentThreadId())));
    query.exec(data);
    return;
}

int OpeDB::handleGroupID()
{
    QString data = "SELECT MAX(group_id) FROM groupTbl";
    QSqlQuery query(QSqlDatabase::database(QString::number((quintptr)QThread::currentThreadId())));
    query.exec(data);
    if(query.next()){
        return query.value(0).toInt();
    }
    qDebug() << "Search failed";
    return 0;
}

QStringList OpeDB::handleGetNonMemberFriend(const char *pername, const char *groupname)
{
    QStringList strFriendList;
    strFriendList.clear();
    if(NULL == pername || NULL == groupname)
    {
        return strFriendList;
    }
    QString data = QString("select name from usrInfo where online=1 and id in (select id from friend where friendId=(select id from usrInfo where name=\'%1\'))").arg(pername);
    QSqlQuery query(QSqlDatabase::database(QString::number((quintptr)QThread::currentThreadId())));
    query.exec(data);
    while(query.next())
    {
        QString friendName = query.value("name").toString();
        //查找好友是否在群聊
        QString dataStr = QString("select * from groupTbl where group_name = \'%1\' and member_id = ( select id from usrInfo where name = \'%2\' )").arg(groupname).arg(friendName);
        QSqlQuery qs(QSqlDatabase::database(QString::number((quintptr)QThread::currentThreadId())));
        qs.exec(dataStr);
        if(!qs.next()){
            //如果好友不在群聊
            strFriendList.append(friendName);
        }
    }
    data = QString("select name from usrInfo where online=1 and id in (select friendId from friend where id=(select id from usrInfo where name=\'%1\'))").arg(pername);
    query.exec(data);
    while(query.next()) //移动到下一行
    {
        QString friendName = query.value("name").toString();
        //查找好友是否在群聊
        QString dataStr = QString("select * from groupTbl where group_name = \'%1\' and member_id = ( select id from usrInfo where name = \'%2\' )").arg(groupname).arg(friendName);
        QSqlQuery qs(QSqlDatabase::database(QString::number((quintptr)QThread::currentThreadId())));
        qs.exec(dataStr);
        if(!qs.next()){
            //如果好友不在群聊
            strFriendList.append(friendName);
        }
    }
    return strFriendList;
}

void OpeDB::handleAddGroup(const char *groupname, const char *name)
{
    if(NULL == name || NULL == groupname)
    {
        return ;
    }

    QString dataStr = QString("select group_id from groupTbl where group_name = \'%1\'").arg(groupname);
    QSqlQuery qs(QSqlDatabase::database(QString::number((quintptr)QThread::currentThreadId())));
    qs.exec(dataStr);
    int groupID = 0;
    if(qs.next()){
         groupID = qs.value(0).toInt();
    }

    if(groupID != 0 ){
        QString data = "INSERT INTO groupTbl (group_id, group_name, member_id) VALUES (" + QString::number(groupID) + " , '" + QString::fromLocal8Bit(groupname) + "', (SELECT id FROM usrInfo WHERE name = '" + QString::fromLocal8Bit(name) +"'))";
        QSqlQuery query(QSqlDatabase::database(QString::number((quintptr)QThread::currentThreadId())));
        query.exec(data);
    }
    return;
}

//处理改群名
void OpeDB::handleChangeGroupName(const char *oldname, const char *newname)
{
    QString data = QString("update groupTbl set group_name = \'%1\' where group_name = \'%2\'").arg(newname).arg(oldname);
    QSqlQuery qs(QSqlDatabase::database(QString::number((quintptr)QThread::currentThreadId())));
    qs.exec(data);
    qDebug() << data;
    return ;
}

//处理删除文件时间
void OpeDB::handleAddToRecycleBin(const char *username, const char *fileName)
{
    if (NULL == username || NULL == fileName)
    {
        return;
    }

    QString data = QString("INSERT INTO recycleBin (name, fileName, time) VALUES ('%1', '%2', CURRENT_TIMESTAMP)").arg(username).arg(fileName);

    QSqlQuery query(QSqlDatabase::database(QString::number((quintptr)QThread::currentThreadId())));
    if (!query.exec(data))
    {
        qDebug() << "Failed to insert into recycleBin";
    }
    else
    {
        qDebug() << "File information successfully inserted.";
    }
}

QSqlDatabase OpeDB::getMainDb()
{
    return m_db;
}

