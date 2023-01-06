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
	{}

	~Binary()
	{
		delete m_Left;
		delete m_Right;
	}

	BigInteger evaluate(Environment* env) const override
	{
		BigInteger left = m_Left->evaluate(env);
		BigInteger right = m_Right->evaluate(env);

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

		// ---------- Logical ----------

		case TokenType::AND: return left && right;
		case TokenType::OR: return left || right;

		default: throw RunTimeError("Unexpected behavior", currLine());
		}
	}

private:
	Expression* m_Left;
	Token m_Operator;
	Expression* m_Right;

	uint64_t currLine() const
	{
		return m_Operator.getLine();
	}
};

#endif // !BINARY_H
