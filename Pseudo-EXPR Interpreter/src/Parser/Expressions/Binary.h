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
		unsigned long long left = m_Left->evaluate();
		unsigned long long right = m_Right->evaluate();

		TokenType type = m_Operator.getType();

		switch (type)
		{
		// ---------- Artithmetic ---------- 

		case TokenType::PLUS: return left + right;
		case TokenType::PROD: return left * right;

		case TokenType::MINUS:
			if (left < right) throw RunTimeError("Usage of negative numbers", currLine());
			return left - right;

		case TokenType::DIV:
			if (right == 0) throw RunTimeError("Division by zero", currLine());
			return left / right;

		case TokenType::MOD:
			if (right == 0) throw RunTimeError("Division by zero", currLine());
			return left % right;

		// ---------- Comparison ----------

		case TokenType::LESS_THAN: return left < right;
		case TokenType::GREATER_THAN: return left > right;
		case TokenType::LESS_EQUAL: return left <= right;
		case TokenType::GREATER_EQUAL: return left >= right;

		// ---------- Equality ----------

		case TokenType::EQUAL_EQUAL: return left == right;
		case TokenType::NOT_EQUAL: return left != right;

		default: throw RunTimeError("Unexpected behavior", currLine());
		}
	}

private:
	Expression* m_Left;
	Token m_Operator;
	Expression* m_Right;

	unsigned long long currLine() const
	{
		return m_Operator.getLine();
	}
};

#endif // !BINARY_H
