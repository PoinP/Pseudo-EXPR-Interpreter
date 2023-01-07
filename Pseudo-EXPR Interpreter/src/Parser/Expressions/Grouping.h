#ifndef GROUPING_H

#define GROUPING_H

#include "Expression.h"

class Grouping : public Expression
{
public:
	Grouping(Expression* expr)
		: m_Expr(expr)
	{}

	~Grouping()
	{
		delete m_Expr;
	}

	BigInteger evaluate(Environment* env) const override
	{
		return m_Expr->evaluate(env);
	}

private:
	Expression* m_Expr;
};

#endif // !GROUPING_H
