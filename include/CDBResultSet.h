#ifndef CDBRESULTSET_H
#define CDBRESULTSET_H

#include "CDBConfig.h"
#include "stdtype.h"

typedef enum ESqlExecMode {
    ESqlExecModePrepare = 1, // 预处理 SQL 方式执行
    ESqlExecModeQuery   = 2  // 直接执行 SQL 方式执行
} ESqlExecMode;

class ACT_CPP_DLL_API CDBResultSet
{
public:
    class ACT_CPP_DLL_API CDBResult
    {
        CDBResultSet*   m_obj;
        int             m_i;
    public:
        CDBResult(CDBResultSet* obj, const int i)
            : m_obj(obj), m_i(i)  {

        }
        operator int() const {
            return m_obj->getValueInt(m_i);
        }
        operator UInt4() const {
            return m_obj->getValueInt(m_i);
        }
        operator string() const {
            return m_obj->getValueString(m_i);
        }
        operator QText() const {
            return m_obj->getValueQText(m_i);
        }
        operator bool() const {
            return m_obj->getValueBool(m_i);
        }
        operator float() const {
            return m_obj->getValueFloat(m_i);
        }
        operator double() const {
            return m_obj->getValueDouble(m_i);
        }
        operator time_t() const {
            return m_obj->getValueTime_t(m_i);
        }
    };

public:
    CDBResultSet();
    virtual ~CDBResultSet();

    /**
     * @brief isEnd 用于遍历赋值查询到的数据判断是否已经没有下一条数据了
     * @return      true：表示没有下一条数据了，false：表示还有下一条数据
     */
    virtual bool isEnd()                                     = 0;

    /**
     * @brief clear 清空 CDBResultSet 类中查询过程中的缓存资源
     */
    virtual void clear()                                     = 0;

    /**
     * @brief size  查询到的数据条数
     * @return      < 0 失败，其他成功
     */
    virtual int  size();

    /************ 共用数据库操作接口（查询数据存入缓存中，然后从缓存中遍历读取） *************/

    /**
     * @brief next  用于遍历赋值查询到的数据
     * @return      true 表示还有下一条数据，false，表示没有下一条数据了
     */
    virtual bool next() = 0;
    virtual int  getField(const int fieldId, int& value)     = 0;
    virtual int  getField(const int fieldId, string& value)  = 0;
    virtual int  getField(const int fieldId, QText& value)   = 0;
    virtual int  getField(const int fieldId, bool& value)    = 0;
    virtual int  getField(const int fieldId, float& value)   = 0;
    virtual int  getField(const int fieldId, double& value)  = 0;
    virtual int  getField(const int fieldId, time_t& value)  = 0;

    template<typename _T>
    int  getField(const string& fieldName, _T& value);

    CDBResult getValueRes(const int& fieldId);
    CDBResult getValueRes(const string& fieldName);

private:
    int    getValueInt(const int fieldId);
    string getValueString(const int fieldId);
    QText  getValueQText(const int fieldId);
    bool   getValueBool(const int fieldId);
    float  getValueFloat(const int fieldId);
    double getValueDouble(const int fieldId);
    time_t getValueTime_t(const int fieldId);

    /********************** 各数据库自定义操作接口（性能更优） ************************/

protected:
    virtual int  getFieldIdByName(const string& fieldName)  = 0;

    /**
     * @brief getNextRow  用于遍历赋值查询到的数据
     * @return      true 表示还有下一条数据，false，表示没有下一条数据了
     */
    virtual bool getNextRow()                  {   return true;  }
    virtual int  getValue(const int , int& )   {   return 0;  }
    virtual int  getValue(const int , string& ){   return 0;  }
    virtual int  getValue(const int , QText& ) {   return 0;  }
    virtual int  getValue(const int , bool& )  {   return 0;  }
    virtual int  getValue(const int , float& ) {   return 0;  }
    virtual int  getValue(const int , double& ){   return 0;  }
    virtual int  getValue(const int , time_t& ){   return 0;  }

    template<typename _T>
    int  getValue(const string& fieldName, _T& value);

protected:
    int                     m_nRowCount;   // 总共多少行
    int                     m_nRowIndex;   // 遍历到第几行
    int                     m_nColumnCount;// 查询表多少字段

    int                     m_isSTMT;      // 1, 自定义KV数组容器  2,原生容器
};

#endif // CDBResultSet_H
