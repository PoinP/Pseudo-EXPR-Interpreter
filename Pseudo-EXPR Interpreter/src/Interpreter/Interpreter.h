#ifndef INTERPRETER_H

#define INTERPRETER_H

#include "Environmnet.h"
#include "../Parser/Instructions/Instruction.h"

/// The actual Interpreter
///
/// It combined all of the classes
/// together to form the Interpreter.
/// It stores in itself the instructions
/// that actually make the code run.
class Interpreter
{
public:
	/// Interpreter Constructor
	///
	/// Gets the source code as an arguments
	/// and creates a Tokenizer object which
	/// generates all of the tokens inside
	/// the source code. Those tokens are then
	/// parsed to a Parser object which in return
	/// parses all of the tokens and creates
	/// instructions for the Interpreter to
	/// run. All of the exceptions thrown
	/// are handled within it and spit out
	/// to the console. If an exception is
	/// throws, m_Status gets set to false
	Interpreter(const std::string& srcCode);

	/// Deletes all of the parsed instructions
	~Interpreter();

	/// Runs the program
	///
	/// Goes though all of the instructions
	/// and runs them one by one. If an
	/// instruction throws an error
	/// then interpret() ends and the
	/// error message is printed on the console
	/// @note If m_Status is false then
	/// the code won't run at all
	void interpret() const;

private:
	/// The status of the Interpreter
	bool m_Status;
	/// The global Environmnet
	Environment m_Environment;
	/// A collection of all of the instructions
	std::vector<Instruction*> m_Instructions;
};

#endif // !INTERPRETER_H