#ifndef LOOP_H

#define LOOP_H

#include "Instruction.h"

/// Handles while loops
class Loop : public Instruction
{
public:
	/// Loop Contrsuctor
	///
	/// Takes a condition, a collection of instructions
	/// and an Environment. This creates the base of the
	/// loop instruction.
	/// @param condtion The condition of the loop
	/// @param instructions The collection of instructions
	/// which will be ran during the loop
	/// @param env The environmnet in which the loop was made
	Loop(Expression* condition, const std::vector<Instruction*>& instructions, Environment* env)
		: m_Condition(condition), m_Instructions(instructions), m_Env(env)
	{}

	/// Deletes the condition and all of the instructions
	~Loop()
	{
		delete m_Condition;

		for (Instruction* instruction : m_Instructions)
		{
			delete instruction;
		}
	}

	/// Runs the instruction
	/// 
	/// @param env The Environment in which
	/// the instruction will run.
	/// @note env is a optional parameter.
	/// If not set, the instruction will run
	/// in it's default Environmnet set in
	/// Loop(Expression* condition, const std::vector<Instruction*>& instructions, Environment* env)
	/// @note Before running the loop, a local scope Environmnet is created
	/// which also allows nested loops
	/// @throws RunTimeError if any of the instructions throw
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
	/// The conditon which is checked
	/// on every iteration
	Expression* m_Condition;
	/// The environment in which the
	/// instruction was created
	Environment* m_Env;
	/// The collection of instructions
	/// which are ran during the loop
	std::vector<Instruction*> m_Instructions;
};

#endif // !LOOP_H
