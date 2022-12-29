#ifndef RUN_TIME_ERROR_H

#define RUN_TIME_ERROR_H

#include "Error.h"

class RunTimeError : public Error
{
public:
	RunTimeError(const char* what, unsigned long long line)
		: Error ("Run-Time Error: " + std::string(what) + " at line " + std::to_string(line))
	{}

	RunTimeError(const std::string& what, unsigned long long line)
		: RunTimeError(what.c_str(), line)
	{}
};

#endif // !RUN_TIME_ERROR_H

