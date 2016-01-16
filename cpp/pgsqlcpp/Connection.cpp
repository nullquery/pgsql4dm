#include "Connection.h"
#include <sstream>

Connection::Connection(std::string conninfo)
{
	this->conn								= PQconnectdb(conninfo.c_str());
}

Connection::~Connection()
{
	PQfinish(this->conn);
}

ResultSet* Connection::exec(std::string query)
{
	std::vector<std::string> v;

	return exec(query, v);
}

ResultSet* Connection::exec(std::string query, std::vector<std::string> parameters)
{
	this->transaction_mutex.lock();

	if (PQstatus(this->conn) == CONNECTION_BAD)
	{
		PQreset(this->conn);

		if (PQstatus(this->conn) == CONNECTION_BAD)
		{
			throw std::string("ERROR: No connection to server.");
		}
	}

	ResultSet* resultSet					= new ResultSet();
	std::string error						= "";

	try
	{
		if (PQstatus(this->conn) == CONNECTION_OK)
		{
			PGresult* res;

			res								= PQexec(this->conn, "BEGIN");

			if (PQresultStatus(res) == PGRES_COMMAND_OK)
			{
				PQclear(res);

				char** values				= new char*[parameters.size()];
				int i						= -1;
				char* tmp;

				for (std::string str : parameters)
				{
					if (str == "_<_NULL_>_"){ values[++i] = NULL; }
					else
					{
						tmp					= new char[str.size() + 1];

						memcpy(tmp, str.c_str(), str.size());

						tmp[str.size()]		= 0;

						values[++i]			= tmp;
					}
				}

				if (parameters.empty())		{ res = PQexec(this->conn, query.c_str()); }
				else						{ res = PQexecParams(this->conn, query.c_str(), parameters.size(), NULL, values, NULL, NULL, 0); }

				if (PQresultStatus(res) == PGRES_TUPLES_OK)
				{
					resultSet->init(res);
				}
				else
				{
					char* err = PQresultErrorMessage(res);
					
					error = std::string(err);
				}

				PQclear(res);

				res						= PQexec(this->conn, "END");

				PQclear(res);
			}
			else
			{
				PQclear(res);
			}
		}
	}
	catch (...)
	{
	}

	this->transaction_mutex.unlock();

	if (error == "")					{ return resultSet; }
	else								{ throw error; }
}

unsigned int Connection::execu(std::string query)
{
	std::vector<std::string> v;

	return execu(query, v);
}

unsigned int Connection::execu(std::string query, std::vector<std::string> parameters)
{
	this->transaction_mutex.lock();

	unsigned int ret						= 0;
	std::string error						= "";

	try
	{
		if (PQstatus(this->conn) == CONNECTION_OK)
		{
			PGresult* res;

			res								= PQexec(this->conn, "BEGIN");

			if (PQresultStatus(res) == PGRES_COMMAND_OK)
			{
				PQclear(res);

				char** values				= new char*[parameters.size()];
				int i						= -1;
				char* tmp;

				for (std::string str : parameters)
				{
					if (str == "_<_NULL_>_"){ values[++i] = NULL; }
					else
					{
						tmp					= new char[str.size() + 1];

						memcpy(tmp, str.c_str(), str.size());

						tmp[str.size()]		= 0;

						values[++i]			= tmp;
					}
				}

				if (parameters.empty())		{ res = PQexec(this->conn, query.c_str()); }
				else						{ res = PQexecParams(this->conn, query.c_str(), parameters.size(), NULL, values, NULL, NULL, 0); }

				if (PQresultStatus(res) == PGRES_COMMAND_OK)
				{
					char* rows = PQcmdTuples(res);

					std::istringstream(std::string(rows)) >> ret;
				}
				else
				{
					char* err = PQresultErrorMessage(res);
					
					error = std::string(err);
				}

				PQclear(res);

				res						= PQexec(this->conn, "END");

				PQclear(res);
			}
			else
			{
				PQclear(res);
			}
		}
	}
	catch (...)
	{
	}

	this->transaction_mutex.unlock();

	if (error == "")					{ return ret; }
	else								{ throw error; }
}

bool Connection::isBusy()				{ return this->busy; }
void Connection::setBusy(bool busy)		{ this->busy = busy; }
