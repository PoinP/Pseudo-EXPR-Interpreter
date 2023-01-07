#ifndef EXPRESSION_H

#define EXPRESSION_H

#include "../../Utility/Helper.h"
#include "../../Errors/RunTimeError.h"
#include "../../BigInteger/BigInteger.h"

class Expression
{
public:
	Expression() {}
	virtual ~Expression() {}

	virtual BigInteger evaluate(Environment* env) const = 0;

private:
	Expression(const Expression& other) = delete;
	Expression(Expression&& other) = delete;

	Expression& operator=(const Expression& other) = delete;
	Expression& operator=(Expression&& other) = delete;

};

#endif // !EXPRESSION_H