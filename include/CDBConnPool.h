#ifndef CDBCONNPOOL_H
#define CDBCONNPOOL_H

#include "CDBConnect.h"
#include "CDBTypeUserInfo.h"

class CDBConnPool
{
    typedef list<CDBConnect*> DBConnectList;
    typedef DBConnectList::iterator DBConnectListIter;

    CDBConnPool(const CDBConnPool& );
    CDBConnPool& operator=(const CDBConnPool& );

    CDBConnect* createDBConnect();
public:
    CDBConnPool(const CDBTypeUserInfo& sqlInfo);
    virtual~ CDBConnPool();

    /**
     * @brief setConnNum    设置数据库连接池连接数范围
     * @param connMinNum    最小连接数
     * @param connMaxNum    最大连接数
     */
    void setConnNum(int connMinNum, int connMaxNum);

    /**
     * @brief getDBconnect  从连接池中获取一个数据库连接
     * @param is_wait       如果连接池的连接用完后，是否等待，false 则返回 nullptr， true 则等待，直至有连接为止
     * @return              成功返回对象，失败 nullptr
     */
    CDBConnect* getDBconnect(bool is_wait);

    /**
     * @brief freeDBconnect 返回数据库连接给连接池
     * @param conn          无返回
     */
    void freeDBconnect(CDBConnect* conn);

    bool m_isSuccess;
private:
    int    dbType() const;
    string host() const;
    string user() const;
    string pswd() const;
    string db() const;
    int    port() const;

private:
    ACTMutex        m_mutex;
    CDBTypeUserInfo m_sqlInfo;          // 数据库种类（Mysql，postgresql，sqlite 等） 主机/用户名/密码/数据库名/端口

    int             m_connMinNum;       //连接池的大小范围
    int             m_connMaxNum;       //连接池的大小范围
    int             m_connRealNum;      //连接池的大小范围

    DBConnectList   m_idleConns;        // 闲置的数据库连接容器
    DBConnectList   m_busyConns;        // 忙时的数据库连接容器

//    sem_t               m_poolSemID;
};

#endif // CDBCONNPOOL_H
