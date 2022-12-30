#ifndef FUNCTION_DECL_H

#define FUNCTION_DECL_H

#include "Expression.h"
#include "../Environmnet.h"
#include "Variable.h"
#include <string>

class FunctionDecl : public Expression
{
public:
	FunctionDecl(Token name, Token param, Expression* body)
		: m_Name(name), m_Param(param), m_Body(body)
	{}

	~FunctionDecl()
	{
		delete m_Body;
	}

	const std::string& getName() const
	{
		return m_Name.getIdentifier();
	}

	const std::string& getParamName() const
	{
		return m_Param.getIdentifier();
	}

	const Expression* getBody() const
	{
		return m_Body;
	}

	unsigned long long evaluate() const override
	{
		return 0;
	}

private:
	Token m_Name;
	Token m_Param;
	Expression* m_Body;
};

#endif // !FUNCTION_DECL_H
