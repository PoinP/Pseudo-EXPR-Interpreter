#ifndef CONDITION_H

#define CONDITION_H

#include "Instruction.h"

/// Handles conditions
class Condition : public Instruction
{
public:
	/// Condition Constructor
	///
	/// @param condition The condition
	/// of the object
	/// @param ifTrue The expression that
	/// is going to be evaluated if the condition
	/// is true
	/// @param ifFalse The expression that
	/// is going to be evaluated if the condition
	/// is false
	/// @param env The environmnet in which the 
	/// condition was made
	Condition(Expression* condition, Instruction* ifTrue, Instruction* ifFalse, Environment* env)
		: m_Condition(condition), m_IfTrue(ifTrue), m_IfFalse(ifFalse), m_Env(env)
	{}

	// Deletes the condition, ifTrue and ifFalse expression
	~Condition()
	{
		delete m_Condition;
		delete m_IfTrue;
		delete m_IfFalse;
	}


	/// Runs the instruction
	/// 
	/// @param env The Environment in which
	/// the instruction will run.
	/// @note env is a optional parameter.
	/// If not set, the instruction will run
	/// in it's default Environmnet set in
	/// Condition(Expression* condition, Instruction* ifTrue, Instruction* ifFalse, Environment* env)
	/// @note If the condition is true, then m_IfTrue is evaluated.
	/// If it is false then m_IfFalse is evaluated
	/// @throws RunTimeError if any of the expressions throw
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
	/// The condition of the instruction
	Expression* m_Condition;
	/// Evaluates if m_Condition is true
	Instruction* m_IfTrue;
	/// Evaluates if m_Condition is false
	Instruction* m_IfFalse;
	/// The environment in which the
	/// instruction was created
	Environment* m_Env;
};

#endif // !CONDITION_H
