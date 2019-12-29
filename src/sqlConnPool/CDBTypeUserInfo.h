#ifndef CDBTYPEUSERINFO_H
#define CDBTYPEUSERINFO_H

#include <string>
#include "configure.h"

using std::string;

enum DBType{
    DBTypeMysql,        // Mysql
    DBTypePgsql,        // Postgresql
    DBTypeOracle,       // Oracle
    DBTypeSQLite        // SQLite
};

class ACT_CPP_DLL_API CDBTypeUserInfo
{
public:
    CDBTypeUserInfo();
    CDBTypeUserInfo(const int dbType, const string& host, const string& user, const string& pswd, const string& db, int& port);
    CDBTypeUserInfo(const int dbType, const char* host, const char* user, const char* pswd, const char* db, int port);
    CDBTypeUserInfo(const CDBTypeUserInfo& other);
    CDBTypeUserInfo& operator=(const CDBTypeUserInfo& other);

    int dbType() const;
    void setDbType(const int &dbType);

    string host() const;
    void setHost(const string &host);

    string user() const;
    void setUser(const string &user);

    string pswd() const;
    void setPswd(const string &pswd);

    string db() const;
    void setDb(const string &db);

    int port() const;
    void setPort(int port);

private:
    int    m_dbType;           // 数据库种类（Mysql，postgresql，sqlite 等）
    string m_host;             // 主机
    string m_user;             // 用户名
    string m_pswd;             // 密码
    string m_db;               // 数据库名
    int    m_port;             // 端口
};

#endif // CDBTYPEUSERINFO_H
