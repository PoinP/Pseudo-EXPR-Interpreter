#ifndef READ_H

#define READ_H

#include "Instruction.h"

class Read : public Instruction
{
public:
	Read(Token name, Environment* env)
		: m_Var(name), m_Env(env)
	{}

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
	Token m_Var;
	Environment* m_Env;
};

#endif // !READ_H