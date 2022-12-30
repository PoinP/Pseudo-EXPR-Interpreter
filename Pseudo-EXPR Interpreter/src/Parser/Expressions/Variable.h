#ifndef VARAIBLE_H

#define VARAIBLE_H

#include "Expression.h"
#include "../../Token/Token.h"
#include "../Environmnet.h"

#include "../../Exceptions/RunTimeError.h"

class Variable : public Expression
{
public:
	Variable(const Token& var, const Environment* env)
		: m_Var(var), m_Env(env)
	{}

private:
	Token m_Var;
	const Environment* m_Env;

	virtual unsigned long long evaluate() const override
	{
		std::string varName = m_Var.getIdentifier();

		if (m_Env->contains(varName))
			return m_Env->get(varName)->evaluate();

		throw RunTimeError("The variable \"" + varName + "\" is not initalized", m_Var.getLine());
	}
};

#endif // !VARAIBLE_H
