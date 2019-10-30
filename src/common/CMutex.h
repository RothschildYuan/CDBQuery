#ifndef CMUTEX_H
#define CMUTEX_H

#include <pthread.h>

namespace ACT {

#define MutexLock_t                     pthread_mutex_t
#define MutexCond_t                     pthread_cond_t
#define MutexLockAttr_t                 pthread_mutexattr_t

class CMutex
{
    friend class cond;

    CMutex(const CMutex&);
    CMutex& operator=(const CMutex&);
public:
    CMutex(bool is_process = false);
    virtual ~CMutex();

    int lock();
    int unlock();
    int trylock();

private:

    MutexLock_t     m_lock;
    MutexLockAttr_t m_mutexattr;
};

}

#ifdef STDCPP11

#include <mutex>

#define ACT_MUTEX std::mutex

#else

#define ACT_MUTEX ACT::CMutex

#endif

#endif // CMutex_H
