#ifndef TERNARY_H

#define TERNARY_H

#include "Expression.h"
#include "../../Token/Token.h"
#include "../../Exceptions/RunTimeError.h"

class Ternary : public Expression
{
public:
	Ternary(Expression* condition, Expression* ifTrue, Expression* ifFalse)
		: m_Condition(condition), m_IfTrue(ifTrue), m_IfFalse(ifFalse)
	{}

	~Ternary()
	{
		delete m_Condition;
		delete m_IfTrue;
		delete m_IfFalse;
	}

	unsigned long long evaluate(Environment* env) const override
	{
		return m_Condition->evaluate(env) ? m_IfTrue->evaluate(env) : m_IfFalse->evaluate(env);
	}

private:
	Expression* m_Condition;
	Expression* m_IfTrue;
	Expression* m_IfFalse;
};

#endif // !TERNARY_H