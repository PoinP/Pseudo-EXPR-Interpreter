#ifndef PRINT_H

#define PRINT_H

#include "Instruction.h"

class Print : public Instruction
{
public:
	Print(Expression* expr, Environment* env)
		: m_Expr(expr), m_Env(env)
	{}

	~Print()
	{
		delete m_Expr;
	}

	void run(Environment* env) override
	{
		if (!env)
			env = m_Env;

		std::cout << m_Expr->evaluate(env) << std::endl;
	}

private:
	Expression* m_Expr;
	Environment* m_Env;
};

#endif // !PRINT_INSTRUCTION_H