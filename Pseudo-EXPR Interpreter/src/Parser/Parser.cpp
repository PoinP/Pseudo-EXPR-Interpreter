#include "Parser.h"

#include "Expressions/Binary.h"
#include "Expressions/Grouping.h"
#include "Expressions/Primitive.h"
#include "Expressions/Unary.h"
#include "Expressions/Variable.h"

#include "../Exceptions/SyntaxError.h"
#include "Instructions/Instruction.h"
#include "Instructions/Print.h"
#include "Instructions/Assignment.h"

Parser::Parser(const std::list<Token>& tokens)
	: m_Tokens(tokens), m_CurrToken(m_Tokens.begin())
{
}

std::list<Instruction*> Parser::parse()
{
	std::list<Instruction*> instructions;

	while (peekType() != TokenType::END_OF_FILE)
	{
		if (peekType() == TokenType::END_OF_LINE)
		{
			next();
			continue;
		}

		if (peekType() == TokenType::IDENTIFIER)
		{
			std::string varName = consume().getIdentifier();
			if (peekType() == TokenType::EQUALS)
			{
				next();
				instructions.push_back(new Assignment(varName, expression(), &m_Environment));
				continue;
			}
			continue;
		}

		if (peekType() == TokenType::PRINT)
		{
			next();
			instructions.push_back(new Print(expression()));
			continue;
		}

		throw SyntaxError("Expected a declaration", peek().getLine());
	}

	return instructions;
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

	TokenType type = peekType();

	while (type == TokenType::EQUAL_EQUAL || type == TokenType::NOT_EQUAL)
	{
		Token op = consume();
		Expression* right = comparison();
		type = peekType();
		left = new Binary(left, op, right);
	}

	return left;
}

Expression* Parser::comparison()
{
	Expression* left = arithmetic();

	TokenType type = peekType();

	while (type == TokenType::LESS_THAN || type == TokenType::LESS_EQUAL ||
		type == TokenType::GREATER_THAN || type == TokenType::GREATER_EQUAL)
	{
		Token op = consume();
		Expression* right = arithmetic();
		type = peekType();
		left = new Binary(left, op, right);
	}

	return left;
}

Expression* Parser::arithmetic()
{
	Expression* left = factor();

	TokenType type = peekType();

	while (type == TokenType::PLUS || type == TokenType::MINUS)
	{
		Token op = consume();
		Expression* right = factor();
		type = peekType();
		left = new Binary(left, op, right);
	}

	return left;
}

Expression* Parser::factor()
{
	Expression* left = unary();

	TokenType type = peekType();

	while (type == TokenType::PROD || type == TokenType::DIV)
	{
		Token op = consume();
		Expression* right = unary();
		type = peekType();
		left = new Binary(left, op, right);
	}

	return left;
}

Expression* Parser::unary()
{
	TokenType type = peekType();

	while (type == TokenType::MINUS || type == TokenType::NOT)
	{
		Token op = consume();
		Expression* expr = unary();
		type = peekType();
		return new Unary(op, expr);
	}

	return primitive();
}

Expression* Parser::primitive()
{
	if (peekType() == TokenType::NUMBER)
		return new Primitive(consume());

	if (peekType() == TokenType::IDENTIFIER)
		return new Variable(consume(), &m_Environment);

	if (peekType() == TokenType::OPEN_PAREN)
	{
		next();
		Expression* expr = expression();
		if (peekType() == TokenType::CLOSE_PAREN)
		{
			next();
			return new Grouping(expr);
		}

		log(SyntaxError("Expected a ')'", peek().getLine()).what());
		return nullptr;
	}

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
