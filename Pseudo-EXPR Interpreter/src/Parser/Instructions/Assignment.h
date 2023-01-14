#ifndef ASSIGNMENT_H

#define ASSIGNMENT_H

#include "Instruction.h"

/// Handles Assignments(for variables)
class Assignment : public Instruction
{
public:
	/// Assignment Constructor
	///
	/// @param name The name of the variable
	/// that is being assigned
	/// @param expr The expression that is
	/// assigned to the variable
	/// @param env The environmnet in which the 
	/// assignment was made
	Assignment(const std::string& name, Expression* expr, Environment* env)
		: m_Name(name), m_Expr(expr), m_Env(env)
	{
	}

	/// Deletes the expression
	~Assignment()
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
	/// Assignment(const std::string& name, Expression* expr, Environment* env)
	/// @throws RunTimeError if m_Expr throws
	virtual void run(Environment* env) override
	{
		if (!env)
			env = m_Env;

		BigInteger value = m_Expr->evaluate(env);
		env->set(m_Name, new Primitive(value));
	}

private:
	/// The name of the variable
	std::string m_Name;
	/// The expression being
	/// assigned to the variable
	Expression* m_Expr;
	/// The environment in which the
	/// instruction was created
	Environment* m_Env;
};

#endif // !ASSIGNMENT_H