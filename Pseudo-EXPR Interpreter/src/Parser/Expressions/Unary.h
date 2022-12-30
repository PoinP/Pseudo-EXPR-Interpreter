#ifndef UNARY_H

#define UNARY_H

#include "Expression.h"
#include "../../Token/Token.h"
#include "../../Exceptions/RunTimeError.h"

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

	unsigned long long evaluate() const override
	{
		unsigned long long exprValue = m_Right->evaluate();
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

	unsigned long long currLine() const
	{
		return m_Operator.getLine();
	}
};

#endif // !UNARY_H
