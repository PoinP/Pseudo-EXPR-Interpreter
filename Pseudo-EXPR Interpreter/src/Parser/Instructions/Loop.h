#ifndef LOOP_H

#define LOOP_H

#include "Instruction.h"

class Loop : public Instruction
{
public:
	Loop(Expression* condition, const std::vector<Instruction*>& instructions, Environment* env)
		: m_Condition(condition), m_Instructions(instructions), m_Env(env)
	{}

	~Loop()
	{
		delete m_Condition;

		for (Instruction* instruction : m_Instructions)
		{
			delete instruction;
		}
	}

	void run(Environment* env) override
	{
		if (!env)
			env = m_Env;

		Environment loopEnv(env);

		while (m_Condition->evaluate(&loopEnv))
		{
			for (Instruction* instruction : m_Instructions)
			{
				instruction->run(&loopEnv);
			}
		}
	}
private:
	Expression* m_Condition;
	Environment* m_Env;
	std::vector<Instruction*> m_Instructions;
};

#endif // !LOOP_H
