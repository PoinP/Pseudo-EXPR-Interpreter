#ifndef LOG_H

#define LOG_H

#include <iostream>
#include "../Token/Token.h"

class Log
{
public:
	static void syntaxError(const char* what, unsigned long long line)
	{
		std::cout << "Syntax Error: " << what << " at line " << line << std::endl;
	}
};

#endif // !LOG_H
