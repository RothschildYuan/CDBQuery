#include "CDBMysqlResultSet.h"

#include <string.h>
#include "statuscode.h"
#include "common.h"

#ifdef ORM_OF_MYSQL

CDBMysqlResultSet::CDBMysqlResultSet():CDBResultSet()
{
    m_data.clear();
    m_pTmp      = nullptr;
    m_pQuery    = nullptr;
    m_curRow    = nullptr;
}

CDBMysqlResultSet::~CDBMysqlResultSet()
{
    clear();
}

bool CDBMysqlResultSet::isEnd()
{
    if (m_pQuery == nullptr)
    {
        return false;
    }
    return mysql_eof(m_pQuery) != 1;
}

int CDBMysqlResultSet::reset(MYSQL_RES *pMyQuery)
{
    clear();
    m_isSTMT = ESqlExecModeQuery;
    m_pQuery   = pMyQuery;
    m_curRow = nullptr;
    if (pMyQuery != nullptr)
    {
        m_nColumnCount = mysql_num_fields(pMyQuery);
    }
    else
    {
        return -1;
    }
    m_nRowCount = (int)mysql_num_rows(m_pQuery);
    return m_nRowCount;
}

#define BUFFERSIZE 1024

int CDBMysqlResultSet::reset(MYSQL_STMT* stmt) {
    clear();
    int ret = -1;
    m_isSTMT = ESqlExecModePrepare;
    do {
        m_nColumnCount = mysql_stmt_field_count(stmt);
        char** buf = (char**)calloc(m_nColumnCount, sizeof(char*));
        MYSQL_BIND* sqlbind = (MYSQL_BIND*)calloc(m_nColumnCount, sizeof(MYSQL_BIND));
        if(sqlbind == nullptr) {
            break;
        }
        for(int i = 0; i<m_nColumnCount; ++i) {
            buf[i] = (char*)calloc(1, BUFFERSIZE);
            sqlbind[i].buffer_type    = MYSQL_TYPE_STRING;
            sqlbind[i].buffer         = (void*)buf[i];
            sqlbind[i].buffer_length  = BUFFERSIZE;
        }
        ret = mysql_stmt_bind_result(stmt, sqlbind);
        if(ret != 0) {
            break;
        }
        ret = mysql_stmt_execute(stmt);
        if(ret != 0) {
            break;
        }
        //以便后续的mysql_stmt_fetch()调用能返回缓冲数据
        ret = mysql_stmt_store_result(stmt);
        if(ret != 0) {
            break;
        }
        while (mysql_stmt_fetch(stmt) != MYSQL_NO_DATA) {
            shared_ptr<DataRowType> pRow(new DataRowType);
            for(int i = 0; i < m_nColumnCount; i++)
            {
                pRow->insert(make_pair(i, buf[i]));
            }
            m_data.push_back(pRow);
        }
        for(int i = 0; i<m_nColumnCount; ++i) {
            SFree(buf[i]);
        }
        SFree(buf);
        free(sqlbind);
        m_pQuery = mysql_stmt_result_metadata(stmt);
        ret = m_nRowCount = (int)mysql_stmt_num_rows(stmt);
    } while(0);
    return ret;
}

void CDBMysqlResultSet::clear()
{
    if (m_pQuery != nullptr) {
        mysql_free_result(m_pQuery);
        m_pQuery    = nullptr;
    }
    m_curRow        = nullptr;
    m_data.clear();
    m_nRowCount     = 0;
    m_nRowIndex     = -1;
    m_nColumnCount  = 0;
    m_isSTMT        = 0;
}

int CDBMysqlResultSet::getFieldIdByName(const string& fieldName)
{
    if (m_pQuery == nullptr)
    {
        return DB_GET_FIELDS_ERROR;
    }
    for (int i=0; i<m_nColumnCount; i++)
    {
        MYSQL_FIELD *pField = mysql_fetch_field_direct(m_pQuery, i);
        if(pField == nullptr) {
            continue;
        }
        if(fieldName.compare(pField->name) == 0) {
            return i;
        }
    }
    return DB_NEXT_ROW_ERROR;
}

bool CDBMysqlResultSet::next()
{
    if(m_isSTMT == ESqlExecModeQuery) {
        return getNextRow();
    }
    if (!m_nRowCount)
    {
        return false;
    }
    ++m_nRowIndex;
    bool ret = m_nRowIndex < m_nRowCount ? true : false;
    if(ret == false) {
        return ret;
    }
    m_pTmp = m_data[m_nRowIndex];
    return ret;
}

int CDBMysqlResultSet::getField(const int fieldId, int& value)
{
    if(m_isSTMT == ESqlExecModeQuery) {
        return getValue(fieldId,  value);
    }
    if (!m_nRowCount || fieldId>=m_nColumnCount)
    {
        return DB_GET_FIELDS_ERROR;
    }
    string pField = (*m_pTmp)[fieldId];
    if (pField.empty())
    {
        value = 0;
        return DB_NEXT_ROW_ERROR;
    }
    value = atoi(pField.c_str());
    return SUCCESS;
}

int CDBMysqlResultSet::getField(const int fieldId, string& value)
{
    // 如果时采用直接执行sql方式查询
    if(m_isSTMT == ESqlExecModeQuery) {
        return getValue(fieldId,  value);
    }
    if (!m_nRowCount || fieldId>=m_nColumnCount)
    {
        return DB_GET_FIELDS_ERROR;
    }
    value = (*m_pTmp)[fieldId];
    return SUCCESS;
}

int CDBMysqlResultSet::getField(const int fieldId, QText& obj)
{
    if(m_isSTMT == ESqlExecModeQuery) {
        return getValue(fieldId,  obj);
    }
    char* value = obj.m_data;
    size_t len  = obj.m_len;
    if (!m_nRowCount || fieldId>=m_nColumnCount || value == nullptr || len == 0)
    {
        return DB_GET_FIELDS_ERROR;
    }
    string pField = (*m_pTmp)[fieldId];
    size_t length = pField.length();
    memset(value, 0, len);
    if(len > length) {
        memcpy(value, pField.c_str(), length);
    } else {
        memcpy(value, pField.c_str(), len - 1);
    }
    return SUCCESS;
}

int CDBMysqlResultSet::getField(const int fieldId, bool& value)
{
    if(m_isSTMT == ESqlExecModeQuery) {
        return getValue(fieldId,  value);
    }
    if (!m_nRowCount || fieldId>=m_nColumnCount)
    {
        return DB_GET_FIELDS_ERROR;
    }
    string pField = (*m_pTmp)[fieldId];
    if (pField.empty())
    {
        value = false;
        return DB_NEXT_ROW_ERROR;
    }
    value = atoi(pField.c_str()) != 0;
    return SUCCESS;
}

int CDBMysqlResultSet::getField(const int fieldId, float& value)
{
    if(m_isSTMT == ESqlExecModeQuery) {
        return getValue(fieldId,  value);
    }
    if (!m_nRowCount || fieldId>=m_nColumnCount)
    {
        return DB_GET_FIELDS_ERROR;
    }
    string pField = (*m_pTmp)[fieldId];
    if (pField.empty())
    {
        value = 0.0;
        return DB_NEXT_ROW_ERROR;
    }
    value = (float)atof(pField.c_str());
    return SUCCESS;
}

int CDBMysqlResultSet::getField(const int fieldId, double& value)
{
    if(m_isSTMT == ESqlExecModeQuery) {
        return getValue(fieldId,  value);
    }
    if (!m_nRowCount || fieldId>=m_nColumnCount)
    {
        return DB_GET_FIELDS_ERROR;
    }
    string pField = (*m_pTmp)[fieldId];
    if (pField.empty())
    {
        value = 0.0;
        return DB_NEXT_ROW_ERROR;
    }
    value = atof(pField.c_str());
    return SUCCESS;
}

int CDBMysqlResultSet::getField(const int fieldId, time_t& value)
{
    if(m_isSTMT == ESqlExecModeQuery) {
        return getValue(fieldId,  value);
    }
    if (!m_nRowCount || fieldId>=m_nColumnCount)
    {
        return DB_GET_FIELDS_ERROR;
    }
    string pField = (*m_pTmp)[fieldId];
    tm time;
    memset(&time, 0, sizeof(time));
    if (pField.empty())
    {
        value = 0;
        return DB_NEXT_ROW_ERROR;
    }
    sscanf(pField.c_str(), "%d-%d-%d %d:%d:%d", &time.tm_year, &time.tm_mon, &time.tm_mday, &time.tm_hour, &time.tm_min, &time.tm_sec);
    time.tm_year -= 1900;
    time.tm_mon  -= 1;
    value = mktime(&time);
    return SUCCESS;
}

bool CDBMysqlResultSet::getNextRow()
{
    if (m_pQuery == nullptr)
    {
        return false;
    }
    m_curRow = mysql_fetch_row(m_pQuery);
    return m_curRow != nullptr ? true: false;
}

int CDBMysqlResultSet::getValue(const int fieldId, int& value)
{
    if (m_curRow == nullptr || fieldId>=m_nColumnCount)
    {
        return DB_GET_FIELDS_ERROR;
    }
    char* pField = m_curRow[fieldId];
    if (pField == nullptr || pField[0] == 0)
    {
        return SUCCESS;
    }
    value = atoi(pField);
    return SUCCESS;
}

int CDBMysqlResultSet::getValue(const int fieldId, string& value)
{
    if (m_curRow == nullptr || fieldId>=m_nColumnCount)
    {
        return DB_GET_FIELDS_ERROR;
    }
    char* pField = m_curRow[fieldId];
    if (pField == nullptr || pField[0] == 0)
    {
        return SUCCESS;
    }
    value = pField;
    return SUCCESS;
}

int CDBMysqlResultSet::getValue(const int fieldId, QText& obj)
{
    char* value = obj.m_data;
    size_t len  = obj.m_len;
    if (m_curRow == nullptr || fieldId>=m_nColumnCount || value == nullptr || len == 0)
    {
        return DB_GET_FIELDS_ERROR;
    }
    if (m_curRow[fieldId] == nullptr)
    {
        value[0] = '\0';
    }
    else
    {
        strncpy(value, m_curRow[fieldId], len-1);
        value[len-1] = '\0';
    }
    return SUCCESS;
}

int CDBMysqlResultSet::getValue(const int fieldId, bool& value)
{
    if (m_curRow == nullptr || fieldId>=m_nColumnCount)
    {
        return DB_GET_FIELDS_ERROR;
    }
    char* pField = m_curRow[fieldId];
    if (pField == nullptr || pField[0] == 0)
    {
        return SUCCESS;
    }
    value = atoi(pField) != 0;
    return SUCCESS;
}

int CDBMysqlResultSet::getValue(const int fieldId, float& value)
{
    if (m_curRow == nullptr || fieldId>=m_nColumnCount)
    {
        return DB_GET_FIELDS_ERROR;
    }
    char* pField = m_curRow[fieldId];
    if (pField == nullptr || pField[0] == 0)
    {
        return SUCCESS;
    }
    value = (float)atof(pField);
    return SUCCESS;
}

int CDBMysqlResultSet::getValue(const int fieldId, double& value)
{
    if (m_curRow == nullptr || fieldId>=m_nColumnCount)
    {
        return DB_GET_FIELDS_ERROR;
    }
    char* pField = m_curRow[fieldId];
    if (pField == nullptr || pField[0] == 0)
    {
        return SUCCESS;
    }
    value = atof(pField);
    return SUCCESS;
}

int CDBMysqlResultSet::getValue(const int fieldId, time_t& value)
{
    if (m_curRow == nullptr || fieldId>=m_nColumnCount)
    {
        return DB_GET_FIELDS_ERROR;
    }
    char* pField = m_curRow[fieldId];
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
