#include "CDBPgsqlConnect.h"

#include <stdio.h>
#include <string.h>
#include <iostream>

#include "config.h"
#include "statuscode.h"

using namespace std;

#ifdef ORM_OF_PGSQL

CDBPgsqlConnect::CDBPgsqlConnect()
{
    m_pPsql         = nullptr;
    m_count         = 0;
    m_paramTypes    = nullptr;
    m_param         = nullptr;
    m_paramlen      = nullptr;
}

CDBPgsqlConnect::~CDBPgsqlConnect()
{
    disconnect();
}

int CDBPgsqlConnect::connect(const char* host, const char* user, const char* password, const char* db, int port)
{
    int ret = 0;
    cout << "Connect \n";
    char buf[512] = {0};
    snprintf(buf, sizeof(buf), "dbname=%s user=%s password=%s hostaddr=%s port=%d", db, user, password, host, port);
    m_pPsql = PQconnectdb(buf);
    if (PQstatus(m_pPsql) != CONNECTION_OK)
    {
        fprintf(stdout, "Connection to DB failed: %s", PQerrorMessage(m_pPsql));
        SPQfinish(m_pPsql);
        ret = DB_CONNECT_ERROR;
    }
    return ret;
}

void CDBPgsqlConnect::disconnect()
{
    SPQfinish(m_pPsql);
}

int CDBPgsqlConnect::pagingCount()
{
    int num = 0;
    string::size_type beg = m_sql.find("from");
    string::size_type end = m_sql.find("order");
    string substr;
    if(end == string::npos) {
        substr = "select count(*) " + m_sql.substr(beg, end-beg);
    } else {
        substr = "select count(*) " + m_sql.substr(beg, end-beg) + ";";
    }
//    cout<<"m_sql: "<<m_sql<<endl;
//    int ret = excuteQuery(substr.c_str());
//    while (ret->next()) {
//        ret->getField(0, num);
//        break;
//    }
    return num;
}

void CDBPgsqlConnect::clearQuery()
{
    m_resultSet.clear();
}

int CDBPgsqlConnect::connctionTest()
{
    if (m_pPsql == nullptr)
    {
        return DB_HANDLE_GONE_AWAY_ERROR;
    }
    return PQstatus(m_pPsql);
}

int CDBPgsqlConnect::excuteQuery(const char *szSql)
{
    if (m_pPsql == nullptr)
    {
        return DB_HANDLE_GONE_AWAY_ERROR;
    }
    cout<<"szSql: "<<szSql<<endl;
    PGresult *retdata = PQexec(m_pPsql, szSql);
    ExecStatusType statustype = PQresultStatus(retdata);
    if (statustype != PGRES_TUPLES_OK && statustype != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "PQexec failed: %s", PQerrorMessage(m_pPsql));
        SPQclear(retdata);
        return DB_QUERY_ERROR;
    }
    m_resultSet.reset(retdata);
    return 0;
}

int CDBPgsqlConnect::excuteSql(const char *szSql)
{
    int ret = -1;
    int sqlType = 0;    // 1 insert 2 other 2 delete 3 update
    do {
        if (m_pPsql == nullptr)
        {
            ret = DB_HANDLE_GONE_AWAY_ERROR;
            break;
        }
        char* tmpSql = nullptr;
        if(strcasestr(szSql, "INSERT") != nullptr) {
            tmpSql = (char*)calloc(1, strlen(szSql) + 32);
            snprintf(tmpSql, strlen(szSql), "%s", szSql);
            strcat(tmpSql, " RETURNING id;");
            sqlType = 1;
        } else {
            tmpSql = (char*)strdup(szSql);
            sqlType = 2;
        }
        PGresult *m_res = PQexec(m_pPsql, tmpSql);
        SFree(tmpSql);
        ExecStatusType statustype = PQresultStatus(m_res);
        if (statustype != PGRES_COMMAND_OK &&
                statustype != PGRES_TUPLES_OK &&
                statustype != PGRES_EMPTY_QUERY)
        {
            fprintf(stderr, "PQexec failed: %s\n", PQerrorMessage(m_pPsql));
        } else {
            ret = 0;
        }
        if(sqlType == 1) {
            ret = atoi(PQgetvalue(m_res, 0, 0));
        } else {
            // 返回被修改数据的条数
        }
        SPQclear(m_res);
    } while(0);
    return ret;
}

const char* CDBPgsqlConnect::getLastErrorMsg()
{
    if (m_pPsql == nullptr)
    {
        return "DB_ERROR_NOT_MYSQL_INIT";
    }
    return PQerrorMessage(m_pPsql);
}

unsigned long CDBPgsqlConnect::escapeRealToString(  char *pDst, const char *pSrc, unsigned long ulSrcLen )
{
    if (m_pPsql == nullptr)
    {
        return SUCCESS;
    }
    //这里添加参数检查代码…
    if( ulSrcLen <= 0 )
    {
        //这里添加日志打印语句
        return ulSrcLen;
    }
    return PQescapeString(pDst, pSrc, ulSrcLen);
}

int CDBPgsqlConnect::prepare(const string& query)
{
    m_sql = query;
    return SUCCESS;
}

int CDBPgsqlConnect::exec(const string& sql) {
    if(!sql.empty()) {
        return  excuteSql(sql.c_str());
    }
    int ret = 0;
    if(m_sql.empty()) {
        return -1;
    }
    int sqlType = 0;
    char* tmpSql = nullptr;
    if(strcasestr(m_sql.c_str(), "INSERT") != nullptr) {
        tmpSql = (char*)alloca(m_sql.length() + 32);
        snprintf(tmpSql, m_sql.length(), "%s", m_sql.c_str());
        strcat(tmpSql, " RETURNING id;");
        sqlType = 1;
    } else {
        tmpSql = (char*)m_sql.c_str();
        sqlType = 2;
    }
    PGresult* result = PQprepare(m_pPsql, "1112", tmpSql, m_count, m_paramTypes);
    ExecStatusType statustype = PQresultStatus(result);
    if (statustype != PGRES_TUPLES_OK && statustype != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "PQexec failed: %s", PQerrorMessage(m_pPsql));
        SPQclear(result);
        return -2;
    }
    SPQclear(result);
    result = PQexecPrepared(m_pPsql, "1112", m_count, m_param, m_paramlen, nullptr, 0);
    statustype = PQresultStatus(result);
    if (statustype != PGRES_TUPLES_OK && statustype != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "PQexec failed: %s", PQerrorMessage(m_pPsql));
        SPQclear(result);
        return -3;
    }
    if(sqlType == 1) {
        ret = atoi(PQgetvalue(result, 0, 0));
    } else {
        // 返回被修改数据的条数
    }
    SPQclear(result);
    return ret;
}

int CDBPgsqlConnect::query(const string& sql) {
    if(!sql.empty()) {
        return  excuteQuery(sql.c_str());
    }
    if(m_sql.empty()) {
        return DB_STMT_INIT_ERROR;
    }
    PGresult* result = PQprepare(m_pPsql, "stmtPqsql", m_sql.c_str(), m_count, m_paramTypes);
    ExecStatusType statustype = PQresultStatus(result);
    if (statustype != PGRES_TUPLES_OK && statustype != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "PQexec failed: %s", PQerrorMessage(m_pPsql));
        SPQclear(result);
        return DB_STMT_PREPARE_ERROR;
    }
    SPQclear(result);
    result = PQexecPrepared(m_pPsql, "stmtPqsql", m_count, m_param, m_paramlen, nullptr, 0);
    statustype = PQresultStatus(result);
    if (statustype != PGRES_TUPLES_OK && statustype != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "PQexec failed: %s", PQerrorMessage(m_pPsql));
        SPQclear(result);
        return DB_STMT_EXEC_PREPARED_ERROR;
    }
    m_resultSet.reset(result);
    SPQclear(result);
    return 0;
}

bool CDBPgsqlConnect::isEnd()
{
    return m_resultSet.isEnd();
}

int CDBPgsqlConnect::size()
{
    return m_resultSet.size();
}

bool CDBPgsqlConnect::next()
{
    return m_resultSet.next();
}

CDBObject CDBPgsqlConnect::value(const string &fieldName)
{
    string data;
    int ret = getValue(fieldName, data);
    if(ret < 0) {
        return "";
    }
    return data;
}

CDBResultSet::CDBResult CDBPgsqlConnect::getValue(const string &fieldName)
{
    return m_resultSet.getValueRes(fieldName);
}

CDBResultSet::CDBResult CDBPgsqlConnect::getValue(const int &fieldId)
{
    return m_resultSet.getValueRes(fieldId);
}

int CDBPgsqlConnect::getValue(const int fieldId, int &value)
{
    return m_resultSet.getField(fieldId, value);
}

int CDBPgsqlConnect::getValue(const int fieldId, string &value)
{
    return m_resultSet.getField(fieldId, value);
}

int CDBPgsqlConnect::getValue(const int fieldId, bool &value)
{
    return m_resultSet.getField(fieldId, value);
}

int CDBPgsqlConnect::getValue(const int fieldId, float &value)
{
    return m_resultSet.getField(fieldId, value);
}

int CDBPgsqlConnect::getValue(const int fieldId, double &value)
{
    return m_resultSet.getField(fieldId, value);
}

int CDBPgsqlConnect::getValue(const int fieldId, time_t &value)
{
    return m_resultSet.getField(fieldId, value);
}

int CDBPgsqlConnect::getValue(const string &fieldName, int &value)
{
    return m_resultSet.getField(fieldName, value);
}

int CDBPgsqlConnect::getValue(const string &fieldName, string &value)
{
    return m_resultSet.getField(fieldName, value);
}

int CDBPgsqlConnect::getValue(const string &fieldName, bool &value)
{
    return m_resultSet.getField(fieldName, value);
}

int CDBPgsqlConnect::getValue(const string &fieldName, float &value)
{
    return m_resultSet.getField(fieldName, value);
}

int CDBPgsqlConnect::getValue(const string &fieldName, double &value)
{
    return m_resultSet.getField(fieldName, value);
}

int CDBPgsqlConnect::getValue(const string &fieldName, time_t &value)
{
    return m_resultSet.getField(fieldName, value);
}

void CDBPgsqlConnect::clearStmt() {
    m_count     = 0;
    SFree(m_paramTypes);
    for(int i = 0; i<m_count; ++i) {
        SFree(m_param[i]);
    }
    SFree(m_param);
    SFree(m_paramlen);
}

int CDBPgsqlConnect::addBindValue(const int& index, const int& value) {
    m_paramTypes[index]    = 23;
    m_param[index]         = (char*)calloc(1, 20);
    snprintf(m_param[index], 20, "%d", value);
    m_paramlen[index]      = strlen(m_param[index]);
    return SUCCESS;
}

int CDBPgsqlConnect::addBindValue(const int& index, const double& value) {
    m_paramTypes[index]    = 701;
    m_param[index]         = (char*)calloc(1, 20);
    snprintf(m_param[index], 20, "%.2f", value);
    m_paramlen[index]      = strlen(m_param[index]);
    return SUCCESS;
}

int CDBPgsqlConnect::addBindValue(const int& index, const string& value) {
    m_paramTypes[index]    = 25;
    m_param[index]         = (char*)strdup(value.c_str());
    if(value.length() == 19 ) {
        m_paramTypes[index]    = 1114;
    }
    m_paramlen[index]      = strlen(m_param[index]);
    return SUCCESS;
}

int CDBPgsqlConnect::addBindValue(const int& index, const char* value) {
    m_paramTypes[index]    = 25;
    m_param[index]         = (char*)strdup(value);
    if(strlen(value) == 19 ) {
        m_paramTypes[index]    = 1114;
    }
    m_paramlen[index]      = strlen(m_param[index]);
    return SUCCESS;
}

#endif
