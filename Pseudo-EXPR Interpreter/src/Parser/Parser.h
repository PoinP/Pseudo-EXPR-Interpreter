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

private:
	std::list<Token> m_Tokens;
	std::list<Token>::iterator m_CurrToken;
	Environment m_Environment;

	// -------- Instruction Handling -------- //
	Instruction* instruction();
	Instruction* variable();
	Instruction* function();
	Instruction* condition();
	Instruction* print();
	Instruction* read();
	void eol();

	// -------- Expression Handling -------- //
	Expression* expression();
	Expression* ifElseExpr();
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
	uint64_t peekLine() const;

	void log(const char* msg) const;
};

#endif // !PARSER_H