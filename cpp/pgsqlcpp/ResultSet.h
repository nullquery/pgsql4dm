#ifndef SRC_RESULTSET_H_
#define SRC_RESULTSET_H_

#include <libpq-fe.h>
#include <vector>
#include <map>

class ResultSet
{
private:
	std::vector<std::string> columns;
	std::vector<std::vector<std::string*>*> rows;
	int position								= -1;
	int size();
public:
	void init(PGresult* res);
	bool next();
	bool previous();
	bool isFirst();
	bool isLast();
	bool isBeforeFirst();
	bool isAfterLast();
	void first();
	void last();
	std::string getString(unsigned int column);
	int getNumber(std::string column);
};

#endif