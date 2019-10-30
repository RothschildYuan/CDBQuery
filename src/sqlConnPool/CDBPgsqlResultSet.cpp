#include "CDBPgsqlResultSet.h"

#include <string.h>

#include <iostream>

#include "CDBConfig.h"
#include "common.h"
#include "statuscode.h"

/*
PGresult *PQexec(PGconn *conn,const char *query) 返回查询的结构集。
int PQntuples(const Pgresult *res) 返回查询结果里的记录个数。
int PQnfields(const Pgresult *res) 返回记录中的字段的个数。
char *PQfname(const Pgresult *res,int field_index) 返回结构集中某一字段的名称。
Oid PQftype(const Pgresult *res, int field_index)  返回结构集中某一字段的数据类型。
char *PQgetvalue(const Pgresult *res,int tup_num, int field_index) 返回结构集中某一条记录中某一个字段的值。
*/

#ifdef ORM_OF_PGSQL

CDBPgsqlResultSet::CDBPgsqlResultSet():CDBResultSet()
{
    m_pQuery        = nullptr;
}

CDBPgsqlResultSet::~CDBPgsqlResultSet()
{
    clear();
}

bool CDBPgsqlResultSet::isEnd()
{
    if (m_pQuery == nullptr)
    {
        return false;
    }
    return m_nRowIndex < m_nRowCount;
}

void CDBPgsqlResultSet::reset(PGresult* res)
{
    clear();
    m_pQuery  = res;
    // 返回记录中的字段的个数。
    m_nColumnCount = PQnfields(res);
    // 返回查询结果里的记录个数。
    m_nRowCount = PQntuples(res);
    m_isSTMT = ESqlExecModeQuery;
}

void CDBPgsqlResultSet::clear()
{
    SPQclear(m_pQuery);
    m_nColumnCount  = 0;
    m_nRowCount     = 0;
    m_nRowIndex     = -1;
    m_isSTMT        = 0;
}

int CDBPgsqlResultSet::getFieldIdByName(const string& fieldName)
{
    if (m_pQuery == nullptr)
    {
        return DB_GET_FIELDS_ERROR;
    }
    return PQfnumber(m_pQuery, fieldName.c_str());
}

bool CDBPgsqlResultSet::next()
{
    if(m_isSTMT == ESqlExecModeQuery) {
        return getNextRow();
    }
    return false;
}

int CDBPgsqlResultSet::getField(const int fieldId, int& value)
{
    if(m_isSTMT == ESqlExecModeQuery) {
        return getValue(fieldId,  value);
    }
    return SUCCESS;
}

int CDBPgsqlResultSet::getField(const int fieldId, string& value)
{
    if(m_isSTMT == ESqlExecModeQuery) {
        return getValue(fieldId,  value);
    }
    return SUCCESS;
}

int CDBPgsqlResultSet::getField(const int fieldId, QText& obj)
{
    if(m_isSTMT == ESqlExecModeQuery) {
        return getValue(fieldId,  obj);
    }
    return SUCCESS;
}

int CDBPgsqlResultSet::getField(const int fieldId, bool& value)
{
    if(m_isSTMT == ESqlExecModeQuery) {
        return getValue(fieldId,  value);
    }
    return SUCCESS;
}

int CDBPgsqlResultSet::getField(const int fieldId, float& value)
{
    if(m_isSTMT == ESqlExecModeQuery) {
        return getValue(fieldId,  value);
    }
    return SUCCESS;
}

int CDBPgsqlResultSet::getField(const int fieldId, double& value)
{
    if(m_isSTMT == ESqlExecModeQuery) {
        return getValue(fieldId,  value);
    }
    return SUCCESS;
}

int CDBPgsqlResultSet::getField(const int fieldId, time_t& value)
{
    if(m_isSTMT == ESqlExecModeQuery) {
        return getValue(fieldId,  value);
    }
    return SUCCESS;
}

bool CDBPgsqlResultSet::getNextRow()
{
    if (m_pQuery == nullptr)
    {
        return false;
    }
    ++m_nRowIndex;
    return m_nRowIndex < m_nRowCount;
}

int CDBPgsqlResultSet::getValue(const int fieldId, int& value)
{
    if (m_pQuery == nullptr || fieldId>=m_nColumnCount)
    {
        return DB_GET_FIELDS_ERROR;
    }
    char* pField = PQgetvalue(m_pQuery, m_nRowIndex, fieldId);
    if (pField == nullptr || pField[0] == 0)
    {
        return SUCCESS;
    }
    value = atoi(pField);
    return SUCCESS;
}

int CDBPgsqlResultSet::getValue(const int fieldId, string& value)
{
    if (m_pQuery == nullptr || fieldId>=m_nColumnCount)
    {
        return DB_GET_FIELDS_ERROR;
    }
    char* pField = PQgetvalue(m_pQuery, m_nRowIndex, fieldId);
    if (pField == nullptr || pField[0] == 0)
    {
        return SUCCESS;
    }
#ifdef __linux__
    value = pField;
#else
    value = UTF8_2_GBK(pField);
#endif
    return SUCCESS;
}

int CDBPgsqlResultSet::getValue(const int fieldId, QText& obj)
{
    char* value = obj.m_data;
    size_t len  = obj.m_len;
    if (m_pQuery == nullptr || fieldId>=m_nColumnCount || value == nullptr || len == 0)
    {
        return DB_GET_FIELDS_ERROR;
    }
    char* pField = PQgetvalue(m_pQuery, m_nRowIndex, fieldId);
    if (pField == nullptr)
    {
        value[0] = '\0';
    }
    else
    {
        strncpy(value, pField, len-1);
        value[len-1] = '\0';
    }
    return SUCCESS;
}

int CDBPgsqlResultSet::getValue(const int fieldId, bool& value)
{
    if (m_pQuery == nullptr || fieldId>=m_nColumnCount)
    {
        return DB_GET_FIELDS_ERROR;
    }
    char* pField = PQgetvalue(m_pQuery, m_nRowIndex, fieldId);
    if (pField == nullptr || pField[0] == 0)
    {
        return SUCCESS;
    }
    value = atoi(pField) != 0;
    return SUCCESS;
}

int CDBPgsqlResultSet::getValue(const int fieldId, float& value)
{
    if (m_pQuery == nullptr || fieldId>=m_nColumnCount)
    {
        return DB_GET_FIELDS_ERROR;
    }
    char* pField = PQgetvalue(m_pQuery, m_nRowIndex, fieldId);
    if (pField == nullptr || pField[0] == 0)
    {
        return SUCCESS;
    }
    value = (float)atof(pField);
    return SUCCESS;
}

int CDBPgsqlResultSet::getValue(const int fieldId, double& value)
{
    if (m_pQuery == nullptr || fieldId>=m_nColumnCount)
    {
        return DB_GET_FIELDS_ERROR;
    }
    char* pField = PQgetvalue(m_pQuery, m_nRowIndex, fieldId);
    if (pField == nullptr || pField[0] == 0)
    {
        return SUCCESS;
    }
    value = atof(pField);
    return SUCCESS;
}

int CDBPgsqlResultSet::getValue(const int fieldId, time_t& value)
{
    if (m_pQuery == nullptr || fieldId>=m_nColumnCount)
    {
        return DB_GET_FIELDS_ERROR;
    }
    char* pField = PQgetvalue(m_pQuery, m_nRowIndex, fieldId);
    tm time;
    memset(&time, 0, sizeof(time));
    if (pField == nullptr || pField[0] == 0)
    {
        return SUCCESS;
    }
    sscanf(pField, "%d-%d-%d %d:%d:%d", &time.tm_year, &time.tm_mon, &time.tm_mday, &time.tm_hour, &time.tm_min, &time.tm_sec);
    time.tm_year -= 1900;
    time.tm_mon  -= 1;
    value = mktime(&time);
    return SUCCESS;
}

#endif
