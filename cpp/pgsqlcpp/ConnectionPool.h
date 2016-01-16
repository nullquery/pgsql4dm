#ifndef SRC_CONNECTIONPOOL_H_
#define SRC_CONNECTIONPOOL_H_

#include "Connection.h"
#include "mutex.h"
#include <vector>

class ConnectionPool
{
private:
	std::string conninfo;
	unsigned int maxWait;
	std::vector<Connection*> connections;
	Mutex pool_mutex;
public:
	ConnectionPool(std::string conninfo, unsigned int maxWait);
	~ConnectionPool();
	Connection* getConnection();
	void releaseConnection(Connection* conn);
};

#endif