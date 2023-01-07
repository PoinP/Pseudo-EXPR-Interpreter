#ifndef READ_H

#define READ_H

#include "Instruction.h"
#include "../../Exceptions/RunTimeError.h"
#include "../Expressions/Expression.h"
#include "../Environmnet.h"
#include "../../Token/Token.h"
#include <iostream>


class Read : public Instruction
{
public:
	Read(const Token* name, Environment* env)
		: m_Var(name), m_Env(env)
	{}

	void run(Environment* env) override
	{
		if (!env)
			env = m_Env;

		BigInteger value;
		std::cin >> value;
		env->set(m_Var->getIdentifier(), new Primitive(value));
	}

private:
	const Token* m_Var;
	Environment* m_Env;
};

#endif // !READ_H