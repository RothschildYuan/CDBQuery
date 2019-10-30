#include "CDBMysqlConnect.h"

#include <string.h>
#include "CDBConfig.h"
#include "common.h"

#ifdef ORM_OF_MYSQL

static bool g_isInit = false;

CDBMysqlConnect::CDBMysqlConnect()
{
    m_pMysql        = nullptr;
    m_stmt          = nullptr;
    m_sqlbind       = nullptr;
    m_isPage        = false;
}

CDBMysqlConnect::~CDBMysqlConnect()
{
    disconnect();
    clearStmt();
}

int CDBMysqlConnect::connect(const char* host, const char* user, const char* password, const char* db, int port)
{
    cout << "Connect \n";
    my_init();
    int ret = 0;
    if(!g_isInit) {
        mysql_library_init(0,NULL,NULL);    // mysql_library_end();
        g_isInit = true;
    }
    do {
        if(ret != 0) {
            cout << "mysql_library_init failed\n";
            ret = DB_LIBRARY_INIT_ERROR;
            break;
        }
        m_pMysql = mysql_init((MYSQL*)nullptr);
        if(m_pMysql == nullptr) {
            cout << "mysql_init failed\n";
            ret = DB_INIT_ERROR;
            break;
        }
        //mysql_real_connect连接到MySQL数据库服务器，其中localhost为服务器机器名，root为连接用户名，zhangwei为密码，
        //xxgl为数据库名，3306为端口号
#ifdef _WIN32
		mysql_options(m_pMysql, MYSQL_SET_CHARSET_NAME, "gbk");
#else
		mysql_options(m_pMysql, MYSQL_SET_CHARSET_NAME, "utf8");
#endif
        
        int value = 3;
        mysql_options(m_pMysql, MYSQL_OPT_RECONNECT, &value);
        mysql_options(m_pMysql, MYSQL_OPT_READ_TIMEOUT, &value);
        mysql_options(m_pMysql, MYSQL_OPT_WRITE_TIMEOUT, &value);
        mysql_options(m_pMysql, MYSQL_OPT_CONNECT_TIMEOUT, &value);
        m_pMysql = mysql_real_connect(m_pMysql, host, user, password, db, port, NULL, 0);
        if(m_pMysql == NULL) {
            cout << "mysql_real_connect failed\n";
            mysql_library_end();
            ret = DB_CONNECT_ERROR;
            break;
        }
        cout << "mysql_real_connect success\n";
    } while(0);
    return ret;
}

void CDBMysqlConnect::disconnect()
{
    if(m_pMysql != nullptr) {
        mysql_close(m_pMysql);
        m_pMysql = nullptr;
    }
}

void CDBMysqlConnect::startPageQuery() {
    m_isPage = true;
}

void CDBMysqlConnect::stopPageQuery() {
    m_isPage = false;
}

int CDBMysqlConnect::pagingCount()
{
    int num = 0;
    int ret = mysql_query(m_pMysql, "SELECT FOUND_ROWS();");
    if(ret) {
        cout<<ret<<"  getLastErrorMsg: "<<getLastErrorMsg()<<endl;
        return DB_QUERY_ERROR;
    }
    MYSQL_ROW row;
    MYSQL_RES *result = mysql_store_result(m_pMysql);
    if(result == nullptr) {
        return DB_STORE_RESULT_ERROR;
    }
    while ((row=mysql_fetch_row(result)) != nullptr) {
        num = atoi(row[0]);
        break;
    }
    mysql_free_result(result);
    return num;
}

void CDBMysqlConnect::clearQuery()
{
    m_resultSet.clear();
}

int CDBMysqlConnect::connctionTest()
{
    if (m_pMysql == nullptr)
    {
        return DB_HANDLE_GONE_AWAY_ERROR;
    }
    int ret = mysql_ping(m_pMysql);
    if(ret == 0) {
        return ret;
    }
    return mysql_ping(m_pMysql);
}

/*
 * mysql_rollback(m_pMysql);
 * mysql_commit(m_pMysql);
 * mysql_autocommit(m_pMysql, false);
 */
int CDBMysqlConnect::excuteQuery(const char *szSql)
{
    if (m_pMysql == nullptr)
    {
        return DB_HANDLE_GONE_AWAY_ERROR;
    }
    char* tmp = (char*)szSql;
    if(m_isPage) {
        char* p = strcasestr(tmp, "select");
        size_t size = strlen(szSql);
        tmp = (char*)alloca(size+32);
        snprintf(tmp, size+32, "SELECT SQL_CALC_FOUND_ROWS %s", p+6);
        m_isPage = false;
    }
    int ret = mysql_query(m_pMysql, tmp);
    if(ret)
    {
        cout<<ret<<"  getLastErrorMsg: "<<getLastErrorMsg()<<endl;
        return DB_QUERY_ERROR;
    }
    return m_resultSet.reset(mysql_store_result(m_pMysql));
}

int CDBMysqlConnect::excuteSql(const char *szSql)
{
    int ret = 0;
    do {
        if (m_pMysql == nullptr)
        {
            ret = DB_HANDLE_GONE_AWAY_ERROR;
            break;
        }

        if(mysql_query(m_pMysql,szSql))
        {
            ret = DB_QUERY_ERROR;
            break;
        }

        if(strcasestr(szSql, "INSERT") == nullptr) {
            // 操作所影响的记录行数
            ret = (int)mysql_affected_rows(m_pMysql);
        } else {
            // 获取最后插入的数据的自增ID
            ret = (int)mysql_insert_id(m_pMysql);
        }
    } while(0);
    return ret;
}

const char* CDBMysqlConnect::getLastErrorMsg()
{
    if (m_pMysql == nullptr)
    {
        return "DB_HANDLE_GONE_AWAY_ERROR";
    }
    printf("\n\nmysql_errno:  %d\n\n", mysql_errno(m_pMysql));
    return mysql_error(m_pMysql);
}

unsigned long CDBMysqlConnect::escapeRealToString(  char *pDst, const char *pSrc, unsigned long ulSrcLen )
{
    if (m_pMysql == nullptr)
    {
        return SUCCESS;
    }
    //这里添加参数检查代码…
    if( ulSrcLen <= 0 )
    {
        //这里添加日志打印语句
        return ulSrcLen;
    }
    return mysql_real_escape_string(m_pMysql,pDst, pSrc, ulSrcLen);
}

int CDBMysqlConnect::prepare(const string& query)
{
	m_sql = query;
    clearStmt();
    if(m_stmt == nullptr) {
        if(m_pMysql == nullptr) {
            return DB_HANDLE_GONE_AWAY_ERROR;
        }
        m_stmt = mysql_stmt_init(m_pMysql);
        if(m_stmt == nullptr) {
            return DB_STMT_INIT_ERROR;
        }
    }

    char* tmp = (char*)query.c_str();
    if(m_isPage) {
        char* p = strcasestr(tmp, "select");
        size_t size = query.length();
        tmp = (char*)alloca(size+32);
        snprintf(tmp, size+32, "SELECT SQL_CALC_FOUND_ROWS %s", p+6);
        m_isPage = false;
    }

    int ret = mysql_stmt_prepare(m_stmt, tmp, strlen(tmp));
    m_binCount = mysql_stmt_param_count(m_stmt);
    m_sqlbind = (MYSQL_BIND*)calloc(m_binCount, sizeof(MYSQL_BIND));
    return ret;
}

int CDBMysqlConnect::exec(const string& sql) {
    if(!sql.empty()) {
        return excuteSql(sql.c_str());
    }
    int ret = 0;
    if(m_stmt == nullptr) {
        return DB_STMT_GONE_AWAY_ERROR;
    }
    ret = mysql_stmt_execute(m_stmt);
	if (ret != 0) {
		printf("mysql_error: %s\n", getLastErrorMsg());
		return DB_STMT_GONE_AWAY_ERROR;
	}
    // 判断操作类型 insert（返回 id） 还是 delete/update（影响条数）
    if(strcasestr(m_sql.c_str(), "INSERT") != nullptr) {
        // 获取最后插入的数据的自增ID
        ret = (int)mysql_stmt_insert_id(m_stmt);
    } else {
        // 操作所影响的记录行数
        ret = (int)mysql_stmt_affected_rows(m_stmt);
    }
    return ret;
}

int CDBMysqlConnect::query(const string& sql) {
    if(!sql.empty()) {
        return excuteQuery(sql.c_str());
    }
    if(m_stmt == nullptr) {
        return DB_STMT_INIT_ERROR;
    }
    int ret = m_resultSet.reset(m_stmt);
    return ret;
}

bool CDBMysqlConnect::isEnd()
{
    return m_resultSet.isEnd();
}

int CDBMysqlConnect::size()
{
    return m_resultSet.size();
}

bool CDBMysqlConnect::next()
{
    return m_resultSet.next();
}

CDBObject CDBMysqlConnect::value(const string &fieldName)
{
    string data;
    int ret = getValue(fieldName, data);
    if(ret < 0) {
        return "";
    }
    return data;
}

CDBResultSet::CDBResult CDBMysqlConnect::getValue(const string &fieldName)
{
    return m_resultSet.getValueRes(fieldName);
}

CDBResultSet::CDBResult CDBMysqlConnect::getValue(const int &fieldId)
{
    return m_resultSet.getValueRes(fieldId);
}

int CDBMysqlConnect::getValue(const int fieldId, int &value)
{
    return m_resultSet.getField(fieldId, value);
}

int CDBMysqlConnect::getValue(const int fieldId, string &value)
{
    return m_resultSet.getField(fieldId, value);
}

int CDBMysqlConnect::getValue(const int fieldId, bool &value)
{
    return m_resultSet.getField(fieldId, value);
}

int CDBMysqlConnect::getValue(const int fieldId, float &value)
{
    return m_resultSet.getField(fieldId, value);
}

int CDBMysqlConnect::getValue(const int fieldId, double &value)
{
    return m_resultSet.getField(fieldId, value);
}

int CDBMysqlConnect::getValue(const int fieldId, time_t &value)
{
    return m_resultSet.getField(fieldId, value);
}

int CDBMysqlConnect::getValue(const string &fieldName, int &value)
{
    return m_resultSet.getField(fieldName, value);
}

int CDBMysqlConnect::getValue(const string &fieldName, string &value)
{
    return m_resultSet.getField(fieldName, value);
}

int CDBMysqlConnect::getValue(const string &fieldName, bool &value)
{
    return m_resultSet.getField(fieldName, value);
}

int CDBMysqlConnect::getValue(const string &fieldName, float &value)
{
    return m_resultSet.getField(fieldName, value);
}

int CDBMysqlConnect::getValue(const string &fieldName, double &value)
{
    return m_resultSet.getField(fieldName, value);
}

int CDBMysqlConnect::getValue(const string &fieldName, time_t &value)
{
    return m_resultSet.getField(fieldName, value);
}

/*
    MYSQL_TYPE_DECIMAL, MYSQL_TYPE_TINY,
    MYSQL_TYPE_SHORT,  MYSQL_TYPE_LONG,
    MYSQL_TYPE_FLOAT,  MYSQL_TYPE_DOUBLE,
    MYSQL_TYPE_NULL,   MYSQL_TYPE_TIMESTAMP,
    MYSQL_TYPE_LONGLONG,MYSQL_TYPE_INT24,
    MYSQL_TYPE_DATE,   MYSQL_TYPE_TIME,
    MYSQL_TYPE_DATETIME, MYSQL_TYPE_YEAR,
    MYSQL_TYPE_NEWDATE, MYSQL_TYPE_VARCHAR,
    MYSQL_TYPE_BIT,
    MYSQL_TYPE_TIMESTAMP2,
    MYSQL_TYPE_DATETIME2,
    MYSQL_TYPE_TIME2,
    MYSQL_TYPE_JSON=245,
    MYSQL_TYPE_NEWDECIMAL=246,
    MYSQL_TYPE_ENUM=247,
    MYSQL_TYPE_SET=248,
    MYSQL_TYPE_TINY_BLOB=249,
    MYSQL_TYPE_MEDIUM_BLOB=250,
    MYSQL_TYPE_LONG_BLOB=251,
    MYSQL_TYPE_BLOB=252,
    MYSQL_TYPE_VAR_STRING=253,
    MYSQL_TYPE_STRING=254,
    MYSQL_TYPE_GEOMETRY=255
*/

int CDBMysqlConnect::bindResult(MYSQL_BIND* sqlbind, const int& value) {
    if(sqlbind == nullptr) {
        return -1;
    }
    sqlbind->buffer_type    = MYSQL_TYPE_LONG;
    sqlbind->buffer         = (void*)&value;
    return SUCCESS;
}

int CDBMysqlConnect::bindResult(MYSQL_BIND* sqlbind, const float& value) {
    if(sqlbind == nullptr) {
        return -1;
    }
    sqlbind->buffer_type    = MYSQL_TYPE_FLOAT;
    sqlbind->buffer         = (void*)&value;
    return SUCCESS;
}

int CDBMysqlConnect::bindResult(MYSQL_BIND* sqlbind, const string& value) {
    if(sqlbind == nullptr) {
        return -1;
    }
    sqlbind->buffer_type    = MYSQL_TYPE_STRING;
    sqlbind->buffer         = (void*)value.c_str();
    sqlbind->buffer_length  = 1024;
    return SUCCESS;
}

int CDBMysqlConnect::bindResult(MYSQL_BIND* sqlbind, const char* value) {
    if(sqlbind == nullptr) {
        return -1;
    }
    sqlbind->buffer_type    = MYSQL_TYPE_STRING;
    sqlbind->buffer         = (void*)value;
    sqlbind->buffer_length  = 1024;
    return SUCCESS;
}

int CDBMysqlConnect::bindResult(MYSQL_BIND* sqlbind, const QText& value) {
    if(sqlbind == nullptr) {
        return -1;
    }
    sqlbind->buffer_type    = MYSQL_TYPE_STRING;
    sqlbind->buffer         = value.m_data;
    sqlbind->buffer_length  = value.m_len;
    sqlbind->length         = (unsigned long*)&value.m_len;
    return SUCCESS;
}

void CDBMysqlConnect::clearStmt()
{
    if(m_stmt != nullptr) {
        mysql_stmt_free_result(m_stmt);
        mysql_stmt_reset(m_stmt);
        mysql_stmt_close(m_stmt);
        m_stmt = nullptr;
    }
    SFree(m_sqlbind);
}

int CDBMysqlConnect::stmtBindParam(const int &index) {
    if(index == m_binCount - 1) {
        int ret = mysql_stmt_bind_param(m_stmt, m_sqlbind);
        if(ret != 0) {
            return ret;
        }
    } else if(index >= m_binCount) {
        perror("bind param count < addBindValue index");
    }
    return SUCCESS;
}

int CDBMysqlConnect::addBindValue(const int& index, const int& value) {
    if(m_sqlbind == nullptr) {
        return -1;
    }
    m_sqlbind[index].buffer_type    = MYSQL_TYPE_LONG;
    m_sqlbind[index].buffer         = (void*)&value;// buf;//
    return stmtBindParam(index);
}

int CDBMysqlConnect::addBindValue(const int& index, const double& value) {
    if(m_sqlbind == nullptr) {
        return -1;
    }
    m_sqlbind[index].buffer_type    = MYSQL_TYPE_FLOAT;
    m_sqlbind[index].buffer         = (void*)&value;
    return stmtBindParam(index);
}

int CDBMysqlConnect::addBindValue(const int& index, const string& value) {
    if(m_sqlbind == nullptr) {
        return -1;
    }
    m_sqlbind[index].buffer_type    = MYSQL_TYPE_STRING;
    m_sqlbind[index].buffer         = (void*)value.c_str();
    m_sqlbind[index].buffer_length  = value.length();
    return stmtBindParam(index);
}

int CDBMysqlConnect::addBindValue(const int& index, const char* value) {
    if(m_sqlbind == nullptr) {
        return -1;
    }
    m_sqlbind[index].buffer_type    = MYSQL_TYPE_STRING;
    m_sqlbind[index].buffer         = (void*)value;
    m_sqlbind[index].buffer_length  = strlen(value);
    return stmtBindParam(index);
}

#endif
