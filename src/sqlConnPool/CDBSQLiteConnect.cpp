#include "CDBSQLiteConnect.h"

#include "stdtype.h"
#include "config.h"
#include "statuscode.h"

#ifdef ORM_OF_SQLITE

CDBSQLiteConnect::CDBSQLiteConnect()
{
    m_dbHandle  = nullptr;
    m_statement = nullptr;
}

CDBSQLiteConnect::~CDBSQLiteConnect()
{
    disconnect();
}

int CDBSQLiteConnect::connect(const char* , const char* , const char* , const char* db, int )
{
    cout << "Connect \n";
    int ret = sqlite3_open(db, &m_dbHandle);
    if(ret != SQLITE_OK) {
        cout << "DB connect failed\n";
        return ret;
    }
    cout << "DB connect success\n";
    return ret;
}

void CDBSQLiteConnect::disconnect()
{
    if(m_dbHandle != nullptr) {
        sqlite3_close(m_dbHandle);
        m_dbHandle = nullptr;
    }
    if(m_statement != nullptr) {
        sqlite3_finalize(m_statement);
        m_statement = nullptr;
    }
}

int CDBSQLiteConnect::pagingCount()
{
    int num = 0;
    string::size_type beg = m_sql.find("from");
    string::size_type end = m_sql.find("order");
    string substr;
    if(end == string::npos) {
        substr = "select count(*) " + m_sql.substr(beg, end-beg);
    } else {
        substr = "select count(*) " + m_sql.substr(beg, end-beg) + ";";
    }
//    cout<<"m_sql: "<<m_sql<<endl;
//    int ret = excuteQuery(substr.c_str());
//    while (ret->next()) {
//        ret->getField(0, num);
//        break;
//    }
    return num;
}

void CDBSQLiteConnect::clearQuery()
{
    m_resultSet.clear();
}

int CDBSQLiteConnect::connctionTest()
{
    if (m_dbHandle == nullptr)
    {
        return DB_HANDLE_GONE_AWAY_ERROR;
    }
    return SUCCESS;
}

/*
 * sqlite3_get_autocommit(m_dbHandle);
 * sqlite3_commit_hook(m_dbHandle, nullptr, nullptr);
 * sqlite3_rollback_hook(m_dbHandle, nullptr, nullptr);
 */
int callback(void* arg,int column_count,char **column_value,char** column_name)
{
    CDBSQLiteResultSet* pResultSet = (CDBSQLiteResultSet*)arg;
    if(pResultSet->m_keyIndex.size() == 0) {
        for(int i = 0; i < column_count; i++)
        {
            pResultSet->m_keyIndex[column_name[i]] = i;
        }
    }
    shared_ptr<CDBSQLiteResultSet::DataRowType> pRow(new CDBSQLiteResultSet::DataRowType);
    for(int i = 0; i < column_count; i++)
    {
        pRow->insert(make_pair(i, column_value[i]));
    }
    pResultSet->dataAppend(pRow);
    return SUCCESS;
}

int CDBSQLiteConnect::excuteQuery(const char *szSql)
{
    if (m_dbHandle == nullptr)
    {
        return DB_HANDLE_GONE_AWAY_ERROR;
    }
    m_resultSet.clear();
    int ret = sqlite3_exec(
                     m_dbHandle,    /* An open DB */
                     szSql,         /* SQL to be evaluated */
                     callback,      /* Callback function */
                     &m_resultSet,  /* 1st argument to callback */
                     &m_errMsg      /* Error msg written here */
                     );
    if(ret != SQLITE_OK) {
        return DB_QUERY_ERROR;
    }
    m_resultSet.reset();
    return 0;
}

int CDBSQLiteConnect::excuteSql(const char *szSql)
{
    if (m_dbHandle == nullptr)
    {
        return DB_HANDLE_GONE_AWAY_ERROR;
    }
    int ret = sqlite3_exec(
                     m_dbHandle,    /* An open DB */
                     szSql,         /* SQL to be evaluated */
                     nullptr,       /* Callback function */
                     nullptr,       /* 1st argument to callback */
                     &m_errMsg      /* Error msg written here */
                     );
    if(ret != SQLITE_OK) {
        return -1;
    }
    if(strcasestr(szSql, "INSERT") == nullptr) {
        // 操作所影响的记录行数
        ret = sqlite3_changes(m_dbHandle);
    } else {
        // 获取最后插入的数据的自增ID
        ret = (int)sqlite3_last_insert_rowid(m_dbHandle);
    }
    return ret;
}

const char* CDBSQLiteConnect::getLastErrorMsg()
{
    if (m_dbHandle == nullptr)
    {
        return "DB_HANDLE_GONE_AWAY_ERROR";
    }
    return m_errMsg;
}

unsigned long CDBSQLiteConnect::escapeRealToString(  char *pDst, const char *pSrc, unsigned long ulSrcLen )
{
    if (m_dbHandle == nullptr)
    {
        return SUCCESS;
    }
    //这里添加参数检查代码…
    if( ulSrcLen <= 0 )
    {
        //这里添加日志打印语句
        return ulSrcLen;
    }
    (void)pDst;
    (void)pSrc;
    return SUCCESS;
//    return mysql_real_escape_string(m_pMysql,pDst, pSrc, ulSrcLen);
}

int CDBSQLiteConnect::prepare(const string& query)
{
    m_sql = query;
    if (m_dbHandle == nullptr)
    {
        return DB_HANDLE_GONE_AWAY_ERROR;
    }
    return sqlite3_prepare_v2(m_dbHandle, query.data(), query.length(), &m_statement, nullptr);
}

int CDBSQLiteConnect::exec(const string& sql) {
    if(!sql.empty()) {
        return excuteSql(sql.c_str());
    }
    int ret = 0;
    if(m_statement == nullptr) {
        return DB_STMT_GONE_AWAY_ERROR;
    }
    ret = sqlite3_step(m_statement);
    // 判断操作类型 insert（返回 id） 还是 delete/update（影响条数）
    if(strcasestr(m_sql.c_str(), "INSERT") == nullptr) {
        // 获取最后插入的数据的自增ID
        ret = sqlite3_changes(m_dbHandle);
    } else {
        // 操作所影响的记录行数
        ret = (int)sqlite3_last_insert_rowid(m_dbHandle);
    }
    sqlite3_clear_bindings(m_statement);
    sqlite3_reset(m_statement);
    return ret;
}

int CDBSQLiteConnect::query(const string& sql) {
    if(!sql.empty()) {
        return excuteQuery(sql.c_str());
    }
    int ret = 0;
    if(m_statement == nullptr) {
        return DB_STMT_INIT_ERROR;
    }
    ret = sqlite3_step(m_statement);
    if(ret != SQLITE_OK && ret != SQLITE_ROW) {
        printf("\n\n%s\n\n", sqlite3_errmsg(m_dbHandle));
        return DB_QUERY_ERROR;
    }
    //以便后续的mysql_stmt_fetch()调用能返回缓冲数据
    m_resultSet.reset(m_statement);
    return 0;
}

bool CDBSQLiteConnect::isEnd()
{
    return m_resultSet.isEnd();
}

int CDBSQLiteConnect::size()
{
    return m_resultSet.size();
}

bool CDBSQLiteConnect::next()
{
    return m_resultSet.next();
}

CDBObject CDBSQLiteConnect::value(const string &fieldName)
{
    string data;
    int ret = getValue(fieldName, data);
    if(ret < 0) {
        return "";
    }
    return data;
}

CDBResultSet::CDBResult CDBSQLiteConnect::getValue(const string &fieldName)
{
    return m_resultSet.getValueRes(fieldName);
}

CDBResultSet::CDBResult CDBSQLiteConnect::getValue(const int &fieldId)
{
    return m_resultSet.getValueRes(fieldId);
}

int CDBSQLiteConnect::getValue(const int fieldId, int &value)
{
    return m_resultSet.getField(fieldId, value);
}

int CDBSQLiteConnect::getValue(const int fieldId, string &value)
{
    return m_resultSet.getField(fieldId, value);
}

int CDBSQLiteConnect::getValue(const int fieldId, bool &value)
{
    return m_resultSet.getField(fieldId, value);
}

int CDBSQLiteConnect::getValue(const int fieldId, float &value)
{
    return m_resultSet.getField(fieldId, value);
}

int CDBSQLiteConnect::getValue(const int fieldId, double &value)
{
    return m_resultSet.getField(fieldId, value);
}

int CDBSQLiteConnect::getValue(const int fieldId, time_t &value)
{
    return m_resultSet.getField(fieldId, value);
}

int CDBSQLiteConnect::getValue(const string &fieldName, int &value)
{
    return m_resultSet.getField(fieldName, value);
}

int CDBSQLiteConnect::getValue(const string &fieldName, string &value)
{
    return m_resultSet.getField(fieldName, value);
}

int CDBSQLiteConnect::getValue(const string &fieldName, bool &value)
{
    return m_resultSet.getField(fieldName, value);
}

int CDBSQLiteConnect::getValue(const string &fieldName, float &value)
{
    return m_resultSet.getField(fieldName, value);
}

int CDBSQLiteConnect::getValue(const string &fieldName, double &value)
{
    return m_resultSet.getField(fieldName, value);
}

int CDBSQLiteConnect::getValue(const string &fieldName, time_t &value)
{
    return m_resultSet.getField(fieldName, value);
}

int CDBSQLiteConnect::addBindValue(const int& index, const int& value) {
    // std::cout<<"bindValue(const int& index, const int& value): "<<std::endl;
    return sqlite3_bind_int(m_statement, index + 1, value);
}

int CDBSQLiteConnect::addBindValue(const int &index, const double &value) {
    // std::cout<<"bindValue(const int& index, const double& value)"<<std::endl;
    return sqlite3_bind_double(m_statement, index + 1, value);
}

int CDBSQLiteConnect::addBindValue(const int &index, const string &value) {
    // std::cout<<"bindValue(const int& index, const string& value)"<<std::endl;
    return sqlite3_bind_text(m_statement, index + 1, value.data(), value.length(), SQLITE_TRANSIENT);
}

int CDBSQLiteConnect::addBindValue(const int &index, const char* value) {
    if(value == nullptr) {
        value = "";
    }
    // std::cout<<"bindValue(const int& index, const char* value)"<<std::endl;
    return sqlite3_bind_text(m_statement, index + 1, value, strlen(value), SQLITE_TRANSIENT);
}

#endif
