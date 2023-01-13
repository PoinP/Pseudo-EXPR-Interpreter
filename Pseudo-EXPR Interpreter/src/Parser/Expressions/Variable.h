#ifndef VARAIBLE_H

#define VARAIBLE_H

#include "Expression.h"

/// Handles variables
class Variable : public Expression
{
public:
	/// Variable Constructor
	///
	/// Creates a variable by recieving a token
	/// @param var The token that has information about the variable's name
	Variable(const Token& var)
		: m_Var(var)
	{}

	/// Evaluates the variable's value
	/// @param env The environment in which the variable
	/// is stored in.
	/// @returns The value stored in the variable
	/// @throws RunTimeError when the variable isn't initalized
	BigInteger evaluate(Environment* env) const override
	{
		std::string varName = m_Var.getIdentifier();

		if (env->contains(varName))
			return env->get(varName)->evaluate(env);

		throw RunTimeError("The variable \"" + varName + "\" is not initalized", m_Var.getLine());
	}

private:
	/// A token that stores the variable's information
	Token m_Var;
};

#endif // !VARAIBLE_H
