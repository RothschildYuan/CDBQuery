#ifndef STDTYPE_H
#define STDTYPE_H

typedef char                    Int1;
typedef short                   Int2;
typedef int                     Int4;
typedef long long int           Int8;

typedef Int1*                   PInt1;
typedef Int2*                   PInt2;
typedef Int4*                   PInt4;
typedef Int8*                   PInt8;

typedef unsigned char           UInt1;
typedef unsigned short          UInt2;
typedef unsigned int            UInt4;
typedef unsigned long long int  UInt8;
typedef unsigned char           Byte;
typedef unsigned long           DWORD;

typedef UInt1*                  PUInt1;
typedef UInt2*                  PUInt2;
typedef UInt4*                  PUInt3;
typedef UInt8*                  PUInt4;
typedef Byte*                   PByte;
typedef DWORD*                  PDWORD;

typedef float                   Float4;
typedef double                  Float8;
typedef long double             Float16;

typedef Float4*                 PFloat4;
typedef Float8*                 PFloat8;
typedef Float16*                PFloat16;

typedef char                    Char;
typedef Char*                   PChar;

typedef void*                   Pointer;

#define SFree(p)    if(p != nullptr) { free(p);     p = nullptr;}

#ifdef __cplusplus

#include <iostream>
#include <fstream>
#include <strstream>
#include <string>
#include <vector>
#include <deque>
#include <list>
#include <map>
#include <set>
#include <memory>
#include <stdexcept>
#include <typeinfo>
#include <functional>

#define SDelete(p)  if(p != nullptr) { delete p;    p = nullptr;}
#define SDeletes(p) if(p != nullptr) { delete[] p;  p = nullptr;}

typedef bool*       PBool;

using   std::ostream;
using   std::istream;
using   std::fstream;
using   std::ifstream;
using   std::ofstream;
using   std::stringstream;

using   std::cout;
using   std::endl;

using   std::function;
using   std::forward;
using   std::make_pair;

using   std::string;
using   std::vector;
using   std::deque;
using   std::list;
using   std::map;
using   std::set;
using   std::exception;

typedef string*     PString;

#define PVector     vector*
#define PDeque      deque*
#define PList       list*
#define PMap        map*
#define PSet        set*

#if __cplusplus >= 201103L

#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <mutex>

using   std::unordered_map;
using   std::unordered_set;
using   std::shared_ptr;
using   std::weak_ptr;

typedef std::mutex ACTMutex;

#else

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <boost/function.hpp>
#include <CMutex.h>

using   boost::unordered::unordered_map;
using   boost::unordered::unordered_set;
using   boost::shared_ptr;
using   boost::weak_ptr;
typedef ACT::CMutex ACTMutex;

#define nullptr			NULL
#define override
#define final

#endif

#else

typedef char        bool;
typedef bool*       PBool;

#define true        1
#define false       0

#endif

//
#ifdef __MACH__

typedef unsigned long int       PthreadSize;
typedef int SOCKET;
#define INVALID_SOCKET  (SOCKET)(~0)
#define SOCKET_ERROR            (-1)

#elif defined _WIN32

typedef unsigned int            PthreadSize;

#else

typedef unsigned int            PthreadSize;
typedef int SOCKET;
#define INVALID_SOCKET  (SOCKET)(~0)
#define SOCKET_ERROR            (-1)

#endif

typedef enum
{
    tpIdle,
    tpLowest,
    tpLower,
    tpNormal,
    tpHigher,
    tpHighest,
    tpTimeCritical
}TThreadPriority;

// 字节对齐规则
#pragma pack(push, 1)
#pragma pack(pop)

#endif
