#include "DO.h"

std::string DO::getRSV(ResultSet rs, unsigned int column)								{ return getRSV(rs, column, ""); }
std::string DO::getRSV(ResultSet rs, unsigned int column, std::string defaultValue)
{
	try																					{ return rs.getString(column); }
	catch (...)																			{ return defaultValue; }
}

std::string DO::getRSV(ResultSet rs, std::string column)								{ return getRSV(rs, column, ""); }
std::string DO::getRSV(ResultSet rs, std::string column, std::string defaultValue)		{ return getRSV(rs, rs.getNumber(column), defaultValue); }

int DO::getRSVInt(ResultSet rs, unsigned int column)									{ return getRSVInt(rs, column, 0); }

int DO::getRSVInt(ResultSet rs, unsigned int column, int defaultValue)
{
	try
	{
		std::string str																	= getRSV(rs, column, "");

		if (str.empty())																{ throw "Empty value."; }
		else
		{
			int i;

			std::istringstream(str) >> i;

			return i;
		}
	}
	catch (...)																			{ return defaultValue; }
}

int DO::getRSVInt(ResultSet rs, std::string column)										{ return getRSVInt(rs, column, 0); }
int DO::getRSVInt(ResultSet rs, std::string column, int defaultValue)					{ return getRSVInt(rs, rs.getNumber(column), defaultValue); }

long long DO::getRSVLong(ResultSet rs, unsigned int column)								{ return getRSVLong(rs, column, 0); }
long long DO::getRSVLong(ResultSet rs, unsigned int column, long defaultValue)
{
	try
	{
		std::string str																	= getRSV(rs, column, "");

		if (str.empty())																{ throw "Empty value."; }
		else
		{
			long long i;

			std::istringstream(str) >> i;

			return i;
		}
	}
	catch (...)																			{ return defaultValue; }
}

long long DO::getRSVLong(ResultSet rs, std::string column)								{ return getRSVLong(rs, column, 0); }
long long DO::getRSVLong(ResultSet rs, std::string column, long defaultValue)			{ return getRSVLong(rs, rs.getNumber(column), defaultValue); }

bool DO::getRSVBool(ResultSet rs, unsigned int column)									{ return getRSVBool(rs, column, false); }
bool DO::getRSVBool(ResultSet rs, unsigned int column, bool defaultValue)
{
	try
	{
		std::string str																	= getRSV(rs, column, "");

		if (str.empty())																{ throw "Empty value."; }
		else if (str == "t" || str == "true")											{ return true; }
		else if (str == "f" || str == "false")											{ return false; }
		else																			{ throw "Invalid value."; }
	}
	catch (...)																			{ return defaultValue; }
}

bool DO::getRSVBool(ResultSet rs, std::string column)									{ return getRSVBool(rs, column, false); }
bool DO::getRSVBool(ResultSet rs, std::string column, bool defaultValue)				{ return getRSVBool(rs, rs.getNumber(column), defaultValue); }
