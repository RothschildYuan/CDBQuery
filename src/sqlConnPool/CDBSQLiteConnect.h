#ifndef CDBSQLITECONNECT_H
#define CDBSQLITECONNECT_H

#include "CDBConnect.h"
#include "CDBSQLiteResultSet.h"

#ifdef ORM_OF_SQLITE

template <typename T>
struct is_string : std::false_type {};

template <typename charT, typename traits, typename Alloc>
struct is_string<std::basic_string<charT, traits, Alloc> > : std::true_type {};

class ACT_CPP_DLL_API CDBSQLiteConnect : public CDBConnect
{
    CDBSQLiteConnect(const CDBSQLiteConnect& );
    CDBSQLiteConnect& operator=(const CDBSQLiteConnect& );
public:
    CDBSQLiteConnect();
    virtual ~CDBSQLiteConnect();

    /**
     * @brief connect       创建数据库连接
     * @param host          主机地址
     * @param user          用户名
     * @param password      密码
     * @param db            连接数据库实例名
     * @param port          端口号
     * @return              0 成功，其他失败
     */
    int connect(const char* host = nullptr, const char* user = nullptr, const char* password = nullptr, const char* db = nullptr, int port = 3306);
    /**
     * @brief disconnect    断开数据库连接
     */
    void disconnect() override;

    /**
     * @brief disconnect    分页查询数据库总条数
     */
    int pagingCount() override;

    /**
     * @brief clearQuery    初始化连接的内容
     */
    void clearQuery() override;

    /**
     * @brief connctionTest 测试连接是否断开
     * @return              0 成功，其他失败
     */
    int connctionTest() override;

    /**
     * @brief excuteQuery   执行查询 sql 语句(有反回内容)
     * @param szSql         sql 语句
     * @return              nullptr 失败，其他成功
     */
    int excuteQuery(const char *szSql) override;

    /**
     * @brief excuteSql     执行查询 sql 语句(无反回内容)
     * @param szSql         sql 语句
     * @return              0 成功，其他失败
     */
    int excuteSql(const char *szSql) override;

    /**
     * @brief getLastErrorMsg 获取 mysql 连接中出现的错误信息
     * @return
     */
    const char* getLastErrorMsg() override;

    unsigned long escapeRealToString(  char *pDst, const char *pSrc, unsigned long ulSrcLen ) override;

    ///////////////////////////////////////////////////////////////////////////////////////////////

    /**
     * @brief prepare       mysql 预处理函数
     * @param query         预处理 sql 语句
     * @return              0 成功，其他失败
     */
    int prepare(const string& query) override;

    /**
     * @brief addBindValue  用于为SQL语句中的参数标记符绑定数据
     * @param args          可变模板参数，替代 ？ 占位符
     * @return              0 成功，其他失败
     */
    template<typename... Args>
    int addBindValues(const Args& ... args) {
        if(m_dbHandle == nullptr) {
            return -1;
        }
        return bindVal(0, args...);
    }

    /**
     * @brief exec          执行sql，不带返回结果, 如insert,update,delete等
     * @param sql           sql 语句(不带预计处理)， 如果为 null 则执行 mysql 预处理
     * @return              insert 则返回插入自增 id， update/delete 返回影响数据量 > 0 成功，其他失败
     */
    int exec(const string& sql = "") override;

    /**
     * @brief query         执行sql，带返回结果, 如 select 等
     * @param sql           sql 语句(不带预计处理)， 如果为 null 则执行 mysql 预处理
     * @return              nullptr 失败，其他成功
     */
    int query(const string& sql = "") override;

    /**
     * @brief isEnd 用于遍历赋值查询到的数据判断是否已经没有下一条数据了
     * @return      true：表示没有下一条数据了，false：表示还有下一条数据
     */
    virtual bool isEnd() override;

    /**
     * @brief size  查询到的数据条数
     * @return      < 0 失败，其他成功
     */
    virtual int  size() override;

    /**
     * @brief next  用于遍历赋值查询到的数据
     * @return      true 表示还有下一条数据，false，表示没有下一条数据了
     */
    virtual bool next() override;

    /**
     * @brief value     通过 字段名 获取数据库值，
     * @param fieldName 数据库存储中 字段名
     * @return 返回成员值  尾部添加 toString，toInt, toFloat, toDouble，toBool,
     */
    virtual CDBObject value(const string& fieldName) override;

    /**
     * @brief getValue 通过 字段名 获取数据库值
     * @param fieldName 数据库存储中 字段名
     * @return 返回成员值（仅用于 set 赋值方式，set 赋值什么类型，返回什么类型）
     */
    virtual CDBResultSet::CDBResult getValue(const string& fieldName) override;

    /**
     * @brief getValue  通过 字段编号 获取数据库值
     * @param fieldId   字段编号
     * @return 返回成员值（仅用于 set 赋值方式，set 赋值什么类型，返回什么类型）
     */
    virtual CDBResultSet::CDBResult getValue(const int& fieldId) override;

    /**
     * @brief getValue 通过 字段编号 获取数据库值
     * @param fieldId  数据库存储中 字段编号
     * @param value    需要赋值的对象（支持的数据类型，int，float，double，string, bool）
     * @return 0 成功，其他失败
     */
    virtual int  getValue(const int fieldId, int& value)     override;
    virtual int  getValue(const int fieldId, string& value)  override;
    virtual int  getValue(const int fieldId, bool& value)    override;
    virtual int  getValue(const int fieldId, float& value)   override;
    virtual int  getValue(const int fieldId, double& value)  override;
    virtual int  getValue(const int fieldId, time_t& value)  override;

    /**
     * @brief getValue 通过 字段名 获取数据库值
     * @param fieldName 数据库存储中 字段名
     * @param value     需要赋值的对象
     * @return 0 成功，其他失败
     */
    virtual int  getValue(const string& fieldName, int& value)       override;
    virtual int  getValue(const string& fieldName, string& value)    override;
    virtual int  getValue(const string& fieldName, bool& value)      override;
    virtual int  getValue(const string& fieldName, float& value)     override;
    virtual int  getValue(const string& fieldName, double& value)    override;
    virtual int  getValue(const string& fieldName, time_t& value)    override;

private:

    template<typename _T, typename... Args>
    int bindVal(const int& index, const _T& t, const Args& ... args) {
        int ret = bindValue(index, t);
        if(ret != 0) {
            std::cout<<"bindValue error ret: " << ret<<std::endl;
            return -1;
        }
        return bindVal(index + 1, args...);
    }

    template<typename _T>
    int bindVal(const int& index, const _T& t) {
        return bindValue(index, t);
    }

public:
    /**
     * @brief addBindValue  用于为SQL语句中的参数标记符绑定数据
     * @param index         表示绑定第几个占位符（如果总共8个占位符，index值为 0 ～ 7）
     * @param value         可变模板参数，替代 ？ 占位符
     * @return              0 成功，其他失败
     */
    int addBindValue(const int& index, const int& value)    override;
    int addBindValue(const int& index, const double& value) override;
    int addBindValue(const int& index, const string& value) override;
    int addBindValue(const int& index, const char* value)   override;

private:
    sqlite3*            m_dbHandle;     //
    sqlite3_stmt*       m_statement;    //

    CDBSQLiteResultSet  m_resultSet;    //
    char*               m_errMsg;       //

    string              m_sql;          // 缓存 sql 语句
};

#endif

#endif // CDBSQLITECONNECT_H
