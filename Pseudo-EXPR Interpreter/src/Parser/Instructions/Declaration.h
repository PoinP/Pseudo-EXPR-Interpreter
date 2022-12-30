#ifndef DECLARATION_H

#define DECLARATION_H

#include "../Environmnet.h"
#include "../Expressions/Expression.h"
#include "Instruction.h"
#include "../Expressions/FunctionDeclaration.h"

class Declaration : public Instruction
{
public:
	Declaration(Token name, Token param, Expression* body, Environment* env)
		: m_Name(name), m_Param(param), m_Body(body), m_Env(env)
	{
	}

	virtual void run()
	{
		FunctionDeclaration* func = new FunctionDeclaration(m_Name, m_Param, m_Body);
		m_Env->set(m_Name.getIdentifier(), func);
	}

private:
	Token m_Name;
	Token m_Param;
	Expression* m_Body;
	Environment* m_Env;
};
#endif // !DECLARATION_H
