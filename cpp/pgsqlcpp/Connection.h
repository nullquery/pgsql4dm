#ifndef SRC_CONNECTION_H_
#define SRC_CONNECTION_H_

#include <string>
#include <string.h>
#include "mutex.h"
#include <libpq-fe.h>
#include "ResultSet.h"

class Connection
{
private:
	PGconn* conn;
	Mutex transaction_mutex;
	bool busy;
public:
	Connection(std::string conninfo);
	~Connection();
	ResultSet* exec(std::string query);
	ResultSet* exec(std::string query, std::vector<std::string> parameters);
	unsigned int execu(std::string query);
	unsigned int execu(std::string query, std::vector<std::string> parameters);
	bool isBusy();
	void setBusy(bool busy);
};

#endif