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

TEST_CASE("If Expression Parsing")
{
	SUBCASE("Correct parsing")
	{
		Tokenizer lexer("x = if 1 > 0 then 1 else 0");
		Environment env;
		Parser parser(lexer.tokenize(), &env);
		CHECK_NOTHROW(parser.parse());
	}

	SUBCASE("Invalid Condition")
	{
		Tokenizer lexer("x = if print 2 then 1 else 0");
		Environment env;
		Parser parser(lexer.tokenize(), &env);
		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected an expression } at line 1", SyntaxError);
	}

	SUBCASE("Invalid If True Expression")
	{
		Tokenizer lexer("x = if 1 > 0 then print 1 else 0");
		Environment env;
		Parser parser(lexer.tokenize(), &env);
		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected an expression } at line 1", SyntaxError);
	}

	SUBCASE("Invalid If False Expression")
	{
		Tokenizer lexer("x = if 1 > 0 then 1 else print 3");
		Environment env;
		Parser parser(lexer.tokenize(), &env);
		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected an expression } at line 1", SyntaxError);
	}

	SUBCASE("Missing 'then'")
	{
		Tokenizer lexer("x = if 1 > 0 1 else 0");
		Environment env;
		Parser parser(lexer.tokenize(), &env);
		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected \"then\" after condition } at line 1", SyntaxError);
	}

	SUBCASE("Missing 'else'")
	{
		Tokenizer lexer("x = if 1 > 0 then 1 0");
		Environment env;
		Parser parser(lexer.tokenize(), &env);
		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected \"else\" } at line 1", SyntaxError);
	}

	SUBCASE("Chaining expressions")
	{
		Tokenizer lexer("x = if 1 > 0 then if 1 < 0 then 2 else 1 else 0");
		Environment env;
		Parser parser(lexer.tokenize(), &env);
		CHECK_NOTHROW(parser.parse()[0]->run());
		CHECK(env.get("x")->evaluate(&env) == 1);
	}
}

TEST_CASE("Invalid Primitive Parsing")
{
	SUBCASE("Functions")
	{
		Tokenizer lexer("x = FUNC");
		Environment env;
		Parser parser(lexer.tokenize(), &env);
		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected a function call } at line 1", SyntaxError);

		lexer = Tokenizer("x = FUNC[");
		parser = Parser(lexer.tokenize(), &env);
		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected an expression } at line 1", SyntaxError);

		lexer = Tokenizer("x = FUNC[5");
		parser = Parser(lexer.tokenize(), &env);
		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected a ']' } at line 1", SyntaxError);

		lexer = Tokenizer("x = FUNC[5]");
		parser = Parser(lexer.tokenize(), &env);
		CHECK_NOTHROW(parser.parse());
	}

	SUBCASE("Grouping")
	{
		Tokenizer lexer("x = (1 + 5");
		Environment env;
		Parser parser(lexer.tokenize(), &env);
		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected a ')' } at line 1", SyntaxError);

		lexer = Tokenizer("x = (1 + 5)");
		parser = Parser(lexer.tokenize(), &env);
		CHECK_NOTHROW(parser.parse());
	}
}