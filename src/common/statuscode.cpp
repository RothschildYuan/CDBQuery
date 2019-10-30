#include "statuscode.h"

///////////////////////////////////       错误返回值     ///////////////////////////////////////////////

const char *getErrnoInfo(EReturnStatus status)
{
    switch (status) {
    case SUCCESS:
        return "SUCCESS";

        // -10000
    case LINUX_FAILED               : return "linux Api error";
    case DB_LIBRARY_INIT_ERROR      : return "数据库连接动态库初始化失败";
    case DB_INIT_ERROR              : return "数据库连接初始化失败";
    case DB_CONNECT_ERROR           : return "数据库连接失败";
    case DB_NOT_INIT_ERROR          : return "数据库未进行初始化";
    case DB_SELECT_DB_ERROR         : return "选择连接数据库实例失败";
    case DB_QUERY_ERROR             : return "sql 执行失败";
    case DB_STORE_RESULT_ERROR      : return "数据库预处理执行失败";
    case DB_STMT_INIT_ERROR         : return "预处理句柄初始化失败";
    case DB_STMT_PREPARE_ERROR      : return "预处理 SQL 语句失败";
    case DB_STMT_EXEC_PREPARED_ERROR: return "预处理执行 SQL 语句失败";
    case DB_HANDLE_GONE_AWAY_ERROR  : return "数据库连接断开";
    case DB_STMT_GONE_AWAY_ERROR    : return "预处理 SQL 遍历错误";
    case DB_GET_FIELDS_ERROR        : return "获取查询数据库的所有字段名错误";
    case DB_NEXT_ROW_ERROR          : return "遍历获取到的数据时，执行下一行错误";
    case DB_SQL_INSERT_ERROR        : return "数据库增操作失败";
    case DB_SQL_DELETE_ERROR        : return "数据库删操作失败";
    case DB_SQL_UPDATE_ERROR        : return "数据库改操作失败";
    case DB_SQL_SELECT_ERROR        : return "数据库查操作失败";

        // -10100
    case HTTP_POST_ERROR            : return "http post 错误";
    case HTTP_DOWN_ERROR            : return "http down 错误";
    case HTTP_GET_ERROR             : return "http get 错误";

        // -10200
    case JSON_PARSE_ERROR           : return "json 解析 错误";

        // -10300
    case FILE_OPEN_ERROR            : return "文件打开 错误";
    case READ_CONFFILE_ERROR        : return "读文件内容错误";

        // -10400
    case MEM_MALLOC_ERROR:
    case MEM_CALLOC_ERROR:
    case MEM_STRDUP_ERROR:
    case MEM_REALLOC_ERROR          : return "申请内存分配失败";

    default:
        break;
    }
	
	return "";
}
