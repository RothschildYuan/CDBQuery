#ifndef CDBQUERY_H
#define CDBQUERY_H

#include "CDBConnPool.h"

class CDBPool;

class ACT_CPP_DLL_API CDBQuery
{
    CDBQuery(const CDBQuery&);
    CDBQuery& operator=(const CDBQuery&);
public:
    /**
     * @brief CDBQuery  数据库操作对象启用的是默认数据库连接池
     */
    CDBQuery();
    /**
     * @brief CDBQuery  数据库操作对象启用的是指定名称的数据库连接池
     * @param dbname    数据库连接池的名称（设置数据库连接池时可自定义连接池名称）
     */
    CDBQuery(const string& dbname);
    /**
     * @brief CDBQuery  数据库操作对象启用的是指定数据库连接池
     * @param pool      数据库连接池
     */
    CDBQuery(const CDBPool* pool);
    ~CDBQuery();

    /**
     * @brief begin 事物操作封装（开始事物，回滚，提交）
     * @return      0 成功，其他失败
     */
    int begin();
    int rollback();
    int commit();

    /**
     * @brief startPageQuery 开启分页查询, 查询一次后，分页查询设置失效，第二次分页查询需要重新设置（必须在执行前确定是否需要分页查询）
     */
    void startPageQuery();

    /**
     * @brief stopPageQuery 停止分页查询
     */
    void stopPageQuery();

    /**
     * @brief disconnect    分页查询数据库总条数
     */
    int pagingCount();

    /**
     * @brief connctionTest 测试连接是否断开
     * @return              0 成功，其他失败
     */
    int connctionTest();

    /**
     * @brief getLastErrorMsg 获取 mysql 连接中出现的错误信息
     * @return
     */
    const char* getLastErrorMsg();

    /***************************** prepare ****************************/

    /**
     * @brief prepare   mysql 预处理函数
     * @param query     预处理 sql 语句
     * @return          0 成功，其他失败
     */
    int prepare(const string& query);

    /**
     * @brief addBindValue  用于为SQL语句中的参数标记符绑定数据
     * @param args          可变模板参数，替代 ？ 占位符
     * @return              0 成功，其他失败
     */
    template<typename... Args>
    int addBindValues(const Args& ... args) {
        return m_dbConn->addBindValues(args...);
    }

    /**
     * @brief addBindValue  用于为SQL语句中的参数标记符绑定数据
     * @param index         表示绑定第几个占位符（如果总共8个占位符，index值为 0 ～ 7）
     * @param value         可变模板参数，替代 ？ 占位符
     * @return              0 成功，其他失败
     */
    int addBindValue(const int& index, const int& value);
    int addBindValue(const int& index, const double& value);
    int addBindValue(const int& index, const string& value);
    int addBindValue(const int& index, const char* value);

    /**
     * @brief exec  执行sql，不带返回结果, 如insert,update,delete等
     * @param sql   sql 语句(不带预计处理)， 如果为 null 则执行 mysql 预处理
     * @return      insert 则返回插入自增 id， update/delete 返回影响数据量 > 0 成功，其他失败
     */
    int exec(const string&  sql = "");

    /**
     * @brief query 执行sql，带返回结果, 如 select 等
     * @param sql   sql 语句(不带预计处理)， 如果为 null 则执行 mysql 预处理
     * @return      >=0 成功，其他失败, 返回查询到的数据条数
     */
    int query(const string&  sql = "");

    /**
     * @brief value     通过 字段名 获取数据库值，
     * @param fieldName 数据库存储中 字段名
     * @return 返回成员值  尾部添加 toString，toInt, toFloat, toDouble，toBool,
     */
    CDBObject value(const string& fieldName);

    /**
     * @brief getValue 通过 字段名 获取数据库值
     * @param fieldName 数据库存储中 字段名
     * @return 返回成员值（仅用于 set 赋值方式，set 赋值什么类型，返回什么类型）
     */
    CDBResultSet::CDBResult getValue(const string& fieldName);

    /**
     * @brief getValue  通过 字段编号 获取数据库值
     * @param fieldId   字段编号
     * @return 返回成员值（仅用于 set 赋值方式，set 赋值什么类型，返回什么类型）
     */
    CDBResultSet::CDBResult getValue(const int& fieldId);

    /**
     * @brief next  用于遍历赋值查询到的数据
     * @return      true 表示还有下一条数据，false，表示没有下一条数据了
     */
    bool next();

    /**
     * @brief size  查询到的数据条数
     * @return      < 0 失败，其他成功
     */
    int size();

    /**
     * @brief getValue 通过 字段编号 获取数据库值
     * @param fieldId  数据库存储中 字段编号
     * @param value    需要赋值的对象（支持的数据类型，int，float，double，string, bool）
     * @return 0 成功，其他失败
     */
    template<typename _T>
    int getValue(const int fieldId, _T &value)
    {
        return m_dbConn->getValue(fieldId, value);
    }

    /**
     * @brief getValue 通过 字段名 获取数据库值
     * @param fieldName 数据库存储中 字段名
     * @param value     需要赋值的对象
     * @return 0 成功，其他失败
     */
    template<typename _T>
    int getValue(const string& fieldName, _T& value)
    {
        return m_dbConn->getValue(fieldName, value);
    }

public:
    CDBConnect*     m_dbConn;
    string          m_keyname;
    CDBConnPool*    m_pool;
};

#endif // CDBQUERY_H
