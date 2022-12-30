#ifndef EXPRESSION_H

#define EXPRESSION_H

#include "../../Utility/Common.h"

class Expression
{
public:
	Expression() {}
	virtual ~Expression() {}

	virtual unsigned long long evaluate(Environment* env) const = 0;

private:
	Expression(const Expression& other) = delete;
	Expression(Expression&& other) = delete;

	Expression& operator=(const Expression& other) = delete;
	Expression& operator=(Expression&& other) = delete;

};

#endif // !EXPRESSION_H