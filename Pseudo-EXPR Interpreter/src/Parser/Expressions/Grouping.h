#ifndef GROUPING_H

#define GROUPING_H

#include "Expression.h"

/// Handles grouping(brackets) expressions
class Grouping : public Expression
{
public:
	/// Grouping Consructor
	///
	/// @param expr The expression that
	/// is put in brackets
	Grouping(Expression* expr)
		: m_Expr(expr)
	{}

	~Grouping()
	{
		delete m_Expr;
	}

	/// Evaluates the internal expression
	/// 
	/// @param env The environmnet in which
	/// the internal expression(m_Expr) will run
	/// @returns The evaluation of m_Expr
	BigInteger evaluate(Environment* env) const override
	{
		return m_Expr->evaluate(env);
	}

private:
	/// The internal expression
	Expression* m_Expr;
};

#endif // !GROUPING_H
