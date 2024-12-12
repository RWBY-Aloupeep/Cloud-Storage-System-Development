#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef unsigned int uint;

// 消息回复类型
#define REGIST_OK "regist ok"
#define REGIST_FAILED "regist failed : name existed"
#define LOGIN_OK "login ok"
#define LOGIN_FAILED "login failed : name error or pwd error or relogin"
#define SEARCH_USR_NO "no such people"
#define SEARCH_USR_ONLINE "online"
#define SEARCH_USR_OFFLINE "offline"
#define UNKNOW_ERROR "unknow error"
#define EXISTED_FRIEND "friend exist"
#define ADD_FRIEND_OFFLINE "usr offline"
#define ADD_FRIEND_NO_EXIST "usr not exist"
#define DEL_FRIEND_OK "delete friend ok"
#define DIR_NO_EXIST "cur dir not exist"
#define FILE_NAME_EXIST "file name exist"
#define CREAT_DIR_OK "create dir ok"
#define DEL_DIR_OK "delete dir ok"
#define DEL_DIR_FAILURED "delete dir failured: is reguler file"
#define RENAME_FILE_OK "rename file ok"
#define RENAME_FILE_FAILURED "rename file failured"
#define ENTER_DIR_FAILURED "enter dir failured: is reguler file"
#define DEL_FILE_OK "delete file ok"
#define DEL_FILE_FAILURED "delete file failured: is diretory"
#define UPLOAD_FILE_OK "upload file ok"
#define UPLOAD_FILE_FAILURED "upload file failured"
#define MOVE_FILE_OK "move file ok"
#define MOVE_FILE_FAILURED "move file failured:is reguler file"
#define COMMON_ERR "operate failed: system is busy"
#define CHANGE_PWD_OK "change passward ok"
#define CHANGE_PWD_FAILED "change passward failed"
#define CHANGE_PWD_USR_NOT_EXIST "usr not exist"
#define CHANGE_PWD_PWD_MISMATCH "old password mismatch"
#define UPDATE_PIC_OK "Profile picture updated ok"
#define VIP_PASS "VIP application pass. congrats! you are vip now!"
#define VIP_REJECT "VIP application reject."
// 头像储存文件夹路径
#define PIC_DIR_PATH "./profile_photos"

// 消息类型枚举
enum ENUM_MSG_TYPE
{
    ENUM_MSG_TYPE_MIN = 0,
    ENUM_MSG_TYPE_REGIST_REQUEST,           // 注册请求
    ENUM_MSG_TYPE_REGIST_RESPOND,           // 注册回复
    ENUM_MSG_TYPE_LOGIN_REQUEST,            // 登录请求
    ENUM_MSG_TYPE_LOGIN_RESPOND,            // 登录回复
    ENUM_MSG_TYPE_ALL_ONLINE_REQUEST,       // 在线用户请求
    ENUM_MSG_TYPE_ALL_ONLINE_RESPOND,       // 在线用户回复
    ENUM_MSG_TYPE_SEARCH_USR_REQUEST,       // 搜索用户请求
    ENUM_MSG_TYPE_SEARCH_USR_RESPOND,       // 搜索用户回复
    ENUM_MSG_TYPE_ADD_FRIEND_REQUEST,       // 添加好友请求
    ENUM_MSG_TYPE_ADD_FRIEND_RESPOND,       // 添加好友回复
    ENUM_MSG_TYPE_ADD_FRIEND_AGGREE,        // 同意添加好友
    ENUM_MSG_TYPE_ADD_FRIEND_REFUSE,        // 拒绝添加好友
    ENUM_MSG_TYPE_FLUSH_FRIEND_REQUEST,     // 刷新好友请求
    ENUM_MSG_TYPE_FLUSH_FRIEND_RESPOND,     // 刷新好友回复
    ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST,    // 删除好友请求
    ENUM_MSG_TYPE_DELETE_FRIEND_RESPOND,    // 删除好友回复
    ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST,     // 私聊请求
    ENUM_MSG_TYPE_PRIVATE_CHAT_RESPOND,     // 私聊回复
//    ENUM_MSG_TYPE_GROUP_CHAT_REQUEST,       // 群聊请求
//    ENUM_MSG_TYPE_GROUP_CHAT_RESPOND,       // 群聊回复
    ENUM_MSG_TYPE_CREATE_DIR_REQUEST,       // 创建文件夹请求
    ENUM_MSG_TYPE_CREATE_DIR_RESPOND,       // 创建文件夹回复
    ENUM_MSG_TYPE_FLUSH_FILE_REQUEST,       // 刷新文件请求
    ENUM_MSG_TYPE_FLUSH_FILE_RESPOND,       // 刷新文件回复
    ENUM_MSG_TYPE_DEL_DIR_REQUEST,          // 删除目录请求
    ENUM_MSG_TYPE_DEL_DIR_RESPOND,          // 删除目录回复
    ENUM_MSG_TYPE_RENAME_FILE_REQUEST,      // 重命名文件请求
    ENUM_MSG_TYPE_RENAME_FILE_RESPOND,      // 重命名文件回复
    ENUM_MSG_TYPE_ENTER_DIR_REQUEST,        // 进入文件夹请求
    ENUM_MSG_TYPE_ENTER_DIR_RESPOND,        // 进入文件夹回复
    ENUM_MSG_TYPE_DEL_FILE_REQUEST,         // 删除常规文件请求
    ENUM_MSG_TYPE_DEL_FILE_RESPOND,         // 删除常规文件回复
    ENUM_MSG_TYPE_UPLOAD_FILE_REQUEST,      // 上传文件请求
    ENUM_MSG_TYPE_UPLOAD_FILE_RESPOND,      // 上传文件回复
    ENUM_MSG_TYPE_DOWNLOAD_FILE_REQUEST,    // 下载文件请求
    ENUM_MSG_TYPE_DOWNLOAD_FILE_RESPOND,    // 下载文件回复
    ENUM_MSG_TYPE_SHARE_FILE_REQUEST,       // 共享文件请求
    ENUM_MSG_TYPE_SHARE_FILE_RESPOND,       // 共享文件回复
    ENUM_MSG_TYPE_SHARE_FILE_NOTE_REQUEST,  // 共享文件记录请求
    ENUM_MSG_TYPE_SHARE_FILE_NOTE_RESPOND,  // 共享文件记录回复
    ENUM_MSG_TYPE_MOVE_FILE_REQUEST,        // 移动文件请求
    ENUM_MSG_TYPE_MOVE_FILE_RESPOND,        // 移动文件回复

    ENUM_MSG_TYPE_CLEAR_BIN_REQUEST,        //清空回收站请求
    ENUM_MSG_TYPE_CLEAR_BIN_RESPOND,        //清空回收站回复
    ENUM_MSG_TYPE_RECOVER_REQUEST,        //恢复已放入回收站文件请求
    ENUM_MSG_TYPE_RECOVER_RESPOND,        //恢复已放入回收站文件回复
    ENUM_MSG_TYPE_FLUSH_BIN_REQUEST,        //刷新回收站文件请求
    ENUM_MSG_TYPE_FLUSH_BIN_RESPOND,        //刷新回收站文件回复
    ENUM_MSG_TYPE_ENTER_BIN_REQUEST,        //进入回收站文件请求
    ENUM_MSG_TYPE_ENTER_BIN_RESPOND,        //进入回收站文件回复
    ENUM_MSG_TYPE_VIP_DEL_DIR_REQUEST,      //VIP删除文件夹请求
    ENUM_MSG_TYPE_VIP_DEL_DIR_RESPOND,      //VIP删除文件夹回复

    ENUM_MSG_TYPE_CREAT_GROUP_REQUEST,      //群聊创建请求
    ENUM_MSG_TYPE_CREAT_GROUP_RESPOND,      //群聊创建回复
    ENUM_MSG_TYPE_UPDATE_MEMBER_LIST_REQUEST, //更新朋友列表请求
    ENUM_MSG_TYPE_UPDATE_MEMBER_LIST_RESPOND, //更新朋友列表回复
    ENUM_MSG_TYPE_GROUP_CHAT_REQUEST,           //群聊请求
    ENUM_MSG_TYPE_GROUP_CHAT_RESPOND,           //群聊回复
    ENUM_MSG_TYPE_CANCEL_REQUEST,               //注销请求
    ENUM_MSG_TYPE_CANCEL_RESPOND,               //注销回复

    ENUM_MSG_TYPE_CHANGE_PWD_REQUEST,       // 修改密码请求-
    ENUM_MSG_TYPE_CHANGE_PWD_RESPOND,       // 修改密码回复-
    ENUM_MSG_TYPE_VIP_APPLY_REQUEST,        // 申请vip请求-
    ENUM_MSG_TYPE_VIP_APPLY_RESPOND,        // 申请vip回复-

    ENUM_MSG_TYPE_FLUSH_NONMEMBER_FRIEND_REQUEST, //刷新非群成员的在线好友请求
    ENUM_MSG_TYPE_FLUSH_NONMEMBER_FRIEND_RESPOND, //刷新非群成员的在线好友回复
    ENUM_MSG_TYPE_ADD_MEMBER_REQUEST,   //群聊拉人请求
    ENUM_MSG_TYPE_ADD_MEMBER_RESPOND,   //群聊拉人回复
    ENUM_MSG_TYPE_ENTER_GROUP_REQUEST,  //被拉入群聊请求
    ENUM_MSG_TYPE_ENTER_GROUP_RESPOND,  //被拉入群聊回复

    ENUM_MSG_TYPE_CHANGE_GROUP_NAME_REQUEST, //修改群名请求
    ENUM_MSG_TYPE_CHANGE_GROUP_NAME_RESPOND, //修改群名回复

    ENUM_MSG_TYPE_SEARCH_FILE_REQUEST,      //查找文件请求
    ENUM_MSG_TYPE_SEARCH_FILE_RESPOND,     //查找文件回复

    ENUM_MSG_TYPE_ZIP_REQUEST, //压缩文件请求
    ENUM_MSG_TYPE_ZIP_RESPOND, //压缩文件回复
    ENUM_MSG_TYPE_UNZIP_REQUEST, //解压文件请求
    ENUM_MSG_TYPE_UNZIP_RESPOND, //解压文件回复

    ENUM_MSG_TYPE_MAX = 0x00ffffff,
};

// 文件信息结构体
struct FileInfo
{
    char caFileName[32];    // 文件名字
    int iFileType;          // 文件类型
};

struct PDU
{
    uint uiPDULen;      // 总的协议数据单元大小
    uint uiMsgType;     // 消息类型
    char caData[128];    // 文件名
    uint uiMsgLen;      // 实际消息长度
    int caMsg[];        // 实际消息
};

PDU *mkPDU(uint uiMsgLen);

#endif // PROTOCOL_H
