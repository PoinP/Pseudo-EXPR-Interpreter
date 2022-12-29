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

	Expression* expression();
	Expression* equality();
	Expression* comparison();
	Expression* arithmetic();
	Expression* factor();
	Expression* unary();
	Expression* primitive();

	void next();
	const Token& consume();

	const Token& peek() const;
	TokenType peekType() const;

	void log(const char* msg) const;
};

#endif // !PARSER_H