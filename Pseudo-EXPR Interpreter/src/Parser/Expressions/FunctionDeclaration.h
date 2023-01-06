#ifndef FUNCTION_DECLARATION_H

#define FUNCTION_DECLARATION_H

#include "Expression.h"
#include "../../Token/Token.h"

class FunctionDeclaration : public Expression 
{
public:
	FunctionDeclaration(const Token& func, const Token& param, Expression* body)
		: m_Func(func), m_Param(param), m_Body(body)
	{}

	~FunctionDeclaration() { delete m_Body; }

	const Expression* getBody() const { return m_Body; }
	const std::string& getName() const { return m_Func.getIdentifier(); }
	const std::string& getParamName() const { return m_Param.getIdentifier(); }

	BigInteger evaluate(Environment* env) const override { return 0; }

private:
	Token m_Func;
	Token m_Param;
	Expression* m_Body;
};

#endif // !FUNCTION_DECLARATION_H
