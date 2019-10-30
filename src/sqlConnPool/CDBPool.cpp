#include "CDBPool.h"

#include "CDBQuery.h"

#include "CDBTypeUserInfo.h"

extern CDBConnPool*                g_defPool;
extern map<string, CDBConnPool*>   g_mapPools;

CDBPool::CDBPool()
{
    m_pool   = nullptr;
    m_dbType = 0;
    m_port   = 0;

    m_connMinNum = 0;
    m_connMaxNum = 0;
}

CDBPool::CDBPool(const int dbType, const string &host, const string &user, const string &pswd, const string &db, const int port)
{
    m_pool   = nullptr;
    setDbType(dbType);
    setHost(host);
    setUser(user);
    setPswd(pswd);
    setDbname(db);
    setPort(port);
}

CDBPool::~CDBPool()
{

}

int CDBPool::create(const string &keyname)
{
	if (m_host.empty() 
		&& m_user.empty() 
		&& m_pswd.empty() 
		&& m_dbname.empty() 
		&& m_port == 0)
	{
		return -1;
	}

    if(keyname.empty()) 
	{
        return -1;
    }

    map<string, CDBConnPool*>::iterator index = g_mapPools.find(keyname);
    if(index != g_mapPools.end()) {
        return 0;
    }

    CDBTypeUserInfo info(m_dbType, m_host, m_user, m_pswd, m_dbname, m_port);
    m_pool = new CDBConnPool(info);
    if(m_pool->m_isSuccess == false) {
        SDelete(m_pool);
        return -1;
    }

    if(m_connMinNum > 0 && m_connMaxNum > 0) {
        m_pool->setConnNum(m_connMinNum, m_connMaxNum);
    }

    if(g_defPool == nullptr) {
        g_defPool = m_pool;
    }

    g_mapPools[keyname] = m_pool;
    return 0;
}

int CDBPool::clear(const string &keyname)
{
    map<string, CDBConnPool*>::iterator index = g_mapPools.find(keyname);
    if(index == g_mapPools.end()) {
        return 0;
    }
    CDBConnPool* tmp = index->second;
    g_mapPools.erase(index);
    if(g_defPool == tmp) {
        if(g_mapPools.size() > 0) {
            g_defPool = g_mapPools.begin()->second;
        } else {
            g_defPool = nullptr;
        }
    }
    SDelete(tmp);
    return 0;
}

void CDBPool::setDbType(const int dbType)
{
    m_dbType = dbType;
}

void CDBPool::setHost(const string &host)
{
    m_host = host;
}

void CDBPool::setUser(const string &user)
{
    m_user = user;
}

void CDBPool::setPswd(const string &pswd)
{
    m_pswd = pswd;
}

void CDBPool::setDbname(const string &dbname)
{
    m_dbname = dbname;
}

void CDBPool::setPort(const int port)
{
    m_port = port;
}

int CDBPool::setLinkCount(const unsigned connMinNum, const unsigned connMaxNum)
{
    // 连接池连接数设置，最大值必须大于最小值
    if(connMinNum > connMaxNum) {
        return -1;
    }
    if(m_pool == nullptr) {
        m_connMinNum = connMinNum;
        m_connMaxNum = connMaxNum;
    } else {
        m_pool->setConnNum(connMinNum, connMaxNum);
    }
    return 0;
}
