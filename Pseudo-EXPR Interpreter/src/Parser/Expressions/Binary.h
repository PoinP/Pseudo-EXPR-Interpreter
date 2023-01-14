#ifndef BINARY_H

#define BINARY_H

#include "Expression.h"

/// Handles binary expressions
class Binary : public Expression
{
public:
	/// Binary Constructor
	///
	/// Handles all of the supported binary
	/// expressions of the language
	/// @param left The left expression
	/// @param op The operator used for binary evaluation
	/// @param right the right expression 
	Binary(Expression* left, const Token& op, Expression* right)
		: m_Left(left), m_Operator(op), m_Right(right)
	{}

	// Deletes the left and right expression
	~Binary()
	{
		delete m_Left;
		delete m_Right;
	}

	/// Evaluates the binary expression
	///
	/// It evaluates the expression by 
	/// evaluating the left and right side
	/// and then based on the operator does
	/// the evaluation.
	/// @param The Environment in which it will be evaluated
	/// @returns The evaluation's result
	/// @throws RunTimeError if subtraction causes the result
	/// to be negative
	/// @throws RunTimeError if trying to divide by zero
	/// @throws RunTimeError if the token is invalid
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
	/// The left expression
	Expression* m_Left;
	/// The operator token
	Token m_Operator;
	/// The right expression
	Expression* m_Right;

	/// @returns the line that the operator
	/// token is located at
	uint64_t currLine() const
	{
		return m_Operator.getLine();
	}
};

#endif // !BINARY_H
