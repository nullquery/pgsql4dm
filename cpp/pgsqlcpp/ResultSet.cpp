#include "ResultSet.h"
#include <algorithm>

void ResultSet::init(PGresult* res)
{
	int nFields													= PQnfields(res);

	for (int i = 0; i < nFields; i = i + 1)						{ this->columns.push_back(PQfname(res, i)); }

	int nTuples													= PQntuples(res);
	std::vector<std::string*>* v;
	char* value;

	for (int i = 0; i < nTuples; i = i + 1)
	{
		v														= new std::vector<std::string*>();

		for (int j = 0; j < nFields; j++)
		{
			if (PQgetisnull(res, i, j))							{ v->push_back(nullptr); }
			else
			{
				value											= PQgetvalue(res, i, j);

				v->push_back(new std::string(value, PQgetlength(res, i, j)));
			}
		}

		this->rows.push_back(v);
	}
}

int ResultSet::size()											{ return this->rows.size(); }
bool ResultSet::next()											{ return (++this->position < this->size()); }
bool ResultSet::previous()										{ return (--this->position >= 0 && this->size() > 0); }
bool ResultSet::isFirst()										{ return this->size() > 0 && this->position == 0; }
bool ResultSet::isLast()										{ return this->size() > 0 && this->position == (this->size() - 1); }
bool ResultSet::isBeforeFirst()									{ return this->position == -1; }
bool ResultSet::isAfterLast()									{ return this->position == this->size(); }
void ResultSet::first()											{ this->position = this->size() > 0 ? 0 : -1; }
void ResultSet::last()											{ this->position = this->size() - 1; }

std::string ResultSet::getString(unsigned int column)
{
	if (position > -1 && position < this->size())
	{
		if (column > 0 && column <= this->columns.size())		{ return *((std::string*) (*this->rows[this->position])[column - 1]); }
		else													{ throw std::string("Column is invalid."); }
	}
	else														{ throw std::string("Position is invalid."); }
}

int ResultSet::getNumber(std::string column)
{
	unsigned int pos											= find(this->columns.begin(), this->columns.end(), column) - this->columns.begin();

	if (pos >= 0 && pos < this->columns.size())
	{
		return pos + 1;
	}
	else
	{
		return 0;
	}
}
