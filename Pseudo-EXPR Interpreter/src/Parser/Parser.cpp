#include "Parser.h"

#include "Expressions/Binary.h"
#include "Expressions/Grouping.h"
#include "Expressions/Primitive.h"
#include "Expressions/Unary.h"
#include "Expressions/Variable.h"
#include "Expressions/FunctionCaller.h"

#include "../Exceptions/SyntaxError.h"
#include "Instructions/Instruction.h"
#include "Instructions/Print.h"
#include "Instructions/Assignment.h"
#include "Instructions/Read.h"
#include "Instructions/Declaration.h"

Parser::Parser(const std::list<Token>& tokens)
	: m_Tokens(tokens), m_CurrToken(m_Tokens.begin())
{
}

std::list<Instruction*> Parser::parse()
{
	std::list<Instruction*> instructions;

	while (peekType() != TokenType::END_OF_FILE)
	{
		switch (peekType())
		{
		case TokenType::VARIABLE: instructions.push_back(variable()); break;
		case TokenType::FUNCTION: instructions.push_back(function()); break;
		case TokenType::PRINT:    instructions.push_back(print());    break;
		case TokenType::READ:     instructions.push_back(read());     break;

		case TokenType::END_OF_LINE:		             eol();	      break;

		default:	throw SyntaxError("Expected a declaration", peek().getLine());
		}

	}

	return instructions;
}

Instruction* Parser::variable()
{
	Instruction* assignmentInstr = nullptr;
	std::string varName = consume().getIdentifier();

	if (peekType() == TokenType::EQUALS)
	{
		next();
		assignmentInstr = new Assignment(varName, expression(), &m_Environment);
	}

	if (peekType() != TokenType::END_OF_LINE)
		throw SyntaxError("Expected one statement per row only!", peek().getLine());

	return assignmentInstr;
}

Instruction* Parser::function()
{
	Instruction* declInstr = nullptr;
	Token funcToken = consume();

	if (peekType() != TokenType::OPEN_BRACKET)
		throw SyntaxError("Expected a parameter after function declaration", peek().getLine());

	next();

	if (peekType() == TokenType::CLOSE_BRACKET)
		throw SyntaxError("0 parameters are not supported", peek().getLine());

	if (peekType() != TokenType::VARIABLE)
		throw SyntaxError("Fuction parameter expected", peek().getLine());

	Token paramToken = consume();

	if (peekType() != TokenType::CLOSE_BRACKET)
		throw SyntaxError("Expected a ']' with single parameter", peek().getLine());

	next();

	if (peekType() != TokenType::EQUALS)
		throw SyntaxError("Invalid function declaration", peek().getLine());

	next();

	declInstr = new Declaration(funcToken, paramToken, expression(), &m_Environment);

	return declInstr;
}

Instruction* Parser::print()
{
	Instruction* printInstr = nullptr;

	next();
	printInstr = new Print(expression(), &m_Environment);

	if (peekType() != TokenType::END_OF_LINE)
		throw SyntaxError("Expected one statement per row only!", peek().getLine());

	return printInstr;
}

Instruction* Parser::read()
{

	Instruction* readInstr = nullptr;
	next();

	if (peekType() != TokenType::VARIABLE)
		throw SyntaxError("Can not apply value to a non variable token!", peek().getLine());

	readInstr = new Read(&consume(), &m_Environment);

	if (peekType() != TokenType::END_OF_LINE)
		throw SyntaxError("Expected one statement per row only!", peek().getLine());

	return readInstr;
}

Instruction* Parser::eol()
{
	next();
	return nullptr;
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

	while (type == TokenType::PROD || type == TokenType::DIV || type == TokenType::MOD)
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

	if (peekType() == TokenType::VARIABLE)
		return new Variable(consume());

	if (peekType() == TokenType::FUNCTION)
	{
		Token name = consume();
		next();
		Expression* expr = expression();
		next();

		return new FunctionCaller(name, expr);
	}

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