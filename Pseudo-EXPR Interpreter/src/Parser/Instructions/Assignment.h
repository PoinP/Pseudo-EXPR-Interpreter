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
	{}

	~Assignment()
	{
		delete m_Expr;
	}

	virtual void run() override
	{
		unsigned long long value = m_Expr->evaluate();
		Token literal = Token(TokenType::NUMBER, 0, value);
		m_Env->set(m_Name, new Primitive(literal));
	}

private:
	std::string m_Name;
	Expression* m_Expr;
	Environment* m_Env;
};

#endif // !ASSIGNMENT_H
