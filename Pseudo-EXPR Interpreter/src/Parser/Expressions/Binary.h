#ifndef BINARY_H

#define BINARY_H

#include "Expression.h"
#include "../../Token/Token.h"
#include "../../Exceptions/RunTimeError.h"

#include <iostream>

class Binary : public Expression
{
public:
	Binary(Expression* left, const Token& op, Expression* right)
		: m_Left(left), m_Operator(op), m_Right(right)
	{

	}

	~Binary()
	{
		delete m_Left;
		delete m_Right;
	}

	unsigned long long evaluate() override
	{
		// Add errors

		unsigned long long left = m_Left->evaluate();
		unsigned long long right = m_Right->evaluate();

		TokenType type = m_Operator.getType();

		switch (type)
		{
		case TokenType::PLUS:
			return left + right;
			break;
		case TokenType::MINUS:
			return left - right;
			break;
		case TokenType::PROD:
			return left * right;
			break;
		case TokenType::DIV:
			if (right == 0)
				throw RunTimeError("Division by zero!", m_Operator.getLine());

			return left / right;
			break;
		default:
			break;
		}
	}

private:
	Expression* m_Left;
	Token m_Operator;
	Expression* m_Right;
};

#endif // !BINARY_H
