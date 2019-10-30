#include "CDBConnPool.h"

#include <stdio.h>
#include <time.h>

#include "config.h"
#include "CDBMysqlConnect.h"
#include "CDBPgsqlConnect.h"
#include "CDBSQLiteConnect.h"

#define MYSQL_DBHOST "127.0.0.1"
#define MYSQL_DBUSER "root"
#define MYSQL_DBPSWD "4701641"
#define MYSQL_DBNAME "AAAServerDB"
#define MYSQL_DBPORT 3306

static CDBTypeUserInfo mysqlInfo(DBTypeMysql, MYSQL_DBHOST, MYSQL_DBUSER, MYSQL_DBPSWD, MYSQL_DBNAME, MYSQL_DBPORT);

#define PGSQL_DBHOST "127.0.0.1"
#define PGSQL_DBUSER "mysql"
#define PGSQL_DBPSWD "4701641"
#define PGSQL_DBNAME "act"
#define PGSQL_DBPORT 5432

static CDBTypeUserInfo pgsqlInfo(DBTypePgsql, PGSQL_DBHOST, PGSQL_DBUSER, PGSQL_DBPSWD, PGSQL_DBNAME, PGSQL_DBPORT);

#define SQLite_DBHOST "127.0.0.1"
#define SQLite_DBUSER "mysql"
#define SQLite_DBPSWD "4701641"
#define SQLite_DBNAME "act"
#define SQLite_DBPORT 5432

static CDBTypeUserInfo sqliteInfo(DBTypeSQLite, "./test.db", PGSQL_DBUSER, PGSQL_DBPSWD, PGSQL_DBNAME, PGSQL_DBPORT);

CDBConnPool::CDBConnPool(const CDBTypeUserInfo& sqlInfo)
{
    m_isSuccess     = false;
    m_sqlInfo       = sqlInfo;
    m_connMinNum    = 3;
    m_connMaxNum    = 10;
    m_connRealNum   = 0;
    m_idleConns.clear();
    m_busyConns.clear();

    // 给连接池容器创建最小个数数据库连接
    for(int i = 0; i < m_connMinNum; ++i) {
        CDBConnect* conn = createDBConnect();
        if(conn == nullptr) {
            perror("createDBConnect failed");
            return;
        }
        ++m_connRealNum;
        // 把新建的数据库连接加入连接池的容器中
        m_idleConns.push_back(conn);
    }
    m_isSuccess = true;
}

CDBConnPool::~CDBConnPool()
{
    // 释放容器中保存的数据库连接
    for(DBConnectListIter iter = m_idleConns.begin(); iter != m_idleConns.end(); ++iter) {
            SDelete(*iter);
        }
        m_idleConns.clear();
        for(DBConnectListIter iter = m_busyConns.begin(); iter != m_busyConns.end(); ++iter) {
            SDelete(*iter);
        }
        m_busyConns.clear();
    //删除信号量
//    sem_destroy(&m_poolSemID);
}

CDBConnect* CDBConnPool::createDBConnect()
{
    CDBConnect* conn = nullptr;
    switch (dbType()) {
    case DBTypeMysql:
#ifdef ORM_OF_MYSQL
        conn = new CDBMysqlConnect;
#endif
        break;
    case DBTypePgsql:
#ifdef ORM_OF_PGSQL
        conn = new CDBPgsqlConnect;
#endif
        break;
    case DBTypeSQLite:
#ifdef ORM_OF_SQLITE
        conn = new CDBSQLiteConnect;
#endif
        break;
    default:
        break;
    }
    if(conn == nullptr) {
        return conn;
    }
    int ret = conn->connect(host(), user(), pswd(), db(), port());
    if(ret != 0) {
        SDelete(conn);
        return nullptr;
    }
    return conn;
}

void CDBConnPool::setConnNum(int connMinNum, int connMaxNum)
{
    m_connMinNum    = connMinNum;
    m_connMaxNum    = connMaxNum;
}

CDBConnect* CDBConnPool::getDBconnect(bool is_wait) {
    int ret = 0;
    CDBConnect* pConn = nullptr;
Restart:
    m_mutex.lock();
    // 查看闲置连接容器中是否有闲置的数据库连接
    if (m_idleConns.size() > 0)
    {
        // 如果有，则直接取出连接
        pConn = m_idleConns.front();
        m_idleConns.pop_front();
        m_busyConns.push_back(pConn);
    } else {
        // 如果没有闲置连接，则判断连接池连接数是否达到最大连接数上线值
        if(m_connRealNum == m_connMaxNum) {
            // 达到最大连接数，则返回空
            m_mutex.unlock();
            Sleep(100);
            if(is_wait) {
                goto Restart;
            }
            return pConn;
        }
        // 连接池没有达到连接上线数，则创建新的连接，并把它加入到忙时的连接容器中，并把新创的连接指针返回
        pConn = createDBConnect();
        if(pConn == nullptr) {
            m_mutex.unlock();
            return pConn;
        }
        ++m_connRealNum;
        m_busyConns.push_back(pConn);
    }
    // 放弃信号量
//    sem_post(&m_poolSemID);
    // 测试连接是否连上数据库
    if (pConn->connctionTest() != 0)
    {
        printf("\n\n$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n\n");
        // 如果测试没有连上数据库，则断开连接，重连数据库
        pConn->disconnect();
        ret = pConn->connect(host(), user(), pswd(), db(), port());
        if (ret < 0)
        {
//            sem_wait(&m_poolSemID);
            m_busyConns.remove(pConn);
            m_idleConns.push_back(pConn);
//            sem_post(&m_poolSemID);
            m_mutex.unlock();
            return nullptr;
        }
    }
    // 如果连上数据库，则把数据库连接指针返回
    m_mutex.unlock();
    return pConn;
}

void CDBConnPool::freeDBconnect(CDBConnect* conn) {
    if(conn == nullptr) {
        return;
    }
    // 初始化数据库连接中的 resultSet值
    conn->clearQuery();
//    sem_wait(&m_poolSemID);
    m_mutex.lock();
    // 在忙置容器中删除连接
    m_busyConns.remove(conn);
    // 如果闲置容器连接数过多，则关闭部分闲置连接，
    if(m_busyConns.size() * 3 < m_idleConns.size() && m_connRealNum > m_connMinNum) {
        SDelete(conn);
        --m_connRealNum;
    } else {
        // 如果闲置连接数不是过多，则把返回的连接加入到闲置连接容器中
        m_idleConns.push_back(conn);
    }
//    sem_post(&m_poolSemID);
    m_mutex.unlock();
}

int CDBConnPool::dbType() const {
    return m_sqlInfo.dbType();
}

string CDBConnPool::host() const {
    return m_sqlInfo.host();
}

string CDBConnPool::user() const {
    return m_sqlInfo.user();
}

string CDBConnPool::pswd() const {
    return m_sqlInfo.pswd();
}

string CDBConnPool::db() const {
    return m_sqlInfo.db();
}

int CDBConnPool::port() const {
    return m_sqlInfo.port();
}
