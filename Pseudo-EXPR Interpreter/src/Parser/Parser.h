#ifndef PARSER_H

#define PARSER_H

#include <list>
#include "../Token/Token.h"
#include "Expressions/Expression.h"

class Parser
{
public:
	Parser(const std::list<Token>& tokens);
	Expression* parse();

	unsigned long long evaluate(Expression* expressions);

private:
	std::list<Token> m_Tokens;
	std::list<Token>::iterator m_CurrToken;

	Expression* expression();
	Expression* equality();
	Expression* comparison();
	Expression* arithmetic();
	Expression* factor();
	Expression* unary();
	Expression* primitive();

	void next();
	const Token& consume();

	const Token& peek();
	const Token& peekNext();
};

#endif // !PARSER_H