#ifndef UNARY_H

#define UNARY_H

#include "Expression.h"
#include "../../Token/Token.h"

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

	unsigned long long evaluate() override
	{
		unsigned long long exprValue = m_Right->evaluate();
		TokenType type = m_Operator.getType();

		switch (type)
		{
		case TokenType::MINUS:
			return 0;
		case TokenType::NOT:
			return !exprValue;
			break;
		default:
			break;
		}
	}

private:
	Token m_Operator;
	Expression* m_Right;
};

#endif // !UNARY_H
