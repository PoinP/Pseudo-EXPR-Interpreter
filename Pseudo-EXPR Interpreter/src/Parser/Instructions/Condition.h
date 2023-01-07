#ifndef CONDITION_H

#define CONDITION_H

#include "Instruction.h"

class Condition : public Instruction
{
public:
	Condition(Expression* condition, Instruction* ifTrue, Instruction* ifFalse, Environment* env)
		: m_Condition(condition), m_IfTrue(ifTrue), m_IfFalse(ifFalse), m_Env(env)
	{}

	~Condition()
	{
		delete m_Condition;
		delete m_IfTrue;
		delete m_IfFalse;
	}

	virtual void run(Environment* env) override
	{
		if (!env)
			env = m_Env;

		if (m_Condition->evaluate(env))
			m_IfTrue->run();
		else
			m_IfFalse->run();
	}

private:
	Expression* m_Condition;
	Instruction* m_IfTrue;
	Instruction* m_IfFalse;
	Environment* m_Env;
};

#endif // !CONDITION_H
