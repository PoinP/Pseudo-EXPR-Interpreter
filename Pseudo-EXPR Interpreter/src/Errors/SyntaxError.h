#ifndef SYNTAX_ERROR_H

#define SYNTAX_ERROR_H

#include "Error.h"

/// Handles Syntax Errors
class SyntaxError : public Error
{
public:
	/// Used to throw an error that occured in the syntax of the code
	SyntaxError(const char* what, unsigned long long line)
		: Error("Syntax Error: { " + std::string(what) + " } at line " + std::to_string(line))
	{}

	/// Same as SyntaxError(const char* what, unsigned long long line)
	SyntaxError(const std::string& what, unsigned long long line)
		: SyntaxError(what.c_str(), line)
	{}
};

#endif // !SYNTAX_ERROR_H

