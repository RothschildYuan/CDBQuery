#ifndef CDBMYSQLRESULTSET_H
#define CDBMYSQLRESULTSET_H

#include "CDBResultSet.h"
#include "stdtype.h"

#ifdef ORM_OF_MYSQL

#include <mysql/mysql.h>

class ACT_CPP_DLL_API CDBMysqlResultSet : public CDBResultSet
{
    typedef unordered_map<int, string>        DataRowType;	//数据表中的行
    typedef vector<shared_ptr<DataRowType>>   DataTableType;	//数据表

    friend class CDBMysqlConnect;
    CDBMysqlResultSet(const CDBMysqlResultSet& );
    CDBMysqlResultSet& operator=(const CDBMysqlResultSet& );
public:
    CDBMysqlResultSet();
    virtual ~CDBMysqlResultSet();

    bool isEnd() override;

private:
    int reset(MYSQL_RES* pMyQuery);
    int reset(MYSQL_STMT* stmt);
    void clear() override;
    int getFieldIdByName(const string& fieldName) override;

    /***************** 原生API 操作，性能更强 *********************/

    /**
     * @brief getNextRow    用于遍历赋值查询到的数据
     * @return              true 表示还有下一条数据，false，表示没有下一条数据了
     */
    bool getNextRow() override;
    int  getValue(const int fieldId, int& value) override;
    int  getValue(const int fieldId, string& value) override;
    int  getValue(const int fieldId, QText& value) override;
    int  getValue(const int fieldId, bool& value) override;
    int  getValue(const int fieldId, float& value) override;
    int  getValue(const int fieldId, double& value) override;
    int  getValue(const int fieldId, time_t& value) override;

public:
    /***************** 查询时，内容存入缓存中，然后依次读取 *********************/

    /**
     * @brief next  用于遍历赋值查询到的数据
     * @return      true 表示还有下一条数据，false，表示没有下一条数据了
     */
    virtual bool next() override;
    using CDBResultSet::getField;


    virtual int  getField(const int fieldId, int& value)    override;
    virtual int  getField(const int fieldId, string& value) override;
    virtual int  getField(const int fieldId, QText& value)  override;
    virtual int  getField(const int fieldId, bool& value)   override;
    virtual int  getField(const int fieldId, float& value)  override;
    virtual int  getField(const int fieldId, double& value) override;
    virtual int  getField(const int fieldId, time_t& value) override;

private:
    MYSQL_RES*      m_pQuery;      // 结果集
    MYSQL_ROW       m_curRow;      // 基类

    DataTableType           m_data;
    shared_ptr<DataRowType> m_pTmp;
};

#endif

#endif // CDBMysqlResultSet_H
