#ifndef CDBDEFINE_H
#define CDBDEFINE_H

//空类型，statement支持绑定空值
struct NullType{};

// 二进制类型，statement支持绑定二进制

struct blob
{
    const char *pBuf;
    int size;
};

//读取表的范围
struct TableRange
{
    int startIndex;
    int endIndex;
};

struct CommRow
{
    char** parray;
    int size;
};

struct CommTable
{
    CommRow* rows;
    int size;
};

//template <typename T>
//struct is_string : std::false_type {};

//template <typename charT, typename traits, typename Alloc>
//struct is_string<std::basic_string<charT, traits, Alloc> > : std::true_type {};

//template <typename T>
//struct is_nulltype : std::false_type {};

//template <>
//struct is_nulltype < NullType> : std::true_type {};

//template <typename T>
//struct is_blob : std::false_type {};

//template <>
//struct is_blob <blob> : std::true_type {};

//#include <unordered_map>
//#include <type_traits>
//#include <string>
//#include <vector>
//#include <map>
//#include "boost/variant.hpp"

/**
*类型定义，数据库操作要用到的类型，如表、行、字段和值等类型
*/
//typedef boost::variant<double, int, uint32_t, sqlite3_int64, char*, blob, std::string, NullType> Value;	//数据表中的值类型
//typedef std::unordered_map<std::string, Value> Row;	//数据表中的行
//typedef std::vector<Row> Table;	//数据表
//typedef std::unordered_map<std::string, std::string> TableFields; //创建表的表字段（字段名：字段类型）
//typedef std::vector<std::string> Fields; //字段集合

#endif // CDBDEFINE_H
