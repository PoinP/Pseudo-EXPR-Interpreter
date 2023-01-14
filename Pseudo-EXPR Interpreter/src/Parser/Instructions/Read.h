#ifndef READ_H

#define READ_H

#include "Instruction.h"

/// Handles reading from the console
class Read : public Instruction
{
public:
	/// Read Constructor
	///
	/// @param name A variable's token
	/// @param env The Environment in which
	/// the instruction was created
	Read(Token name, Environment* env)
		: m_Var(name), m_Env(env)
	{}

	/// Runs the instruction
	/// 
	/// @param env The Environment in which
	/// the instruction will run.
	/// @note env is a optional parameter.
	/// If not set, the instruction will run
	/// in it's default Environmnet set in
	/// Read(Token name, Environmnet* env)
	/// @throws RunTimeError if an invalid
	/// varibale is passed during run-time
	void run(Environment* env) override
	{
		if (!env)
			env = m_Env;

		BigInteger value;
		try { std::cin >> value; }
		catch (const std::invalid_argument& ex) { throw RunTimeError(ex.what()); }
		env->set(m_Var.getIdentifier(), new Primitive(value));
	}

private:
	/// The variable's token
	Token m_Var;
	/// A pointer to an Environmnet
	Environment* m_Env;
};

#endif // !READ_H