#ifndef ASSIGNMENT_H

#define ASSIGNMENT_H

#include "Instruction.h"
#include "../Environmnet.h"
#include <string>
#include "../Expressions/Expression.h"

class Assignment : public Instruction
{
public:
	Assignment(const std::string& name, Expression* expr, Environment* env)
		: m_Name(name), m_Expr(expr), m_Env(env)
	{

	}

	~Assignment()
	{
		delete m_Expr;
	}

	virtual void run() override
	{
		unsigned long long value = m_Expr->evaluate();
		m_Env->operator[](m_Name) = value;
	}

private:
	std::string m_Name;
	Expression* m_Expr;
	Environment* m_Env;

};

#endif // !ASSIGNMENT_H
