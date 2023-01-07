#ifndef INSTRUCTION_H

#define INSTRUCTION_H

#include "../../Interpreter/Environmnet.h"

class Instruction
{
public:
	virtual ~Instruction()
	{}

	virtual void run(Environment* env = nullptr) = 0;
};

#endif // !INSTRUCTION_H