#ifndef INSTRUCTION_H

#define INSTRUCTION_H

#include "../../BigInteger/BigInteger.h"

class Instruction
{
public:
	virtual ~Instruction()
	{}

	virtual void run() = 0;
};

#endif // !INSTRUCTION_H