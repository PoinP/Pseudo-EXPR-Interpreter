#ifndef PARSER_H

#define PARSER_H

#include <list>
#include "../Token/Token.h"
#include "Expressions/Expression.h"
#include "Instructions/Instruction.h"
#include "Environmnet.h"


class Parser
{
public:
	Parser(const std::list<Token>& tokens);
	std::list<Instruction*> parse();

	unsigned long long evaluate(Expression* expressions);

private:
	std::list<Token> m_Tokens;
	std::list<Token>::iterator m_CurrToken;
	Environment m_Environment;

	// -------- Instruction Handling -------- //
	Instruction* variable();
	Instruction* function();
	Instruction* print();
	Instruction* read();
	void eol();

	// -------- Expression Handling -------- //
	Expression* expression();
	Expression* ternary();
	Expression* logical();
	Expression* equality();
	Expression* comparison();
	Expression* arithmetic();
	Expression* factor();
	Expression* unary();
	Expression* primitive();

	void next();
	const Token& consume();
	TokenType consumeType();

	void skipLine();

	const Token& peek() const;
	TokenType peekType() const;
	unsigned long long peekLine() const;

	void log(const char* msg) const;
};

#endif // !PARSER_H