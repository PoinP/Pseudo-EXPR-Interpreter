#ifndef INSTRUCTION_H

#define INSTRUCTION_H

class Instruction
{
public:
	virtual ~Instruction()
	{}

	virtual void run() = 0;
};

#endif // !INSTRUCTION_H
