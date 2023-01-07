#ifndef UNARY_H

#define UNARY_H

#include "Expression.h"

class Unary : public Expression
{
public:
	Unary(const Token& op, Expression* right)
		: m_Operator(op), m_Right(right)
	{}

	~Unary()
	{
		delete m_Right;
	}

	BigInteger evaluate(Environment* env) const override
	{
		BigInteger exprValue = m_Right->evaluate(env);
		TokenType type = m_Operator.getType();

		switch (type)
		{
		case TokenType::MINUS: throw RunTimeError("Usage of negative numbers", currLine());
		case TokenType::NOT: return !exprValue;
		default: throw RunTimeError("Unexpected behavior", currLine());
		}
	}

private:
	Token m_Operator;
	Expression* m_Right;

	uint64_t currLine() const
	{
		return m_Operator.getLine();
	}
};

#endif // !UNARY_H
