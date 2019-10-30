#include "CDBQuery.h"

#include <exception>
#include <stdexcept>

#include "CDBPool.h"

CDBConnPool*                g_defPool  = nullptr;
map<string, CDBConnPool*>   g_mapPools = map<string, CDBConnPool*>();

CDBQuery::CDBQuery()
{
    if(g_defPool == nullptr) {
        perror("has not set default db connect pool");
        return;
    }
    // 构造函数不传参，则启用默认数据库连接池
    m_pool = g_defPool;
    for(map<string, CDBConnPool*>::iterator it = g_mapPools.begin(); it != g_mapPools.end(); ++it) {
        // 查找该连接池名称，并赋值
        if(it->second == m_pool) {
            m_keyname = it->first;
        }
    }
    // 从数据库连接池中获取数据库连接
    m_dbConn = g_defPool->getDBconnect(true);
}

CDBQuery::CDBQuery(const string &dbname)
{
    // 启用指定名称的数据库连接池
    m_keyname = dbname;
    m_pool = g_mapPools[dbname];
    if(m_pool == nullptr) {
        char buf[1024] = {0};
        // 查找不到指定名称的数据库连接池，则会直接报错
        snprintf(buf, sizeof(buf), "dbname %s error, the dbname not exist", dbname.c_str());
        perror(buf);
        m_dbConn = nullptr;
        return;
    }
    // 从数据库连接池中获取数据库连接
    m_dbConn = m_pool->getDBconnect(true);
}

CDBQuery::CDBQuery(const CDBPool *pool)
{
    // 启用指定的数据库连接池
    m_pool = pool->m_pool;
    for(map<string, CDBConnPool*>::iterator it = g_mapPools.begin(); it != g_mapPools.end(); ++it) {
        // 查找该连接池名称，并赋值
        if(it->second == m_pool) {
            m_keyname = it->first;
        }
    }
    // 从数据库连接池中获取数据库连接
    m_dbConn = m_pool->getDBconnect(true);
}

CDBQuery::~CDBQuery()
{
    if(m_pool == nullptr) {
        return;
    }
    // 释放资源时，把连接还予连接池
    m_pool->freeDBconnect(m_dbConn);
    m_dbConn = nullptr;
}

int CDBQuery::begin()
{
    if(m_dbConn == nullptr) {
        return -1;
    }
    return m_dbConn->begin();
}

int CDBQuery::rollback()
{
    if(m_dbConn == nullptr) {
        return -1;
    }
    return m_dbConn->rollback();
}

int CDBQuery::commit()
{
    if(m_dbConn == nullptr) {
        return -1;
    }
    return m_dbConn->commit();
}

void CDBQuery::startPageQuery()
{
    if(m_dbConn == nullptr) {
        return;
    }
    m_dbConn->startPageQuery();
}

void CDBQuery::stopPageQuery()
{
    if(m_dbConn == nullptr) {
        return;
    }
    m_dbConn->stopPageQuery();
}

int CDBQuery::pagingCount()
{
    if(m_dbConn == nullptr) {
        return -1;
    }
    return m_dbConn->pagingCount();
}

int CDBQuery::connctionTest()
{
    if(m_dbConn == nullptr) {
        return -1;
    }
    return m_dbConn->connctionTest();
}

const char *CDBQuery::getLastErrorMsg()
{
    if(m_dbConn == nullptr) {
        return "dbname error, the dbname not exist";
    }
    return m_dbConn->getLastErrorMsg();
}

int CDBQuery::prepare(const string &query)
{
    if(m_dbConn == nullptr) {
        return -1;
    }
    return m_dbConn->prepare(query);
}

int CDBQuery::addBindValue(const int &index, const int &value)
{
    if(m_dbConn == nullptr) {
        return -1;
    }
    return m_dbConn->addBindValue(index, value);
}

int CDBQuery::addBindValue(const int &index, const double &value)
{
    if(m_dbConn == nullptr) {
        return -1;
    }
    return m_dbConn->addBindValue(index, value);
}

int CDBQuery::addBindValue(const int &index, const string &value)
{
    if(m_dbConn == nullptr) {
        return -1;
    }
    return m_dbConn->addBindValue(index, value);
}

int CDBQuery::addBindValue(const int &index, const char *value)
{
    if(m_dbConn == nullptr) {
        return -1;
    }
    return m_dbConn->addBindValue(index, value);
}

int CDBQuery::exec(const string& sql)
{
    if(m_dbConn == nullptr) {
        return -1;
    }
    return m_dbConn->exec(sql);
}

int CDBQuery::query(const string& sql)
{
    if(m_dbConn == nullptr) {
        return -1;
    }
    return m_dbConn->query(sql);
}

CDBObject CDBQuery::value(const string& fieldName) {
    return m_dbConn->value(fieldName);
}

CDBResultSet::CDBResult CDBQuery::getValue(const string& fieldName)
{
    return m_dbConn->getValue(fieldName);
}

CDBResultSet::CDBResult CDBQuery::getValue(const int& fieldId)
{
    return m_dbConn->getValue(fieldId);
}

bool CDBQuery::next()
{
    if(m_dbConn == nullptr) {
        return false;
    }
    return m_dbConn->next();
}

int CDBQuery::size()
{
    if(m_dbConn == nullptr) {
        return -1;
    }
    return m_dbConn->size();
}
