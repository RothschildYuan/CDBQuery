#include "CSecurity_audit.h"

CSecurity_audit::CSecurity_audit() {
    clear();
}

void CSecurity_audit::clear() {
    id              = 0;
    securitycheck_id= 0;
    audit_type      = 0;
    audit_id        = 0;
    audit_uname     = "";
    audit_nickname  = "";
    audit_time      = "";
    audit_result    = 0;
    audit_explains  = "";
}

ostream &operator <<(ostream &os, const CSecurity_audit &audit) {
    os<<endl;
    os<<"id              : "<<audit.id<<endl;
    os<<"securitycheck_id: "<<audit.securitycheck_id<<endl;
    os<<"audit_type      : "<<audit.audit_type<<endl;
    os<<"audit_id        : "<<audit.audit_id  <<endl;
    os<<"audit_uname     : "<<audit.audit_uname  <<endl;
    os<<"audit_nickname  : "<<audit.audit_nickname<<endl;
    os<<"audit_time      : "<<audit.audit_time    <<endl;
    os<<"audit_result    : "<<audit.audit_result  <<endl;
    os<<"audit_explains  : "<<audit.audit_explains<<endl<<endl;
    return os;
}
