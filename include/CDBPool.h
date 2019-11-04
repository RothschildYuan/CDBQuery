#ifndef CDBPOOL_H
#define CDBPOOL_H

#include "stdtype.h"
#include "configure.h"

class CDBConnPool;

class ACT_CPP_DLL_API CDBPool
{
    friend class CDBQuery;
public:
    CDBPool();

    /**
     * @brief CDBPool   设置数据库连接帐号等基础信息（SQLite 文件数据库，仅设置数据库实例名 = 文件名）
     * @param dbType    0 DBTypeMysql               Mysql 数据库
     *                  1 DBTypePgsql               Postgresql 数据库
     *                  2 DBTypeOracle 暂不支持      Oracle 数据库
     *                  3 DBTypeSQLite              SQLite 数据库
     * @param host      主机名（或ip）
     * @param user      用户名
     * @param pswd      用户密码
     * @param db        数据库实例名
     * @param port      端口号
     */
    CDBPool(const int dbType, const string& host, const string& user, const string& pswd, const string& db, const int port);
    ~CDBPool();

    /**
     * @brief create    创建数据库连接池
     * @param keyname   连接池名称（名称不能为空，且所有连接池名称必须唯一）
     * @return          0 成功，其他失败
     */
    int create(const string &keyname);

    /**
     * @brief clear     创建数据库连接池
     * @param keyname   连接池名称（不能为空）
     * @return          0 成功，其他失败
     */
    int clear(const string &keyname);

    /**
     * @brief setDbType 设置数据库类型（传数值，宏定义都行）
     * @param dbType    0 DBTypeMysql               Mysql 数据库
     *                  1 DBTypePgsql               Postgresql 数据库
     *                  2 DBTypeOracle 暂不支持      Oracle 数据库
     *                  3 DBTypeSQLite              SQLite 数据库
     */
    void setDbType(const int dbType);

    /**
     * @brief setHost   设置数据库主机名（或ip）
     * @param host      主机名（或ip）
     */
    void setHost(const string &host);

    /**
     * @brief setUser   设置数据库用户名
     * @param user      用户名
     */
    void setUser(const string &user);

    /**
     * @brief setPswd   设置数据库用户密码
     * @param pswd      用户密码
     */
    void setPswd(const string &pswd);

    /**
     * @brief setDbname 设置数据库数据库实例名
     * @param dbname    数据库实例名
     */
    void setDbname(const string &dbname);

    /**
     * @brief setPort   设置数据库端口号
     * @param port      端口号
     */
    void setPort(const int port);

    /**
     * @brief setLinkCount  设置连接池最大连接数和最小连接数  (先设置连接数，再创建连接池，或者先创建连接池，再设置连接数一样)
     * @param connMinNum    最小连接数（最小连接数必须小于等于最大连接数）
     * @param connMaxNum    最大连接数
     * @return              0 成功，其他失败
     */
    int setLinkCount(const unsigned connMinNum, const unsigned connMaxNum);

private:
    CDBConnPool*    m_pool;
    int             m_dbType;
    string          m_host;
    string          m_user;
    string          m_pswd;
    string          m_dbname;
    int             m_port;

    int             m_connMinNum;
    int             m_connMaxNum;
};

#endif // CDBPOOL_H
