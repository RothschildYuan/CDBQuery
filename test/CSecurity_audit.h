#ifndef CSECURITY_AUDIT_H
#define CSECURITY_AUDIT_H

#include "configure.h"
#include "stdtype.h"

class CSecurity_audit
{
public:
    CSecurity_audit();

    void clear();

    int getId() const
    {
        return id;
    }
    void setId(int value)
    {
        id = value;
    }
    int getSecuritycheck_id() const
    {
        return securitycheck_id;
    }
    void setSecuritycheck_id(int value)
    {
        securitycheck_id = value;
    }
    int getAudit_type() const
    {
        return audit_type;
    }
    void setAudit_type(int value)
    {
        audit_type = value;
    }
    int getAudit_id() const
    {
        return audit_id;
    }
    void setAudit_id(int value)
    {
        audit_id = value;
    }
    string getAudit_uname() const
    {
        return audit_uname;
    }
    void setAudit_uname(const string &value)
    {
        audit_uname = value;
    }
    string getAudit_nickname() const
    {
        return audit_nickname;
    }
    void setAudit_nickname(const string &value)
    {
        audit_nickname = value;
    }
    string getAudit_time() const
    {
        return audit_time;
    }
    void setAudit_time(const string &value)
    {
        audit_time = value;
    }
    int getAudit_result() const
    {
        return audit_result;
    }
    void setAudit_result(int value)
    {
        audit_result = value;
    }
    string getAudit_explains() const
    {
        return audit_explains;
    }
    void setAudit_explains(const string &value)
    {
        audit_explains = value;
    }

public:
    int    id;
    int    securitycheck_id;
    int    audit_type;
    int    audit_id;
    string audit_uname;
    string audit_nickname;
    string audit_time;
    int    audit_result;
    string audit_explains;
};

ostream& operator <<(ostream& os, const CSecurity_audit& audit);

#endif // CSECURITY_AUDIT_H
