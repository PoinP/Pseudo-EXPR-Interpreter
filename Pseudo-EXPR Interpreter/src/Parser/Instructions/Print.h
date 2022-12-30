#ifndef PRINT_H

#define PRINT_H

#include "Instruction.h"

class Print : public Instruction
{
public:
	Print(Expression* expr)
		: m_Expr(expr)
	{}

	~Print()
	{
		delete m_Expr;
	}

	void run() override
	{
		std::cout << m_Expr->evaluate() << std::endl;
	}

private:
	Expression* m_Expr;
};

#endif // !PRINT_INSTRUCTION_H