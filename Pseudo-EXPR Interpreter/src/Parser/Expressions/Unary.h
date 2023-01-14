#ifndef UNARY_H

#define UNARY_H

#include "Expression.h"

/// Handles unary expressions
class Unary : public Expression
{
public:
	/// Unary constructor
	///
	/// Creates a unary expression object
	/// @param op A token that stores the operator of the unary expression
	/// @param right An expression which gets evaluated by the unary expression
	Unary(const Token& op, Expression* right)
		: m_Operator(op), m_Right(right)
	{}

	/// Deletes its expression
	~Unary()
	{
		delete m_Right;
	}

	/// Evaluates the unary expression
	/// @param env The environment in which to 
	/// evaluate the expression.
	/// @returns The value of the evaluation
	/// @throws RunTime Error when the operator is the sign '-'
	/// @throws RunTime Error when an invalid operator is present
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
	/// A token that stores the expression's operator
	Token m_Operator;

	/// An expression that stores the expression that needs to get evaluated
	Expression* m_Right;

	/// Gets the current line of the opeator token hence getting the unary expression's current line
	/// @return The current line of the unary expression
	uint64_t currLine() const
	{
		return m_Operator.getLine();
	}
};

#endif // !UNARY_H
