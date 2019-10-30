#include "CDBObject.h"

#include <string.h>

#include "config.h"

CDBObject::CDBObject()
{

}

CDBObject::CDBObject(const char *data) {
    m_data = data;
}

CDBObject::CDBObject(const string &data) {
    m_data = data;
}

CDBObject::CDBObject(const CDBObject &data) {
    m_data = data.m_data;
}

CDBObject &CDBObject::operator =(const string &data) {
    m_data = data;
    return *this;
}

Int4 CDBObject::toInt()
{
    return atoi(m_data.c_str());
}

Int8 CDBObject::toInt8()
{
    return atol(m_data.c_str());
}

UInt4 CDBObject::toUInt()
{
    return (UInt4)atoi(m_data.c_str());
}

UInt8 CDBObject::toUInt8()
{
    return (UInt8)atol(m_data.c_str());
}

string &CDBObject::toString() {
    return m_data;
}

bool CDBObject::toBool() {
    return atoi(m_data.c_str()) != 0;
}

float CDBObject::toFloat() {
    return (float)atof(m_data.c_str());
}

double CDBObject::toDouble() {
    return atof(m_data.c_str());
}

time_t CDBObject::toTime() {
    tm time;
    memset(&time, 0, sizeof(time));
    if (m_data.empty())
    {
        return 0;
    }
    sscanf(m_data.c_str(), "%d-%d-%d %d:%d:%d", &time.tm_year, &time.tm_mon, &time.tm_mday, &time.tm_hour, &time.tm_min, &time.tm_sec);
    time.tm_year -= 1900;
    time.tm_mon  -= 1;
    return mktime(&time);
}

CDBObject &CDBObject::operator =(const CDBObject &data) {
    m_data = data.m_data;
    return *this;
}

CDBObject &CDBObject::operator =(const char *data) {
    m_data = data;
    return *this;
}
