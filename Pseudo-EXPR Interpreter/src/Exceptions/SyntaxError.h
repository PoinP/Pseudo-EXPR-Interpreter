#ifndef SYNTAX_ERROR_H

#define SYNTAX_ERROR_H

#include "Error.h"

class SyntaxError : public Error
{
public:
	SyntaxError(const char* what, unsigned long long line)
		: Error("Run-Time Error: " + std::string(what) + " at line " + std::to_string(line))
	{}

	SyntaxError(const std::string& what, unsigned long long line)
		: SyntaxError(what.c_str(), line)
	{}
};

#endif // !SYNTAX_ERROR_H

