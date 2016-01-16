#include "ConnectionPool.h"

ConnectionPool::ConnectionPool(std::string conninfo, unsigned int maxWait)
{
	this->conninfo								= conninfo;
	this->maxWait								= maxWait;
}

ConnectionPool::~ConnectionPool()
{
	for (Connection* conn : this->connections)
	{
		conn->~Connection();
	}
}

#include <fstream>

Connection* ConnectionPool::getConnection()
{
	this->pool_mutex.lock();

	Connection* res					= nullptr;

	while (res == nullptr)
	{
		for (Connection* conn : this->connections)
		{
			if (!conn->isBusy())
			{
				res					= conn;

				break;
			}
		}

		if (res == nullptr)
		{
			if (this->connections.size() < this->maxWait)
			{
				Connection* conn	= new Connection(this->conninfo);

				this->connections.push_back(conn);

				res					= conn;
			}
			else					{ SLEEP(1); }
		}
	}

	res->setBusy(true);

	this->pool_mutex.unlock();

	return res;
}

void ConnectionPool::releaseConnection(Connection* conn)
{
	conn->setBusy(false);
}
