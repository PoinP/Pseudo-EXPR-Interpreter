#include "Parser.h"

#include "Expressions/Binary.h"
#include "Expressions/Grouping.h"
#include "Expressions/Primitive.h"
#include "Expressions/Unary.h"
#include "Expressions/Variable.h"
#include "Expressions/FunctionCaller.h"
#include "Expressions/Ternary.h"

#include "../Exceptions/SyntaxError.h"
#include "Instructions/Instruction.h"
#include "Instructions/Print.h"
#include "Instructions/Assignment.h"
#include "Instructions/Read.h"
#include "Instructions/Declaration.h"
#include "Instructions/Condition.h"
#include "Instructions/Loop.h"

Parser::Parser(const std::list<Token>& tokens)
	: m_Tokens(tokens), m_CurrToken(m_Tokens.begin())
{
}

std::list<Instruction*> Parser::parse()
{
	std::list<Instruction*> instructions;

	while (peekType() != TokenType::END_OF_FILE)
	{
		try 
		{
			Instruction* instr = instruction();

			if (instr)
				instructions.push_back(instr);
		}
		catch (const SyntaxError& error)
		{
			std::cout << error.what() << std::endl;
			skipLine();
		}
		catch (...)
		{
			throw;
		}
	}

	return instructions;
}

Instruction* Parser::instruction()
{
	switch (peekType())
	{
	case TokenType::VARIABLE:	 return variable();
	case TokenType::FUNCTION:	 return function();
	case TokenType::IF:			 return condition();
	case TokenType::WHILE:		 return loop();
	case TokenType::PRINT:		 return print();
	case TokenType::READ:		 return read();

	case TokenType::END_OF_LINE: eol(); break;
		
	case TokenType::ELSE:		 throw SyntaxError("\"else\" can not be used without and if expression", peekLine());
	case TokenType::THEN:		 throw SyntaxError("\"then\" must be used after an if condition", peekLine());
	case TokenType::DO:		     throw SyntaxError("\"do\" must be used after a loop condition", peekLine());
	case TokenType::DONE:        throw SyntaxError("\"done\" can not be used witout a while loop", peekLine());
	default:			         throw SyntaxError("Expected a declaration", peekLine());
	}

	return nullptr;
}

Instruction* Parser::variable()
{
	Instruction* assignmentInstr = nullptr;
	std::string varName = consume().getIdentifier();

	if (peekType() == TokenType::EQUALS)
	{
		next();
		assignmentInstr = new Assignment(varName, expression(), &m_Environment);

		if (peekType() != TokenType::END_OF_LINE && peekType() != TokenType::ELSE)
			throw SyntaxError("Expected one statement per row only!", peekLine());

		return assignmentInstr;
	}
			
	throw SyntaxError("Expected variable initalization!", peekLine());
}

Instruction* Parser::function()
{
	Instruction* declInstr = nullptr;
	Token funcToken = consume();

	if (consumeType() != TokenType::OPEN_BRACKET)
		throw SyntaxError("Expected a parameter after function declaration", peekLine());

	if (peekType() == TokenType::CLOSE_BRACKET)
		throw SyntaxError("0 parameters are not supported", peekLine());

	if (peekType() != TokenType::VARIABLE)
		throw SyntaxError("Fuction parameter expected", peekLine());

	Token paramToken = consume();

	if (consumeType() != TokenType::CLOSE_BRACKET)
		throw SyntaxError("Expected a ']' with single parameter", peekLine());

	if (consumeType() != TokenType::EQUALS)
		throw SyntaxError("Invalid function declaration", peekLine());

	declInstr = new Declaration(funcToken, paramToken, expression(), &m_Environment);

	return declInstr;
}

Instruction* Parser::condition()
{
	Instruction* conditionInstr = nullptr;
	consume();

	Expression* condition = logicalOr();

	if (consumeType() != TokenType::THEN)
		throw SyntaxError("Expected \"then\" after condition", peekLine());

	Instruction* ifTrue = instruction();

	if (consumeType() != TokenType::ELSE)
		throw SyntaxError("Expected \"else\"", peekLine());

	Instruction* ifFalse = instruction();

	return new Condition(condition, ifTrue, ifFalse, &m_Environment);
}

Instruction* Parser::loop()
{
	Instruction* loopInstr = nullptr;
	consume();

	Expression* condition = logicalOr();

	if (consumeType() != TokenType::DO)
		throw SyntaxError("Expected \"do\" after condition", peekLine());

	std::vector<Instruction*> instructions;
	while (peekType() != TokenType::DONE)
	{
		if (peekType() == TokenType::END_OF_LINE)
		{
			eol();
			continue;
		}

		if (peekType() == TokenType::END_OF_FILE)
			throw SyntaxError("Expected \"done\"", peekLine());

		instructions.push_back(instruction());
	}

	consume();

	loopInstr = new Loop(condition, instructions, &m_Environment);

	return loopInstr;
}

Instruction* Parser::print()
{
	Instruction* printInstr = nullptr;

	next();

	printInstr = new Print(expression(), &m_Environment);

	if (peekType() != TokenType::END_OF_LINE && peekType() != TokenType::ELSE)
		throw SyntaxError("Expected one statement per row only!", peekLine());

	return printInstr;
}

Instruction* Parser::read()
{
	Instruction* readInstr = nullptr;

	next();

	if (peekType() != TokenType::VARIABLE)
		throw SyntaxError("Can not apply value to a non variable token!", peekLine());

	readInstr = new Read(&consume(), &m_Environment);

	if (peekType() != TokenType::END_OF_LINE && peekType() != TokenType::ELSE)
		throw SyntaxError("Expected one statement per row only", peekLine());

	return readInstr;
}

void Parser::eol()
{
	skipLine();
}

Expression* Parser::expression()
{
	return ifElseExpr();
}

Expression* Parser::ifElseExpr()
{
	if (peekType() != TokenType::IF)
		return ternary();

	next();

	Expression* left = logicalOr();

	if (consumeType() != TokenType::THEN)
		throw SyntaxError("Expected \"then\" after condition", peekLine());

	Expression* ifTrue = expression();

	if (consumeType() != TokenType::ELSE)
		throw SyntaxError("Expected \"else\"", peekLine());

	Expression* ifFalse = expression();

	left = new Ternary(left, ifTrue, ifFalse);

	return left;
}

Expression* Parser::ternary()
{
	Expression* left = logicalOr();

	if (peekType() == TokenType::QUESTION)
	{
		next();
		
		Expression* ifTrue = ternary();

		if (consumeType() != TokenType::COLON)
			throw SyntaxError("Expected ':'", peekLine());

		Expression* ifFalse = ternary();

		left = new Ternary(left, ifTrue, ifFalse);
	}

	return left;
}

Expression* Parser::logicalOr()
{
	Expression* left = logicalAnd();

	TokenType type = peekType();

	while (type == TokenType::OR)
	{
		Token op = consume();
		Expression* right = logicalAnd();
		type = peekType();
		left = new Binary(left, op, right);
	}

	return left;
}
 
Expression* Parser::logicalAnd()
{
	Expression* left = equality();

	TokenType type = peekType();

	while (type == TokenType::AND)
	{
		Token op = consume();
		Expression* right = equality();
		type = peekType();
		left = new Binary(left, op, right);
	}

	return left;
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

		if (consumeType() != TokenType::OPEN_BRACKET)
			throw SyntaxError("Expected a function call", peekLine());

		Expression* expr = expression();

		if (consumeType() != TokenType::CLOSE_BRACKET)
			throw SyntaxError("Expected a ']'", peekLine());

		return new FunctionCaller(name, expr);
	}

	if (consumeType() == TokenType::OPEN_PAREN)
	{
		Expression* expr = expression();

		if (consumeType() == TokenType::CLOSE_PAREN)
			return new Grouping(expr);

		throw SyntaxError("Expected a ')'", peekLine());
	}

	throw SyntaxError("Expected an expression", peekLine());
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

TokenType Parser::consumeType()
{
	return consume().getType();
}

void Parser::skipLine()
{
	while (consume().getType() != TokenType::END_OF_LINE);
}

const Token& Parser::peek() const
{
	return *m_CurrToken;
}

TokenType Parser::peekType() const
{
	return peek().getType();
}

uint64_t Parser::peekLine() const
{
	return peek().getLine();
}

void Parser::log(const char* msg) const
{
	std::cout << msg << std::endl;
}