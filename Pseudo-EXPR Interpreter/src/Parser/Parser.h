#ifndef PARSER_H

#define PARSER_H

#include <list>
#include "../Token/Token.h"
#include "Expressions/Expression.h"
#include "Instructions/Instruction.h"
#include "../Interpreter/Environmnet.h"


class Parser
{
public:
	Parser(const std::list<Token>& tokens, Environment* environmnet);
	std::list<Instruction*> parse();
	bool isAtEnd() const;

private:
	std::list<Token> m_Tokens;
	std::list<Token>::iterator m_CurrToken;
	Environment* m_Environment;

	// -------- Instruction Handling -------- //
	Instruction* instruction();
	Instruction* variable();
	Instruction* function();
	Instruction* condition();
	Instruction* loop();
	Instruction* print();
	Instruction* read();
	void eol();

	// -------- Expression Handling -------- //
	Expression* expression();
	Expression* ifElseExpr();
	Expression* ternary();
	Expression* logicalOr();
	Expression* logicalAnd();
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