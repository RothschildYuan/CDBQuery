#include "CDBTypeUserInfo.h"

CDBTypeUserInfo::CDBTypeUserInfo()
{
    m_dbType        = DBTypeMysql;
    m_port          = 0;
}

CDBTypeUserInfo::CDBTypeUserInfo(const int dbType, const string& host, const string& user, const string& pswd, const string& db, int& port) {
    m_dbType        = dbType;
    m_host          = host;
    m_user          = user;
    m_pswd          = pswd;
    m_db            = db;
    m_port          = port;
}

CDBTypeUserInfo::CDBTypeUserInfo(const int dbType, const char* host, const char* user, const char* pswd, const char* db, int port) {
    m_dbType        = dbType;
    m_host          = host;
    m_user          = user;
    m_pswd          = pswd;
    m_db            = db;
    m_port          = port;
}

CDBTypeUserInfo::CDBTypeUserInfo(const CDBTypeUserInfo& other)
{
    m_dbType        = other.m_dbType;
    m_host          = other.m_host;
    m_user          = other.m_user;
    m_pswd          = other.m_pswd;
    m_db            = other.m_db;
    m_port          = other.m_port;
}

CDBTypeUserInfo& CDBTypeUserInfo::operator=(const CDBTypeUserInfo& other)
{
    if(this != &other) {
       new (this) CDBTypeUserInfo(other);
    }
    return *this;
}

int CDBTypeUserInfo::dbType() const
{
    return m_dbType;
}

void CDBTypeUserInfo::setDbType(const int &dbType)
{
    m_dbType = dbType;
}

string CDBTypeUserInfo::host() const
{
    return m_host;
}

void CDBTypeUserInfo::setHost(const string &host)
{
    m_host = host;
}

string CDBTypeUserInfo::user() const
{
    return m_user;
}

void CDBTypeUserInfo::setUser(const string &user)
{
    m_user = user;
}

string CDBTypeUserInfo::pswd() const
{
    return m_pswd;
}

void CDBTypeUserInfo::setPswd(const string &pswd)
{
    m_pswd = pswd;
}

string CDBTypeUserInfo::db() const
{
    return m_db;
}

void CDBTypeUserInfo::setDb(const string &db)
{
    m_db = db;
}

int CDBTypeUserInfo::port() const
{
    return m_port;
}

void CDBTypeUserInfo::setPort(int port)
{
    m_port = port;
}
