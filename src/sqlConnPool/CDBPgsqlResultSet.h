#ifndef CDBPGSQLRESULTSET_H
#define CDBPGSQLRESULTSET_H

#include "CDBResultSet.h"
#include "stdtype.h"

#ifdef ORM_OF_PGSQL

#include <postgresql/libpq-fe.h>

#define SPQfinish(p) if((p) != nullptr) { PQfinish((p));   (p) = nullptr; }
#define SPQclear(p)  if((p) != nullptr) { PQclear((p));    (p) = nullptr; }

class ACT_CPP_DLL_API CDBPgsqlResultSet : public CDBResultSet
{
    friend class CDBPgsqlConnect;
    CDBPgsqlResultSet(const CDBPgsqlResultSet& );
    CDBPgsqlResultSet& operator=(const CDBPgsqlResultSet& );
public:
    CDBPgsqlResultSet();
    virtual ~CDBPgsqlResultSet();

    bool isEnd() override;

private:
    void reset(PGresult* res);
    void clear() override;
    int getFieldIdByName(const string& fieldName) override;

    /***************** 原生API 操作，性能更强 *********************/

    /**
     * @brief getNextRow  用于遍历赋值查询到的数据
     * @return      true 表示还有下一条数据，false，表示没有下一条数据了
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
    PGresult*       m_pQuery;
};

#endif

#endif // CDBPGSQLRESULTSET_H
