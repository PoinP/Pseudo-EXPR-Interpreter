#ifndef RUN_TIME_ERROR_H

#define RUN_TIME_ERROR_H

#include "Error.h"

/// Handles Run-Time Errors
class RunTimeError : public Error
{
public:
	/// Used when the error isn't in the code
	RunTimeError(const char* what)
		: Error("Run-Time Error: { " + std::string(what) + " }")
	{}

	/// Used when the error is in the code
	RunTimeError(const char* what, unsigned long long line)
		: Error ("Run-Time Error: { " + std::string(what) + " } at line " + std::to_string(line))
	{}

	/// Same as RunTimeError(const char* what, unsigned long long line)
	RunTimeError(const std::string& what, unsigned long long line)
		: RunTimeError(what.c_str(), line)
	{}
};

#endif // !RUN_TIME_ERROR_H

