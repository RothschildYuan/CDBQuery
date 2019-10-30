#ifndef CDBSQLITERESULTSET_H
#define CDBSQLITERESULTSET_H

#include "CDBResultSet.h"
#include "stdtype.h"

#ifdef ORM_OF_SQLITE

#include "sqlite3.h"

class ACT_CPP_DLL_API CDBSQLiteResultSet : public CDBResultSet
{
    typedef unordered_map<int, string>        DataRowType;	//���ݱ��е���
    typedef vector<shared_ptr<DataRowType>>   DataTableType;	//����???
    friend int callback(void* arg,int column_count,char **column_value,char** column_name);
    friend class CDBSQLiteConnect;
    typedef unordered_map<string, int>    KeyNameIndex;

    CDBSQLiteResultSet(const CDBSQLiteResultSet& );
    CDBSQLiteResultSet& operator=(const CDBSQLiteResultSet& );
public:
    CDBSQLiteResultSet();
    virtual ~CDBSQLiteResultSet();

    bool isEnd() override;

private:
    void reset(sqlite3_stmt* statement = nullptr);
    void clear() override;
    int getFieldIdByName(const string& fieldName) override;

public:

    /***************** ��ѯʱ�����ݴ��뻺���У�Ȼ�����ζ�ȡ *********************/

    /**
     * @brief next  ���ڱ�����ֵ��ѯ��������
     * @return      true ��ʾ������һ�����ݣ�false����ʾû����һ��������
     */
    virtual bool next()override;
    using CDBResultSet::getField;
    virtual int  getField(const int fieldId, int& value)    override;
    virtual int  getField(const int fieldId, string& value) override;
    virtual int  getField(const int fieldId, QText& value)  override;
    virtual int  getField(const int fieldId, bool& value)   override;
    virtual int  getField(const int fieldId, float& value)  override;
    virtual int  getField(const int fieldId, double& value) override;
    virtual int  getField(const int fieldId, time_t& value) override;

    void dataAppend(const shared_ptr<DataRowType>& pRow) {
        m_data.push_back(pRow);
    }

private:
    DataTableType           m_data;
    shared_ptr<DataRowType> m_pTmp;

    KeyNameIndex    m_keyIndex;
};

#endif

#endif // CDBSQLITERESULTSET_H
