#ifndef DECLARATION_H

#define DECLARATION_H

#include "Instruction.h"

/// Handles function declarations
class Declaration : public Instruction
{
public:
	/// Declaration Constructor
	///
	/// Creates a function declaration instruction
	/// @param name The function token
	/// @param param The parameter token
	/// @param body The body of the function
	/// @param env The Environment in which the
	/// instruction was made
	Declaration(Token name, Token param, Expression* body, Environment* env)
		: m_Name(name), m_Param(param), m_Body(body), m_Env(env)
	{
	}

	/// Deletes the body of the function
	~Declaration()
	{
		delete m_Body;
	}

	/// Runs the instruction
	/// 
	/// Creates a FunctionDeclaration expression
	/// which is saved to the Environment
	/// @param env The Environment in which
	/// the instruction will run.
	/// @note env is a optional parameter.
	/// If not set, the instruction will run
	/// in it's default Environmnet set in
	/// Declaration(Token name, Token param, Expression* body, Environment* env)
	void run(Environment* env) override
	{
		if (!env)
			env = m_Env;

		FunctionDeclaration* func = new FunctionDeclaration(m_Name, m_Param, m_Body);
		env->setOnSelf(m_Name.getIdentifier(), func);
	}

private:
	/// The function's token
	Token m_Name;
	/// The parameter's token
	Token m_Param;
	/// The expression of the body
	Expression* m_Body;
	/// The environment in which the
	/// instruction was created
	Environment* m_Env;
};
#endif // !DECLARATION_H
