#ifndef COMMON_H
#define COMMON_H

#include "config.h"
#include "stdtype.h"

template<typename _T>
string itoa(const _T& t) {
    std::stringstream ss;
    ss << t;
    return ss.str();
}

#ifdef __linux__

//to utf8
int char_convert(const char *from_charset, const char *to_charset, const char *inbuf, int inlen, char *outbuf, int outlen);

#else

string GBK_2_UTF8(const string& gbkStr);
string UTF8_2_GBK(const string& utf8Str);

#endif

string staticFileMessage(string path, string rootpath = "./static");

#endif // COMMON_H
