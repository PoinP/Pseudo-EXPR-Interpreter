#ifndef VARAIBLE_H

#define VARAIBLE_H

#include "Expression.h"

class Variable : public Expression
{
public:
	Variable(const Token& var)
		: m_Var(var)
	{}

	BigInteger evaluate(Environment* env) const override
	{
		std::string varName = m_Var.getIdentifier();

		if (env->contains(varName))
			return env->get(varName)->evaluate(env);

		throw RunTimeError("The variable \"" + varName + "\" is not initalized", m_Var.getLine());
	}

private:
	Token m_Var;
};

#endif // !VARAIBLE_H
