#include "doctest.h"

#include "../src/Parser/Parser.h"
#include "../src/Token/Tokenizer.h"
#include <string>

#include "../src/Errors/SyntaxError.h"
#include "../src/Errors/RuntimeError.h"


TEST_CASE("Constructor")
{
	std::vector<Token> tokens;
	Environment env;

	Parser parser(tokens, &env);
	
	CHECK(parser.parse().empty());
	CHECK(parser.isAtEnd());
}

TEST_CASE("Base Parsing")
{
	SUBCASE("Without Any Instructions")
	{
		std::string src = "";
		Tokenizer lexer(src);

		std::vector<Token> tokens = lexer.tokenize();
		Environment env;

		Parser parser(tokens, &env);
		std::vector<Instruction*> instructions = parser.parse();

		CHECK(instructions.size() == 0);
	}

	SUBCASE("With a Syntax Error")
	{
		std::string src = "12 + 3";
		Tokenizer lexer(src);

		std::vector<Token> tokens = lexer.tokenize();
		Environment env;

		Parser parser(tokens, &env);
		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected a declaration } at line 1", SyntaxError);
	}

	SUBCASE("With Instructions")
	{
		std::string src = "x = 2";
		Tokenizer lexer(src);

		std::vector<Token> tokens = lexer.tokenize();
		Environment env;

		Parser parser(tokens, &env);
		std::vector<Instruction*> instructions = parser.parse();

		CHECK(instructions.size() == 1);

		instructions[0]->run();

		CHECK(env.get("x")->evaluate(&env) == 2);
	}

	SUBCASE("With a Run-Time Error")
	{
		std::string src = "x = 192 / 0";
		Tokenizer lexer(src);

		std::vector<Token> tokens = lexer.tokenize();
		Environment env;

		Parser parser(tokens, &env);
		std::vector<Instruction*> instructions = parser.parse();

		for (Instruction* i : instructions)
			CHECK_THROWS_WITH_AS(i->run(), "Run-Time Error: { Division by zero } at line 1", RunTimeError);
	}
}