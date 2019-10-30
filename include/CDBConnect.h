#ifndef CDBCONNECT_H
#define CDBCONNECT_H

#include "CDBResultSet.h"
#include "CDBObject.h"

class ACT_CPP_DLL_API CDBConnect
{
    CDBConnect(const CDBConnect& );
    CDBConnect& operator=(const CDBConnect& );
public:
    CDBConnect();
    virtual ~CDBConnect();

    /**
     * @brief begin 事物操作封装（开始事物，回滚，提交）
     * @return      0 成功，其他失败
     */
    virtual int begin() final;
    virtual int rollback() final;
    virtual int commit() final;

    /**
     * @brief connect       创建数据库连接
     * @param host          主机地址
     * @param user          用户名
     * @param password      密码
     * @param db            连接数据库实例名
     * @param port          端口号
     * @return              0 成功，其他失败
     */
    virtual int connect(const char* host, const char* user, const char* password, const char* db, int port) = 0;
    virtual int connect(const string& host, const string& user, const string& password, const string& db, const int& port) final;

    /**
     * @brief disconnect    断开数据库连接
     */
    virtual void disconnect() = 0;

    /**
     * @brief startPageQuery 开启分页查询
     */
    virtual void startPageQuery() {}

    /**
     * @brief stopPageQuery 停止分页查询
     */
    virtual void stopPageQuery() {}

    /**
     * @brief disconnect    分页查询数据库总条数
     */
    virtual int pagingCount() = 0;

    /**
     * @brief clearQuery    初始化连接的内容
     */
    virtual void clearQuery() = 0;

    /**
     * @brief connctionTest 测试连接是否断开
     * @return              0 成功，其他失败
     */
    virtual int connctionTest() = 0;

    /**
     * @brief excuteQuery   执行查询 sql 语句(有反回内容)
     * @param szSql         sql 语句
     * @return              nullptr 失败，其他成功
     */
    virtual int excuteQuery(const char *szSql) = 0;

    /**
     * @brief excuteSql     执行查询 sql 语句(无反回内容)
     * @param szSql         sql 语句
     * @return              0 成功，其他失败
     */
    virtual int excuteSql(const char *szSql) = 0;

    /**
     * @brief getLastErrorMsg 获取 mysql 连接中出现的错误信息
     * @return
     */
    virtual const char* getLastErrorMsg() = 0;

    virtual unsigned long escapeRealToString(char *pDst, const char *pSrc, unsigned long ulSrcLen ) = 0;

    /**
     * @brief prepare       mysql 预处理函数
     * @param query         预处理 sql 语句
     * @return              0 成功，其他失败
     */
    virtual int prepare(const string& query) = 0;

    /**
     * @brief exec  执行sql，不带返回结果, 如insert,update,delete等
     * @param sql   sql 语句(不带预计处理)， 如果为 null 则执行 mysql 预处理
     * @return      insert 则返回插入自增 id， update/delete 返回影响数据量 > 0 成功，其他失败
     */
    virtual int exec(const string& sql = "")   = 0;

    /**
     * @brief query 执行sql，带返回结果, 如 select 等
     * @param sql   sql 语句(不带预计处理)， 如果为 null 则执行 mysql 预处理
     * @return      >=0 成功，其他失败, 返回查询到的数据条数
     */
    virtual int query(const string&  sql = "")  = 0;

    /**
     * @brief isEnd 用于遍历赋值查询到的数据判断是否已经没有下一条数据了
     * @return      true：表示没有下一条数据了，false：表示还有下一条数据
     */
    virtual bool isEnd()    = 0;

    /**
     * @brief size  查询到的数据条数
     * @return      < 0 失败，其他成功
     */
    virtual int  size()     = 0;

    /**
     * @brief next  用于遍历赋值查询到的数据
     * @return      true 表示还有下一条数据，false，表示没有下一条数据了
     */
    virtual bool next()     = 0;

    /**
     * @brief value     通过 字段名 获取数据库值，
     * @param fieldName 数据库存储中 字段名
     * @return 返回成员值  尾部添加 toString，toInt, toFloat, toDouble，toBool,
     */
    virtual CDBObject value(const string& fieldName)                    = 0;

    /**
     * @brief getValue 通过 字段名 获取数据库值
     * @param fieldName 数据库存储中 字段名
     * @return 返回成员值（仅用于 set 赋值方式，set 赋值什么类型，返回什么类型）
     */
    virtual CDBResultSet::CDBResult getValue(const string& fieldName)   = 0;

    /**
     * @brief getValue  通过 字段编号 获取数据库值
     * @param fieldId   字段编号
     * @return 返回成员值（仅用于 set 赋值方式，set 赋值什么类型，返回什么类型）
     */
    virtual CDBResultSet::CDBResult getValue(const int& fieldId)        = 0;

    /**
     * @brief getValue 通过 字段编号 获取数据库值
     * @param fieldId  数据库存储中 字段编号
     * @param value    需要赋值的对象（支持的数据类型，int，float，double，string, bool）
     * @return 0 成功，其他失败
     */
    virtual int  getValue(const int fieldId, int& value)                = 0;
    virtual int  getValue(const int fieldId, string& value)             = 0;
    virtual int  getValue(const int fieldId, bool& value)               = 0;
    virtual int  getValue(const int fieldId, float& value)              = 0;
    virtual int  getValue(const int fieldId, double& value)             = 0;
    virtual int  getValue(const int fieldId, time_t& value)             = 0;

    /**
     * @brief getValue 通过 字段名 获取数据库值
     * @param fieldName 数据库存储中 字段名
     * @param value     需要赋值的对象
     * @return 0 成功，其他失败
     */
    virtual int  getValue(const string& fieldName, int& value)          = 0;
    virtual int  getValue(const string& fieldName, string& value)       = 0;
    virtual int  getValue(const string& fieldName, bool& value)         = 0;
    virtual int  getValue(const string& fieldName, float& value)        = 0;
    virtual int  getValue(const string& fieldName, double& value)       = 0;
    virtual int  getValue(const string& fieldName, time_t& value)       = 0;

    virtual int addBindValue(const int& index, const int& value)        = 0;
    virtual int addBindValue(const int& index, const double& value)     = 0;
    virtual int addBindValue(const int& index, const string& value)     = 0;
    virtual int addBindValue(const int& index, const char* value)       = 0;

    /**
     * @brief addBindValue  用于为SQL语句中的参数标记符绑定数据
     * @param args          可变模板参数，替代 ？ 占位符
     * @return              0 成功，其他失败
     */
    template<typename... Args>
    int addBindValues(const Args& ... args) {
        return bindParamVal(0, args...);
    }

private:

    template <typename _T>
    int bindParamVal(const int index, const _T& t)
    {
        return addBindValue(index, t);
    }

    template <typename _T, typename... Args>
    int bindParamVal(const int index, const _T& t, const Args &... args)
    {
        int ret = addBindValue(index, t);
        if(ret != 0) {
            return ret;
        }
        return bindParamVal(index + 1, args...);
    }
};

#endif // CDBCONNECT_H
