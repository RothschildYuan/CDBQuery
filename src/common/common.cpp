#include "common.h"

#undef read

#ifdef __linux__

//to utf8
int char_convert(const char *from_charset, const char *to_charset, const char *inbuf, int inlen, char *outbuf, int outlen)
{
	iconv_t cd;
	char *pin = (char*)inbuf;
	char *pout = outbuf;
	size_t insize = inlen;
	size_t outsize = outlen;
	cd = iconv_open(to_charset, from_charset);
	if (cd == 0) return -1;
	if ((int)iconv(cd, &pin, &insize, &pout, &outsize) == -1) return -1;
	iconv_close(cd);
	return 0;
}

#else

string GBK_2_UTF8(const string& gbkStr)
{
	string outUtf8;
	int n = MultiByteToWideChar(CP_ACP, 0, gbkStr.c_str(), -1, NULL, 0);
	WCHAR *str1 = new WCHAR[n];
	MultiByteToWideChar(CP_ACP, 0, gbkStr.c_str(), -1, str1, n);
	n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
	char *str2 = new char[n];
	WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);
	outUtf8 = str2;
	delete[]str1;
	str1 = NULL;
	delete[]str2;
	str2 = NULL;
	return outUtf8;
}


string UTF8_2_GBK(const string& utf8Str)
{
	string outGBK;
	int n = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, NULL, 0);
	WCHAR *str1 = new WCHAR[n];
	MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, str1, n);
	n = WideCharToMultiByte(CP_ACP, 0, str1, -1, NULL, 0, NULL, NULL);
	char *str2 = new char[n];
	WideCharToMultiByte(CP_ACP, 0, str1, -1, str2, n, NULL, NULL);
	outGBK = str2;
	delete[] str1;
	str1 = NULL;
	delete[] str2;
	str2 = NULL;
	return outGBK;
}

#endif


string staticFileMessage(string path, string rootpath)
{
    // /home/roths/qt_c++/build-WebServer_fee-Desktop_Qt_5_8_0_GCC_64bit-Debug
    ifstream in;
    in.open(rootpath + path, std::ios::in | std::ios::binary);
    string retdata;
    do {
        if (!in) {
            // retdata = "file Not Found or file no open";
			// break;
			in.open(rootpath + "/err404.html", std::ios::in | std::ios::binary);
        }
        char buffer[1024];
        in.seekg(0, std::ios_base::end);
        size_t len = (size_t)in.tellg();
        printf("len: %lu\n",len);
        in.seekg(0, std::ios_base::beg);
        while (len > 0) {
            memset(buffer, 0, sizeof(buffer));
            size_t size = sizeof(buffer) < len ? sizeof(buffer) : len;
            len -= size;
            in.read(buffer, size);
            retdata += buffer;
        }
    } while(0);
    return retdata;
}
