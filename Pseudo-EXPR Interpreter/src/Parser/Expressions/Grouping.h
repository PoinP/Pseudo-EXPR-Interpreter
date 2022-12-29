#ifndef GROUPING_H

#define GROUPING_H

#include "Expression.h"
#include "../../Token/Token.h"

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

	unsigned long long evaluate() override
	{
		return m_Expr->evaluate();
	}

private:
	Expression* m_Expr;
};

#endif // !GROUPING_H
