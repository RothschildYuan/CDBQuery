#ifndef CDBOBJECT_H
#define CDBOBJECT_H

#include "configure.h"
#include "stdtype.h"

#ifdef _WIN32

template struct ACT_CPP_DLL_API std::_Simple_types<char>;
template union  ACT_CPP_DLL_API std::_String_val<std::_Simple_types<char>>::_Bxty;
template class  ACT_CPP_DLL_API std::_String_val<std::_Simple_types<char>>;
template struct ACT_CPP_DLL_API std::_Wrap_alloc < std::allocator<char>>;
template class  ACT_CPP_DLL_API std::_Compressed_pair<std::_Wrap_alloc<std::allocator<char>>, std::_String_val<std::_Simple_types<char>>, true>;
template class  ACT_CPP_DLL_API std::allocator<char>;
template struct ACT_CPP_DLL_API std::char_traits<char>;
template class  ACT_CPP_DLL_API std::basic_string<char, std::char_traits<char>, std::allocator<char>>;

#endif

class ACT_CPP_DLL_API CDBObject
{
public:
    CDBObject();
    CDBObject(const char* data);
    CDBObject(const string& data);
    CDBObject(const CDBObject& data);
    CDBObject& operator =(const CDBObject& data);
    CDBObject& operator =(const char* data);
    CDBObject& operator =(const string& data);

    Int4 toInt();
    Int8 toInt8();
    UInt4 toUInt();
    UInt8 toUInt8();
    string &toString();
    bool toBool();
    float toFloat();
    double toDouble();
    time_t toTime();

private:
    string m_data;
};

#endif // CDBOBJECT_H
