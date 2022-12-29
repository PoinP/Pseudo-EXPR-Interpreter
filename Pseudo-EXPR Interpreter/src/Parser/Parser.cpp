#include "Parser.h"

#include "Expressions/Binary.h"
#include "Expressions/Grouping.h"
#include "Expressions/Primitive.h"
#include "Expressions/Unary.h"

#include "../Exceptions/SyntaxError.h"

Parser::Parser(const std::list<Token>& tokens)
	: m_Tokens(tokens), m_CurrToken(m_Tokens.begin())
{
}

Expression* Parser::parse()
{
	return expression();
}

unsigned long long Parser::evaluate(Expression* expressions)
{
	return expressions->evaluate();
}

Expression* Parser::expression()
{
	return equality();
}

Expression* Parser::equality()
{
	Expression* left = comparison();

	TokenType type = peek().getType();

	while (type == TokenType::EQUAL_EQUAL || type == TokenType::NOT_EQUAL)
	{
		Token op = consume();
		Expression* right = comparison();
		left = new Binary(left, op, right);
	}

	return left;
}

Expression* Parser::comparison()
{
	Expression* left = arithmetic();

	TokenType type = peek().getType();

	while (type == TokenType::LESS_THAN || type == TokenType::LESS_EQUAL ||
		type == TokenType::GREATER_THAN || type == TokenType::GREATER_EQUAL)
	{
		Token op = consume();
		Expression* right = arithmetic();
		type = peek().getType();
		left = new Binary(left, op, right);
	}

	return left;
}

Expression* Parser::arithmetic()
{
	Expression* left = factor();

	TokenType type = peek().getType();

	while (type == TokenType::PLUS || type == TokenType::MINUS)
	{
		Token op = consume();
		Expression* right = factor();
		type = peek().getType();
		left = new Binary(left, op, right);
	}

	return left;
}

Expression* Parser::factor()
{
	Expression* left = unary();

	TokenType type = peek().getType();

	while (type == TokenType::PROD || type == TokenType::DIV)
	{
		Token op = consume();
		Expression* right = unary();
		type = peek().getType();
		left = new Binary(left, op, right);
	}

	return left;
}

Expression* Parser::unary()
{
	TokenType type = peek().getType();

	while (type == TokenType::MINUS || type == TokenType::NOT)
	{
		Token op = consume();
		Expression* expr = unary();
		type = peek().getType();
		return new Unary(op, expr);
	}

	return primitive();
}

Expression* Parser::primitive()
{
	if (peek().getType() == TokenType::NUMBER)
		return new Primitive(consume());

	if (peek().getType() == TokenType::OPEN_PAREN)
	{
		next();
		Expression* expr = expression();
		if (peek().getType() == TokenType::CLOSE_PAREN)
		{
			next();
			return new Grouping(expr);
		}

		log(SyntaxError("Expected a ')'", peek().getLine()).what());
		return nullptr;
	}

	// Add variables;

	log(SyntaxError("Expected an expression", peek().getLine()).what());
	return nullptr;
}

void Parser::next()
{
	m_CurrToken++;
}

const Token& Parser::consume()
{
	const Token& token = *m_CurrToken;
	m_CurrToken++;
	return token;
}

const Token& Parser::peek() const
{
	return *m_CurrToken;
}

TokenType Parser::peekType() const
{
	return peek().getType();
}

void Parser::log(const char* msg) const
{
	std::cout << msg << std::endl;
}
