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

	void run() override
	{
		std::cin >> m_Env->operator[](m_Var->getIdentifier());
	}

private:
	const Token* m_Var;
	Environment* m_Env;
};

#endif // !READ_H