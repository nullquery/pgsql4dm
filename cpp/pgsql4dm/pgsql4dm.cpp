#include "win32.h"
#include <string>
#include <string.h>
#include <map>
#include "../pgsqlcpp/ConnectionPool.h"
#include "../pgsqlcpp/Connection.h"
#include "../pgsqlcpp/DO.h"
#include <stdio.h>
#include <sstream>

std::map<std::string, ConnectionPool*> pools;
std::map<unsigned int, ResultSet*> results;

ConnectionPool* getPool(std::string conninfo)
{
	if (!pools.count(conninfo))						{ pools[conninfo] = new ConnectionPool(conninfo, 8); }

	return pools[conninfo];
}

void destroyPool(std::string conninfo)
{
	if (pools.count(conninfo))
	{
		delete pools[conninfo];

		pools.erase(pools.find(conninfo));
	}
}

extern "C" EXPORT char *exec(int n, char *v[]) // exec(conninfo, query, ...)
{
	try
	{
		if (n >= 2)
		{
			ConnectionPool* pool					= getPool(std::string(v[0]));
			Connection* conn						= pool->getConnection();

			try
			{
				std::vector<std::string> params;

				for (int i = 2; i < n; i = i + 1)	{ params.push_back(v[i]); }

				try
				{
					ResultSet *rs					= conn->exec(std::string(v[1]), params);

					unsigned int id = -1;

					for (unsigned int i = 0; i < results.size(); i = i + 1)
					{
						if (!results.count(i))		{ id = i; break; }
					}

					if (id == -1)					{ id = results.size(); }

					results[id]						= rs;

					char* buf						= new char[32];

					intToChar(id, buf, 32);

					pool->releaseConnection(conn);

					return buf;
				}
				catch (std::string error)
				{
					pool->releaseConnection(conn);

					error							= std::string("_<SQLERROR>_" + error);

					char* buf						= new char[error.size() + 1];
					memcpy(buf, error.c_str(), error.size());
					buf[error.size()]				= 0;

					return buf;
				}
			}
			catch (...)								{ return (char*) "-3"; }

			pool->releaseConnection(conn);
		}
		else										{ return (char*) "-2"; }
	}
	catch (...)										{ return (char*) "-1"; }
}

extern "C" EXPORT char *execu(int n, char *v[]) // execu(conninfo, query, ...)
{
	try
	{
		if (n >= 2)
		{
			ConnectionPool* pool					= getPool(std::string(v[0]));
			Connection* conn						= pool->getConnection();

			try
			{
				std::vector<std::string> params;

				for (int i = 2; i < n; i = i + 1)	{ params.push_back(v[i]); }

				try
				{
					unsigned int i					= conn->execu(std::string(v[1]), params);

					char* buf						= new char[32];

					intToChar(i, buf, 32);

					pool->releaseConnection(conn);

					return buf;
				}
				catch (std::string error)
				{
					pool->releaseConnection(conn);

					error							= std::string("_<SQLERROR>_" + error);

					char* buf						= new char[error.size() + 1];
					memcpy(buf, error.c_str(), error.size());
					buf[error.size()]				= 0;

					return buf;
				}
			}
			catch (...)								{ return (char*) "-3"; }

			pool->releaseConnection(conn);
		}
		else										{ return (char*) "-2"; }
	}
	catch (...)										{ return (char*) "-1"; }
}

extern "C" EXPORT char *result(int n, char *v[])
{
	try
	{
		if (n >= 2)
		{
			unsigned int i;

			std::istringstream(std::string(v[0])) >> i;

			if (results.count(i))
			{
				std::string function(v[1]);

				if		(function == "next")
				{
					if (results[i]->next())			{ return (char*) "1"; }
					else							{ return (char*) "0"; }
				}
				else if (function == "previous")
				{
					if (results[i]->previous())		{ return (char*) "1"; }
					else							{ return (char*) "0"; }
				}
				else if (function == "isBeforeFirst")
				{
					if (results[i]->isBeforeFirst()){ return (char*) "1"; }
					else							{ return (char*) "0"; }
				}
				else if (function == "isAfterLast")
				{
					if (results[i]->isAfterLast())	{ return (char*) "1"; }
					else							{ return (char*) "0"; }
				}
				else if (function == "isFirst")
				{
					if (results[i]->isFirst())		{ return (char*) "1"; }
					else							{ return (char*) "0"; }
				}
				else if (function == "isLast")
				{
					if (results[i]->isLast())		{ return (char*) "1"; }
					else							{ return (char*) "0"; }
				}
				else if (function == "first")
				{
					results[i]->first();

					return (char*) "1";
				}
				else if (function == "last")
				{
					results[i]->last();

					return (char*) "1";
				}
				else if (function == "getNumber")
				{
					if (n == 3)
					{
						unsigned int column			= results[i]->getNumber(v[2]);

						char* buf					= new char[32];

						intToChar(column, buf, 32);

						return buf;
					}
					else							{ return (char*) "-5"; }
				}
				else if (function == "dispose")
				{
					delete results[i];

					results.erase(results.find(i));

					return (char*) "1";
				}
				else								{ return (char*) "-4"; }
			}
			else									{ return (char*) "-3"; }
		}
		else										{ return (char*) "-2"; }
	}
	catch (...)										{ return (char*) "-1"; }
}

#include <fstream>
extern "C" EXPORT char *get(int n, char *v[])
{
	try
	{
		if (n == 3)
		{
			unsigned int i;

			std::istringstream(std::string(v[0])) >> i;

			if (results.count(i))
			{
				unsigned int column;

				if (std::string(v[1]) == "i")		{ std::istringstream(std::string(v[2])) >> column; }
				else								{ column = results[i]->getNumber(std::string(v[2])); }

				if (column > 0)
				{
					try
					{
						std::string str				= results[i]->getString(column);

						char* buf					= new char[str.size() + 1];

						memcpy(buf, str.c_str(), str.size());

						buf[str.size()]				= 0;

						return buf;
					}
					catch (std::string str)
					{
						if (str == "Column is invalid.")
						{
							return (char*) "_<PGSQL4DM_ERROR>_-5";
						}
						else if (str == "Position is invalid.")
						{
							return (char*) "_<PGSQL4DM_ERROR>_-6";
						}
						else						{ return (char*) "_<PGSQL4DM_NULL>_"; }
					}
				}
				else								{ return (char*) "_<PGSQL4DM_ERROR>_-4"; }
			}
			else									{ return (char*) "_<PGSQL4DM_ERROR>_-3"; }
		}
		else										{ return (char*) "_<PGSQL4DM_ERROR>_-2"; }
	}
	catch (...)										{ return (char*) "_<PGSQL4DM_ERROR>_-1"; }
}

extern "C" EXPORT char *dispose(int n, char *v[]) // exec(conninfo)
{
	try
	{
		if (n == 1)
		{
			destroyPool(std::string(v[0]));

			return (char*) "1";
		}
		else										{ return (char*) "-2"; }
	}
	catch (...)										{ return (char*) "-1"; }
}
