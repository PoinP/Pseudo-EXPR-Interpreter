#ifndef TERNARY_H

#define TERNARY_H

#include "Expression.h"

/// Handles ternary expressions
class Ternary : public Expression
{
public:
	/// Ternary Constructor
	///
	/// Creates a Ternary expression based on a condition and outputs
	/// based on the value of the condition
	/// @param condition The condition of the ternary operator
	/// @param ifTrue Evaluates if condition is true
	/// @param ifFalse Evaluates if condition is false
	Ternary(Expression* condition, Expression* ifTrue, Expression* ifFalse)
		: m_Condition(condition), m_IfTrue(ifTrue), m_IfFalse(ifFalse)
	{}

	/// Deletes all of the expressions
	/// stored in it
	~Ternary()
	{
		delete m_Condition;
		delete m_IfTrue;
		delete m_IfFalse;
	}

	/// Evaluates the condition and based on the result
	/// evalauets the m_IfTrue or m_IfFalse expression
	/// 
	/// @param env The environment in which the ternary
	/// expression will be evaluated
	/// @returns The value of m_IfTrue if the condition is true.
	/// If it's false, then the value of m_IfFalse is returned
	BigInteger evaluate(Environment* env) const override
	{
		return m_Condition->evaluate(env) ? m_IfTrue->evaluate(env) : m_IfFalse->evaluate(env);
	}

private:
	/// Condition expression
	Expression* m_Condition;
	/// The expression which evaluates if
	/// the condition is true
	Expression* m_IfTrue;
	/// The expression which evaluates if
	/// the condition is false
	Expression* m_IfFalse;
};

#endif // !TERNARY_H