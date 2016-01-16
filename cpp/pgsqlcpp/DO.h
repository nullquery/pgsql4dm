#ifndef SRC_DO_H_
#define SRC_DO_H_

#include "ResultSet.h"
#include <string>
#include <sstream>

class DO
{
public:
	static std::string getRSV(ResultSet rs, unsigned int column);
	static std::string getRSV(ResultSet rs, unsigned int column, std::string defaultValue);
	static std::string getRSV(ResultSet rs, std::string column);
	static std::string getRSV(ResultSet rs, std::string column, std::string defaultValue);

	static int getRSVInt(ResultSet rs, unsigned int column);
	static int getRSVInt(ResultSet rs, unsigned int column, int defaultValue);
	static int getRSVInt(ResultSet rs, std::string column);
	static int getRSVInt(ResultSet rs, std::string column, int defaultValue);

	static long long getRSVLong(ResultSet rs, unsigned int column);
	static long long getRSVLong(ResultSet rs, unsigned int column, long defaultValue);
	static long long getRSVLong(ResultSet rs, std::string column);
	static long long getRSVLong(ResultSet rs, std::string column, long defaultValue);

	static bool getRSVBool(ResultSet rs, unsigned int column);
	static bool getRSVBool(ResultSet rs, unsigned int column, bool defaultValue);
	static bool getRSVBool(ResultSet rs, std::string column);
	static bool getRSVBool(ResultSet rs, std::string column, bool defaultValue);

	// TODO: getRSVBD, getRSVDate, getRSVTime, getRSVDateTime, getRSVBytes, getRSVArray (getRSV*Array)
	// TODO: getDBV*
};

#endif