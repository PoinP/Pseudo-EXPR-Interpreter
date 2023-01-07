#ifndef INTERPRETER_H

#define INTERPRETER_H

#include "Environmnet.h"
#include "../Parser/Instructions/Instruction.h"

class Interpreter
{
public:
	Interpreter(std::string& srcCode);
	~Interpreter();

	void interpret() const;

private:
	bool m_Status;
	Environment m_Environment;
	std::vector<Instruction*> m_Instructions;
};

#endif // !INTERPRETER_H