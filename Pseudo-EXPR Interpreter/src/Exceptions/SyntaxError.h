#ifndef SYNTAX_ERROR_H

#define SYNTAX_ERROR_H

#include <stdexcept>

class SyntaxError : public std::exception
{
	SyntaxError(const char* what, unsigned long long line);
	SyntaxError(const std::string& what, unsigned long long line);
};

#endif // !SYNTAX_ERROR_H

