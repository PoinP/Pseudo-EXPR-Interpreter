#ifndef FUNCTION_CALLER_H

#define FUNCTION_CALLER_H

#include "Expression.h"
#include "../../Token/Token.h"
#include "../Environmnet.h"
#include "FunctionDecl.h"
#include "Primitive.h"

class FunctionCaller : public Expression
{
public:
	FunctionCaller(Token func, Expression* argument, Environment* env)
		: m_Func(func), m_Arg(argument), m_Env(env)
	{}

	unsigned long long evaluate() const override
	{
		if (!m_Env->contains(m_Func.getIdentifier()))
			throw "Error!";

		Environment copy = *m_Env;
		const Expression* func = m_Env->get(m_Func.getIdentifier());

		const FunctionDecl* funcData = static_cast<const FunctionDecl*>(func);

		unsigned long long value = m_Arg->evaluate();

		Token literal = Token(TokenType::NUMBER, 0, value);
		m_Env->set(funcData->getParamName(), new Primitive(literal));
		unsigned long long result = funcData->getBody()->evaluate();

		*m_Env = copy;

		return result;
	}

private:
	Token m_Func;
	Expression* m_Arg;
	Environment* m_Env;
};

#endif // !FUNCTION_CALLER_H
