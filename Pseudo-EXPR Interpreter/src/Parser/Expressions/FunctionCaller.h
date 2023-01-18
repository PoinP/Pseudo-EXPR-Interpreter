#ifndef FUNCTION_CALLER_H

#define FUNCTION_CALLER_H

#include "Expression.h"
#include "FunctionDeclaration.h"
class FunctionCaller : public Expression
{
public:
	FunctionCaller(const Token& func, Expression* arg)
		: m_Func(func), m_Arg(arg)
	{}

	~FunctionCaller()
	{
		delete m_Arg;
	}

	BigInteger evaluate(Environment* env) const override
	{
		Environment functionScope(env);

		if (!functionScope.contains(m_Func.getIdentifier()))
			throw RunTimeError("Function \"" + m_Func.getIdentifier() + "\" undefined", m_Func.getLine());

		const Expression* funcExpr = functionScope.get(m_Func.getIdentifier());
		const FunctionDeclaration* func = dynamic_cast<const FunctionDeclaration*>(funcExpr);

		if (!func)
			throw RunTimeError("Internal error! Expression not a function!", m_Func.getLine());

		BigInteger argValue = m_Arg->evaluate(&functionScope);
		functionScope.setOnSelf(func->getParamName(), new Primitive(argValue));

		return func->getBody()->evaluate(&functionScope);
	}

private:
	Token m_Func;
	Expression* m_Arg;
};

#endif // !FUNCTION_CALLER_H
