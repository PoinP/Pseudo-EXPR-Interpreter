#include "../doctest.h"

#include "../../src/Parser/Parser.h"
#include "../../src/Token/Tokenizer.h"
#include "../../src/Errors/SyntaxError.h"

TEST_CASE("Declaration")
{
	std::string t1 = "print 10";

	Tokenizer lexer(t1);
	std::vector<Token> tokens = lexer.tokenize();
	Environment env;
	Parser parser(tokens, &env);
	CHECK_NOTHROW(parser.parse());
}

TEST_CASE("Invalid Expression")
{
	std::string t1 = "print print 2";

	Tokenizer lexer(t1);
	std::vector<Token> tokens = lexer.tokenize();
	Environment env;
	Parser parser(tokens, &env);

	CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected an expression } at line 1", SyntaxError);
}

TEST_CASE("Has More Than One Instruction On Row")
{
	std::string t1 = "print 5 print 5\n";

	Tokenizer lexer(t1);
	std::vector<Token> tokens = lexer.tokenize();
	Environment env;
	Parser parser(tokens, &env);

	CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected one instruction per row only } at line 1", SyntaxError);
}