#include "mutex.h"

Mutex::Mutex()
{
#ifdef _WIN32
	InitializeCriticalSection(&this->criticalSection);
#else
	pthread_mutex_init(&this->localLock, nullptr);
#endif
}

Mutex::~Mutex()
{
#ifdef _WIN32
	DeleteCriticalSection(&this->criticalSection);
#else
	pthread_mutex_destroy(&this->localLock);
#endif
}

void Mutex::lock()
{
#ifdef _WIN32
	EnterCriticalSection(&this->criticalSection);
#else
	pthread_mutex_lock(&this->localLock);
#endif
}

void Mutex::unlock()
{
#ifdef _WIN32
	LeaveCriticalSection(&this->criticalSection);
#else
	pthread_mutex_unlock(&this->localLock);
#endif
}
