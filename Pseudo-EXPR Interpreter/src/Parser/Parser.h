#ifndef PARSER_H

#define PARSER_H

#include <vector>

#include "../Token/Token.h"
#include "Expressions/Expression.h"
#include "Instructions/Instruction.h"
#include "../Interpreter/Environmnet.h"

/// Handles the parsing of the tokens
///
/// This class handles all the parsing of
/// the tokens while generating both expressions
/// and instructions. The expressions are
/// parsed using a recursive descent algorithm
/// while the instructions are parsed using a
/// basic switch which checks the first token on
/// every row.
/// @attention This class does not store
/// the instructions in itself. Which means
/// that if parse() throws an exception then
/// all of the genrated instructions are lost.
/// However if you call parse() again, it will skip
/// the invalid instruction and continue parsing the
/// other tokens.
class Parser
{
public:
	/// Parser Constructor
	///
	/// Takes all of the genreated tokens
	/// and a global Environment which will
	/// get filled with the global identifiers
	/// @param tokens A collection of the generated
	/// tokens from Tokenizer
	/// @param environment An Environment pointer
	/// which points to a global Environment
	Parser(const std::vector<Token>& tokens, Environment* environmnet);

	/// Parses all of the tokens
	///
	/// Starts parsing all of the tokens one by one.
	/// For expression it uses reucrsive descent
	/// while for instruction uses a simple switch
	/// condition.
	/// @returns A collection of instructions
	/// @throws SyntaxError if there is an
	/// invalid language syntax while parsing.
	/// @note Before throwing an exception, it
	/// skips the faulty token and then throws
	std::vector<Instruction*> parse();

	/// Checkes wether parse()
	/// parsed all of the tokens or not
	/// @returns If all of the tokens are parsed
	bool isAtEnd() const;

private:
	/// A collection of the tokens which need
	/// to be parsed
	std::vector<Token> m_Tokens;
	/// An iterator to the current token
	std::vector<Token>::iterator m_CurrToken;
	/// A pointer that points to the global Environmnet
	Environment* m_Environment;

	// -------- Instruction Handling -------- //

	/// Handles the parsing of an instruction
	/// @returns An Instruction
	/// @throws SyntaxError if there is 
	/// invalid language Syntax
	Instruction* instruction();

	/// Handles the parsing of assigning a
	/// an expression to a variable
	/// @returns Assignment Instruction
	/// @throws SyntaxError if there is 
	/// invalid language Syntax
	Instruction* variable();

	/// Handles the parsing of a delaration
	/// of a function
	/// @returns Declaration Instruction
	/// @throws SyntaxError if there is 
	/// invalid language Syntax
	Instruction* function();

	/// Handles the parsing of an if-else condition
	/// @returns Condition Instruction
	/// @throws SyntaxError if there is 
	/// invalid language Syntax
	Instruction* condition();

	/// Handles the parsing of a while loop
	/// @returns Loop Instruction
	/// @throws SyntaxError if there is 
	/// invalid language Syntax
	Instruction* loop();

	/// Handles the parsing of a print instruction
	/// @returns Print Instruction
	/// @throws SyntaxError if there is 
	/// invalid language Syntax
	Instruction* print();

	/// Handles the parsing of a read instruction
	/// @returns Read Instruction
	/// @throws SyntaxError if there is 
	/// invalid language Syntax
	Instruction* read();

	/// Skips a line
	/// 
	/// Used when an exception is thrown
	/// and the Interpreter is confused
	/// about it's location in the code
	void eol();

	// -------- Expression Handling -------- //
	
	/// Handles the parsing of an expression
	/// @returns An Expression
	/// @throws SyntaxError if there is
	/// invalid language syntax
	Expression* expression();

	/// Handles the parsing of an if-else expression
	/// @returns A Ternary Expression
	/// @throws SyntaxError if there is
	/// invalid language syntax
	Expression* ifElseExpr();

	/// Handles the parsing of a ternary expression
	/// @returns A Ternary Expression
	/// @throws SyntaxError if there is
	/// invalid language syntax
	Expression* ternary();

	/// Handles the parsing of a logical or expression
	/// @returns A Binary Expression
	/// @throws SyntaxError if there is
	/// invalid language syntax
	Expression* logicalOr();

	/// Handles the parsing of a logical and expression
	/// @returns A Binary Expression
	/// @throws SyntaxError if there is
	/// invalid language syntax
	Expression* logicalAnd();

	/// Handles the parsing of an equality expression
	/// @returns A Binary Expression
	/// @throws SyntaxError if there is
	/// invalid language syntax
	Expression* equality();

	/// Handles the parsing of a comparison expression
	/// @returns A Binary Expression
	/// @throws SyntaxError if there is
	/// invalid language syntax
	Expression* comparison();

	/// Handles the parsing of an arithmetic expression
	/// @returns A Binary Expression
	/// @throws SyntaxError if there is
	/// invalid language syntax
	Expression* arithmetic();

	/// Handles the parsing of a factor expression
	/// @returns A Binary Expression
	/// @throws SyntaxError if there is
	/// invalid language syntax
	Expression* factor();

	/// Handles the parsing of a unary expression
	/// @returns A Unary Expression
	/// @throws SyntaxError if there is
	/// invalid language syntax
	Expression* unary();

	/// Handles the parsing of a primitive expression
	/// @returns Either a Primitive, Variable or 
	/// FunctionCaller
	/// @throws SyntaxError if there is
	/// invalid language syntax
	Expression* primitive();

	/// Skips to the next token
	void next();

	/// Gets the current token
	/// and calls next()
	/// @returns The current token
	const Token& consume();

	/// Gets the current token's type
	/// and calls next()
	/// @returns The current token's type
	TokenType consumeType();

	/// Skips a line in the code
	void skipLine();

	/// Gets the current token
	/// @returns The current token
	const Token& peek() const;

	/// Gets the current token's type
	/// @returns The current token's type
	TokenType peekType() const;

	/// Gets the current line in the code
	/// @returns The current line in the code
	uint64_t peekLine() const;

	/// Gets the previous line in the code
	/// @returns The previous line in the code
	uint64_t prevLine() const;

	/// Checks if there are more instructions
	/// on the current row
	/// @returns If there are more instructions
	/// on the current row
	bool hasMoreInstructions() const;
};

#endif // !PARSER_H