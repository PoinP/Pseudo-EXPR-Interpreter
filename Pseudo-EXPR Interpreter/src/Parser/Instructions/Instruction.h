#ifndef INSTRUCTION_H

#define INSTRUCTION_H

#include "../../Interpreter/Environmnet.h"

/// Instruction interface
///
/// This is the interface that is
/// inherited by all of the other
/// instructions.
/// @note The copy and move constructors /
/// assignment operators are deleted
class Instruction
{
public:
	Instruction()
	{}
	virtual ~Instruction()
	{}

	/// Runs an instruction
	/// @param env The Environmnt in which
	/// the instruction will run
	virtual void run(Environment* env = nullptr) = 0;

private:
	/// Deleted constructor
	Instruction(const Instruction& other) = delete;
	/// Deleted constructor
	Instruction(Instruction&& other) = delete;

	/// Deleted assignment
	Instruction* operator=(const Instruction& other) = delete;
	/// Deleted assignment
	Instruction* operator=(Instruction&& other) = delete;
};

#endif // !INSTRUCTION_H