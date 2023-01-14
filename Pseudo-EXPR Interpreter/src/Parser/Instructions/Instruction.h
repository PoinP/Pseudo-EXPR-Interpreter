#ifndef INSTRUCTION_H

#define INSTRUCTION_H

#include "../../Interpreter/Environmnet.h"

class Instruction
{
public:
	Instruction()
	{}
	virtual ~Instruction()
	{}

	virtual void run(Environment* env = nullptr) = 0;

private:
	Instruction(const Instruction& other) = delete;
	Instruction(Instruction&& other) = delete;

	Instruction& operator= (const Instruction& other) = delete;
	Instruction& operator= (Instruction&& other) = delete;
};

#endif // !INSTRUCTION_H