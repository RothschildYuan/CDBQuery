#include "CDBResultSet.h"

#include "common.h"
#include "statuscode.h"

CDBResultSet::CDBResultSet()
{
    m_nRowCount      = 0;
    m_nRowIndex      = -1;
    m_nColumnCount   = 0;
    m_isSTMT         = 0;
}

CDBResultSet::~CDBResultSet()
{

}

int CDBResultSet::size()
{
    return m_nRowCount;
}

template<typename _T>
int CDBResultSet::getField(const string &fieldName, _T &value)
{
    if(m_isSTMT == 2) {
        return getValue(fieldName,  value);
    }
    int id = getFieldIdByName(fieldName);
    if (id>= 0) {
        return getField(id, value);
    }
    return DB_NEXT_ROW_ERROR;
}

template int CDBResultSet::getField<int>(const string& fieldName, int& value);
template int CDBResultSet::getField<string>(const string& fieldName, string& value);
template int CDBResultSet::getField<QText>(const string& fieldName, QText& value);
template int CDBResultSet::getField<bool>(const string& fieldName, bool& value);
template int CDBResultSet::getField<float>(const string& fieldName, float& value);
template int CDBResultSet::getField<double>(const string& fieldName, double& value);
template int CDBResultSet::getField<time_t>(const string& fieldName, time_t& value);

CDBResultSet::CDBResult CDBResultSet::getValueRes(const int& fieldId) {
    return CDBResult(this, fieldId);
}

CDBResultSet::CDBResult CDBResultSet::getValueRes(const string &fieldName) {
    int id = getFieldIdByName(fieldName);
    return CDBResult(this, id);
}

int CDBResultSet::getValueInt(const int fieldId) {
    int value;
    getField(fieldId, value);
    return value;
}

string CDBResultSet::getValueString(const int fieldId) {
    string value;
    getField(fieldId, value);
    return value;
}

QText CDBResultSet::getValueQText(const int fieldId) {
    QText value;
    getField(fieldId, value);
    return value;
}

bool CDBResultSet::getValueBool(const int fieldId) {
    bool value;
    getField(fieldId, value);
    return value;
}

float CDBResultSet::getValueFloat(const int fieldId) {
    float value;
    getField(fieldId, value);
    return value;
}

double CDBResultSet::getValueDouble(const int fieldId) {
    double value;
    getField(fieldId, value);
    return value;
}

time_t CDBResultSet::getValueTime_t(const int fieldId) {
    time_t value;
    getField(fieldId, value);
    return value;
}

template<typename _T>
int CDBResultSet::getValue(const string& fieldName, _T& value) {
    int id = getFieldIdByName(fieldName);
    if (id >= 0) {
        return getValue(id, value);
    }
    return DB_NEXT_ROW_ERROR;
}

template int CDBResultSet::getValue<int>(const string& fieldName, int& value);
template int CDBResultSet::getValue<string>(const string& fieldName, string& value);
template int CDBResultSet::getValue<QText>(const string& fieldName, QText& value);
template int CDBResultSet::getValue<bool>(const string& fieldName, bool& value);
template int CDBResultSet::getValue<float>(const string& fieldName, float& value);
template int CDBResultSet::getValue<double>(const string& fieldName, double& value);
template int CDBResultSet::getValue<time_t>(const string& fieldName, time_t& value);
