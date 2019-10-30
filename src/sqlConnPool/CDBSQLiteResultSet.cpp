#include "CDBSQLiteResultSet.h"

#include <exception>
#include "statuscode.h"
#include "common.h"

#ifdef ORM_OF_SQLITE

CDBSQLiteResultSet::CDBSQLiteResultSet():CDBResultSet()
{
    m_data.clear();
    m_keyIndex.clear();
    m_pTmp = nullptr;
}

CDBSQLiteResultSet::~CDBSQLiteResultSet()
{
    clear();
}

void CDBSQLiteResultSet::reset(sqlite3_stmt* statement) {
    if(statement == nullptr) {
        m_nColumnCount  = m_keyIndex.size();
    } else {
        m_nColumnCount = sqlite3_column_count(statement);
        //获取字段名和字段类型
        for(int i = 0; i < m_nColumnCount; i++)
        {
            m_keyIndex.insert(make_pair(sqlite3_column_name(statement, i), i));
        }
        int ret = SQLITE_ROW;
        while (ret == SQLITE_ROW) {

            shared_ptr<DataRowType> pRow(new DataRowType);
            for(int i = 0; i < m_nColumnCount; i++)
            {
                string tmp = (const char*)sqlite3_column_text(statement,i);
                pRow->insert(make_pair(i, tmp));
            }
            m_data.push_back(pRow);

            ret = sqlite3_step(statement);
        }
    }
    m_nRowCount = m_data.size();
    m_isSTMT = ESqlExecModePrepare;

    // printf("column name:%s\n",sqlite3_column_name(stmt, i));
    // printf("column type:%i\n",sqlite3_column_type(stmt, i));
    // sqlite3_step(statement) == SQLITE_ROW
}

bool CDBSQLiteResultSet::isEnd()
{
    if (!m_nRowCount)
    {
        return false;
    }
    return m_nRowIndex < m_nRowCount;
}

void CDBSQLiteResultSet::clear()
{
    m_data.clear();
    m_keyIndex.clear();
    m_nRowCount      = 0;
    m_nRowIndex      = -1;
    m_nColumnCount   = 0;
    m_isSTMT         = 0;
}

int CDBSQLiteResultSet::getFieldIdByName(const string& fieldName)
{
    if (!m_nRowCount)
    {
        return DB_GET_FIELDS_ERROR;
    }
    auto it = m_keyIndex.find(fieldName);
    if(it == m_keyIndex.end()) {
        return DB_NEXT_ROW_ERROR;
    }
    return it->second;
}

bool CDBSQLiteResultSet::next()
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

int CDBSQLiteResultSet::getField(const int fieldId, int& value)
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
        return SUCCESS;
    }
    value = atoi(pField.c_str());
    return SUCCESS;
}

int CDBSQLiteResultSet::getField(const int fieldId, string& value)
{
    if(m_isSTMT == ESqlExecModeQuery) {
        return getValue(fieldId,  value);
    }
    if (!m_nRowCount || fieldId>=m_nColumnCount)
    {
        return DB_GET_FIELDS_ERROR;
    }
#ifdef __linux__
    value = (*m_pTmp)[fieldId];
#else
    value = UTF8_2_GBK((*m_pTmp)[fieldId]);
#endif
    return SUCCESS;
}

int CDBSQLiteResultSet::getField(const int fieldId, QText& obj)
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
#ifdef __linux__
    string pField = (*m_pTmp)[fieldId];
#else
    string pField = UTF8_2_GBK((*m_pTmp)[fieldId]);
#endif
    size_t length = pField.length();
    memset(value, 0, len);
    if(len > length) {
        memcpy(value, pField.c_str(), length);
    } else {
        memcpy(value, pField.c_str(), len - 1);
    }
    return SUCCESS;
}

int CDBSQLiteResultSet::getField(const int fieldId, bool& value)
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
        return SUCCESS;
    }
    value = atoi(pField.c_str()) != 0;
    return SUCCESS;
}

int CDBSQLiteResultSet::getField(const int fieldId, float& value)
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
        return SUCCESS;
    }
    value = (float)atof(pField.c_str());
    return SUCCESS;
}

int CDBSQLiteResultSet::getField(const int fieldId, double& value)
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
        return SUCCESS;
    }
    value = atof(pField.c_str());
    return SUCCESS;
}

int CDBSQLiteResultSet::getField(const int fieldId, time_t& value)
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
        return SUCCESS;
    }
    sscanf(pField.c_str(), "%d-%d-%d %d:%d:%d", &time.tm_year, &time.tm_mon, &time.tm_mday, &time.tm_hour, &time.tm_min, &time.tm_sec);
    time.tm_year -= 1900;
    time.tm_mon  -= 1;
    value = mktime(&time);
    return SUCCESS;
}

#endif
