#ifndef SRC_MUTEX_H_
#define SRC_MUTEX_H_

#include "win32.h"
#ifndef _WIN32
	#include <pthread.h>
#endif

class Mutex
{
public:
	Mutex();
	~Mutex();
	void lock();
	void unlock();

private:
#ifdef _WIN32
	CRITICAL_SECTION criticalSection;
#else
	pthread_mutex_t localLock;
#endif
};

#endif