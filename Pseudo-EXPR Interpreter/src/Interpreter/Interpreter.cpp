#include "Interpreter.h"

#include "../Parser/Parser.h"
#include "../Token/Tokenizer.h"
#include "../Errors/RunTimeError.h"
#include "../Errors/SyntaxError.h"

Interpreter::Interpreter(const std::string& srcCode)
	: m_Status(true)
{
	Tokenizer lexer(srcCode);
	std::vector<Token> tokens;

	while (!lexer.isAtEnd())
	{
		try
		{
			tokens = lexer.tokenize();
		}
		catch (const SyntaxError& error)
		{
			std::cout << error.what() << std::endl;
			m_Status = false;
		}
	}

	Parser parser(tokens, &m_Environment);
	while (!parser.isAtEnd())
	{
		try
		{
			m_Instructions = parser.parse();
		}
		catch (const SyntaxError& error)
		{
			std::cout << error.what() << std::endl;
			m_Status = false;
		}
	}
}

Interpreter::~Interpreter()
{
	for (Instruction* instruction : m_Instructions)
	{
		delete instruction;
	}
}

void Interpreter::interpret() const
{
	if (!m_Status)
		return;

	for (Instruction* instruction : m_Instructions)
	{
		try
		{
			instruction->run();
		}
		catch (const RunTimeError& error)
		{
			std::cout << error.what() << std::endl;
			return;
		}
	}
}
