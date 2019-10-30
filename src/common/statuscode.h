#ifndef STATUSCODE_H
#define STATUSCODE_H

///////////////////////////////////       错误返回值     ///////////////////////////////////////////////

enum EReturnStatus {
    SUCCESS                     = 0,        // 成功
    LINUX_FAILED                = -10000,   // linux Api error
    DB_LIBRARY_INIT_ERROR       = -10001,   // 数据库连接动态库初始化失败
    DB_INIT_ERROR               = -10002,   // 数据库连接初始化失败
    DB_CONNECT_ERROR            = -10003,   // 数据库连接失败
    DB_NOT_INIT_ERROR           = -10004,   // 数据库未进行初始化
    DB_SELECT_DB_ERROR          = -10005,   // 选择连接数据库实例失败
    DB_QUERY_ERROR              = -10006,   // sql 执行失败
    DB_STORE_RESULT_ERROR       = -10007,   // 数据库预处理执行失败
    DB_STMT_INIT_ERROR          = -10008,   // 预处理句柄初始化失败
    DB_STMT_PREPARE_ERROR       = -10009,   // 预处理 SQL 语句失败
    DB_STMT_EXEC_PREPARED_ERROR = -10010,   // 预处理执行 SQL 语句失败
    DB_HANDLE_GONE_AWAY_ERROR   = -10011,   // 数据库连接断开
    DB_STMT_GONE_AWAY_ERROR     = -10012,   // 预处理 SQL 遍历错误
    DB_GET_FIELDS_ERROR         = -10013,   // 获取查询数据库的所有字段名错误
    DB_NEXT_ROW_ERROR           = -10014,   // 遍历获取到的数据时，执行下一行错误
    DB_SQL_INSERT_ERROR         = -10015,   // 数据库增操作失败
    DB_SQL_DELETE_ERROR         = -10016,   // 数据库删操作失败
    DB_SQL_UPDATE_ERROR         = -10017,   // 数据库改操作失败
    DB_SQL_SELECT_ERROR         = -10018,   // 数据库查操作失败

    HTTP_POST_ERROR             = -10101,   // http post 错误
    HTTP_DOWN_ERROR             = -10102,   // http down 错误
    HTTP_GET_ERROR              = -10103,   // http get 错误

    JSON_PARSE_ERROR            = -10201,   // json 解析 错误

    FILE_OPEN_ERROR             = -10301,   // 文件打开 错误
    READ_CONFFILE_ERROR         = -10302,   // 读文件内容错误

    MEM_MALLOC_ERROR            = -10401,   // 申请内存分配失败
    MEM_CALLOC_ERROR            = -10402,   // 申请内存分配失败
    MEM_STRDUP_ERROR            = -10403,   // 申请内存分配失败
    MEM_REALLOC_ERROR           = -10404,   // 申请内存分配失败

    SUCCESS5 = -1014
};

const char* getErrnoInfo(EReturnStatus status);

#endif // CONFIG_H
