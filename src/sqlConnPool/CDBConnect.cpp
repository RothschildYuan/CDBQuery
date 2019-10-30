#include "CDBConnect.h"

#include <iostream>

#include "CDBConfig.h"

CDBConnect::CDBConnect()
{

}

CDBConnect::~CDBConnect()
{

}

int CDBConnect::begin() {
    return excuteSql("BEGIN");
}

int CDBConnect::rollback() {
    return excuteSql("ROLLBACK");
}

int CDBConnect::commit() {
    return excuteSql("COMMIT");
}

int CDBConnect::connect(const string& host, const string& user, const string& password, const string& db, const int& port)
{
    return connect(host.c_str(), user.c_str(), password.c_str(), db.c_str(),  port);
}
