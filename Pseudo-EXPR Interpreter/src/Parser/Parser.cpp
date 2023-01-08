#include "Parser.h"

#include "Expressions/Unary.h"
#include "Expressions/Binary.h"
#include "Expressions/Ternary.h"
#include "Expressions/Variable.h"
#include "Expressions/Grouping.h"
#include "Expressions/Primitive.h"
#include "Expressions/FunctionCaller.h"

#include "Instructions/Loop.h"
#include "Instructions/Read.h"
#include "Instructions/Print.h"
#include "Instructions/Condition.h"
#include "Instructions/Assignment.h"
#include "Instructions/Declaration.h"
#include "Instructions/Instruction.h"

#include "../Errors/SyntaxError.h"
#include "../Errors/RunTimeError.h"

Parser::Parser(const std::vector<Token>& tokens, Environment* environmnet)
	: m_Tokens(tokens), m_CurrToken(m_Tokens.begin()), m_Environment(environmnet)
{
}

std::vector<Instruction*> Parser::parse()
{
	std::vector<Instruction*> instructions;

	if (m_Tokens.empty())
		return instructions;

	instructions.reserve(m_Tokens.back().getLine());

	while (peekType() != TokenType::END_OF_FILE)
	{
		try 
		{
			Instruction* instr = instruction();

			if (instr)
				instructions.push_back(instr);
		}
		catch (const SyntaxError&)
		{
			for (Instruction* instruction : instructions)
				delete instruction;

			eol();
			throw;
		}
	}

	return instructions;
}

bool Parser::isAtEnd() const
{
	if (m_Tokens.empty())
		return true;

	return peekType() == TokenType::END_OF_FILE;
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
		
	case TokenType::ELSE:		 throw SyntaxError("\"else\" can not be used without an \"if\" expression", peekLine());
	case TokenType::THEN:		 throw SyntaxError("\"then\" must be used after an \"if\" condition", peekLine());
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
		assignmentInstr = new Assignment(varName, expression(), m_Environment);

		if (hasMoreInstructions())
		{
			delete assignmentInstr;
			throw SyntaxError("Expected one instruction per row only", peekLine());
		}

		return assignmentInstr;
	}
			
	throw SyntaxError("Expected variable initalization", peekLine());
}

Instruction* Parser::function()
{
	Instruction* declInstr = nullptr;
	Token funcToken = consume();

	if (consumeType() != TokenType::OPEN_BRACKET)
		throw SyntaxError("Expected a parameter after function declaration", prevLine());

	if (peekType() == TokenType::CLOSE_BRACKET)
		throw SyntaxError("0 parameters are not supported", peekLine());

	if (peekType() != TokenType::VARIABLE)
		throw SyntaxError("Fuction parameter expected", peekLine());

	Token paramToken = consume();

	if (consumeType() != TokenType::CLOSE_BRACKET)
		throw SyntaxError("Expected a ']' with single parameter", prevLine());

	if (consumeType() != TokenType::EQUALS)
		throw SyntaxError("Invalid function declaration", prevLine());

	declInstr = new Declaration(funcToken, paramToken, expression(), m_Environment);

	if (hasMoreInstructions())
	{
		delete declInstr;
		throw SyntaxError("Expected one instruction per row only", peekLine());
	}

	return declInstr;
}

Instruction* Parser::condition()
{
	Instruction* conditionInstr = nullptr;
	consume();

	Expression* condition = logicalOr();

	if (consumeType() != TokenType::THEN)
	{
		delete condition;
		throw SyntaxError("Expected \"then\" after condition", prevLine());
	}

	Instruction* ifTrue = nullptr;
	try { ifTrue = instruction(); }
	catch (...) { delete condition; throw; }

	if (!ifTrue)
	{
		delete condition;
		throw SyntaxError("Expected a declaration", prevLine());
	}

	if (consumeType() != TokenType::ELSE)
	{
		delete condition;
		delete ifTrue;
		throw SyntaxError("Expected \"else\" after declaration", prevLine());
	}

	Instruction* ifFalse = nullptr;
	try { ifTrue = instruction(); }
	catch (...) { delete condition; delete ifTrue; throw; }

	if (!ifFalse)
	{
		delete condition;
		delete ifTrue;
		throw SyntaxError("Expected a declaration", prevLine());
	}

	conditionInstr = new Condition(condition, ifTrue, ifFalse, m_Environment);

	if (hasMoreInstructions())
	{
		delete conditionInstr;
		throw SyntaxError("Expected one instruction per row only", peekLine());
	}

	return conditionInstr;
}

Instruction* Parser::loop()
{
	Instruction* loopInstr = nullptr;
	consume();

	Expression* condition = logicalOr();

	if (consumeType() != TokenType::DO)
	{
		delete condition;
		throw SyntaxError("Expected \"do\" after condition", prevLine());
	}

	std::vector<Instruction*> instructions;
	while (peekType() != TokenType::DONE)
	{
		if (peekType() == TokenType::END_OF_LINE)
		{
			eol();
			continue;
		}

		if (peekType() == TokenType::END_OF_FILE)
		{
			delete condition;
			for (Instruction* instruction : instructions)
				delete instruction;

			throw SyntaxError("Expected \"done\"", peekLine());
		}

		try { instructions.push_back(instruction()); }
		catch (...)
		{
			delete condition;
			for (Instruction* instruction : instructions)
				delete instruction;

			throw;
		}
	}

	consume();

	loopInstr = new Loop(condition, instructions, m_Environment);

	if (hasMoreInstructions())
	{
		delete loopInstr;
		throw SyntaxError("Expected one instruction per row only", peekLine());
	}

	return loopInstr;
}

Instruction* Parser::print()
{
	Instruction* printInstr = nullptr;

	next();

	printInstr = new Print(expression(), m_Environment);

	if (hasMoreInstructions())
	{
		delete printInstr;
		throw SyntaxError("Expected one instruction per row only", peekLine());
	}

	return printInstr;
}

Instruction* Parser::read()
{
	Instruction* readInstr = nullptr;

	next();

	if (peekType() != TokenType::VARIABLE)
		throw SyntaxError("Can not apply value to a non variable token", peekLine());

	readInstr = new Read(consume(), m_Environment);

	if (hasMoreInstructions())
	{
		delete readInstr;
		throw SyntaxError("Expected one instruction per row only", peekLine());
	}

	return readInstr;
}

void Parser::eol()
{
	if (peekType() == TokenType::END_OF_FILE)
		return;

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
	{
		delete left;
		throw SyntaxError("Expected \"then\" after condition", prevLine());
	}

	Expression* ifTrue = nullptr;
	try { ifTrue = expression(); }
	catch (...) { delete left; throw; }

	if (consumeType() != TokenType::ELSE)
	{
		delete left;
		delete ifTrue;
		throw SyntaxError("Expected \"else\"", prevLine());
	}

	Expression* ifFalse = nullptr;
	try { ifFalse = expression(); }
	catch (...) { delete left; delete ifTrue; throw; }

	left = new Ternary(left, ifTrue, ifFalse);

	return left;
}

Expression* Parser::ternary()
{
	Expression* left = logicalOr();

	if (peekType() == TokenType::QUESTION)
	{
		next();
		
		Expression* ifTrue = nullptr;
		try { ifTrue = ternary(); }
		catch (...) { delete left; throw; }

		if (consumeType() != TokenType::COLON)
		{
			delete left;
			delete ifTrue;
			throw SyntaxError("Expected ':'", prevLine());
		}

		Expression* ifFalse = nullptr;
		try { ifFalse = ternary(); }
		catch (...) { delete left; delete ifTrue; throw; }

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
		Expression* right = nullptr;
		
		try { right = logicalAnd(); }
		catch (...) { delete left; throw; }

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
		Expression* right = nullptr;
		try { right = logicalAnd(); }
		catch (...) { delete left; throw; }
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
		Expression* right = nullptr;
		try { right = comparison(); }
		catch (...) { delete left; throw; }
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
		Expression* right = nullptr;
		try { right = arithmetic(); }
		catch (...) { delete left; throw; }
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
		Expression* right = nullptr;
		try { right = factor(); }
		catch (...) { delete left; throw; }
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
		Expression* right = nullptr;
		try { right = unary(); }
		catch (...) { delete left; throw; }
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
			throw SyntaxError("Expected a function call", prevLine());

		Expression* expr = expression();

		if (consumeType() != TokenType::CLOSE_BRACKET)
			throw SyntaxError("Expected a ']'", prevLine());

		return new FunctionCaller(name, expr);
	}

	if (peekType() == TokenType::OPEN_PAREN)
	{
		next();
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

uint64_t Parser::prevLine() const
{
	return (*(m_CurrToken - 1)).getLine();
}

bool Parser::hasMoreInstructions() const
{
	return peekType() != TokenType::END_OF_LINE && 
		   peekType() != TokenType::ELSE && 
		   peekType() != TokenType::DONE;
}
