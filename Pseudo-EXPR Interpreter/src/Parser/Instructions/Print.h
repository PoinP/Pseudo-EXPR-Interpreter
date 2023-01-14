#ifndef PRINT_H

#define PRINT_H

#include "Instruction.h"

/// Handles printing on the console
class Print : public Instruction
{
public:
	/// Print Constructor
	///
	/// @param expr The expression that is
	/// going to be printed
	/// @param env The environment in which the
	/// instruction was created
	Print(Expression* expr, Environment* env)
		: m_Expr(expr), m_Env(env)
	{}

	/// Deletes the expression
	~Print()
	{
		delete m_Expr;
	}

	/// Runs the instruction
	/// 
	/// @param env The Environment in which
	/// the instruction will run.
	/// @note env is a optional parameter.
	/// If not set, the instruction will run
	/// in it's default Environmnet set in
	/// Print(Expression* expr, Environmnet* env)
	void run(Environment* env) override
	{
		if (!env)
			env = m_Env;

		std::cout << m_Expr->evaluate(env) << std::endl;
	}

private:
	/// The expression to be printed
	Expression* m_Expr;
	/// The environment in which the
	/// instruction was created
	Environment* m_Env;
};

#endif // !PRINT_INSTRUCTION_H