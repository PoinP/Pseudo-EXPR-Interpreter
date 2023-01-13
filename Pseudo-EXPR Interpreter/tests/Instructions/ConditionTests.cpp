#include "../doctest.h"

#include "../../src/Parser/Parser.h"
#include "../../src/Token/Tokenizer.h"
#include "../../src/Errors/SyntaxError.h"

TEST_CASE("Declaration")
{
	SUBCASE("When True")
	{
		std::string t1 = "if 60 > 30 then x = 1 else x = 0\n";

		Tokenizer lexer(t1);
		std::vector<Token> tokens = lexer.tokenize();
		Environment env;
		Parser parser(tokens, &env);
		std::vector<Instruction*> instrs = parser.parse();

		instrs[0]->run();
		CHECK(env.get("x")->evaluate(&env) == 1);

		for (Instruction* i : instrs)
			delete i;
	}

	SUBCASE("When False")
	{
		std::string t1 = "if 60 < 30 then x = 1 else x = 0\n";

		Tokenizer lexer(t1);
		std::vector<Token> tokens = lexer.tokenize();
		Environment env;
		Parser parser(tokens, &env);
		std::vector<Instruction*> instrs = parser.parse();

		instrs[0]->run();
		CHECK(env.get("x")->evaluate(&env) == 0);

		for (Instruction* i : instrs)
			delete i;
	}
}

TEST_CASE("Invalid Condition")
{
	std::string t1 = "if print 2 then x = 1 else x = 0\n";

	Tokenizer lexer(t1);
	std::vector<Token> tokens = lexer.tokenize();
	Environment env;
	Parser parser(tokens, &env);

	CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected an expression } at line 1", SyntaxError);
}

TEST_CASE("Invalid Condition Without 'then'")
{
	std::string t1 = "if print 2\n";

	Tokenizer lexer(t1);
	std::vector<Token> tokens = lexer.tokenize();
	Environment env;
	Parser parser(tokens, &env);

	CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected an expression } at line 1", SyntaxError);
}

TEST_CASE("Missing 'then'")
{
	std::string t1 = "if 60 > 30 x = 1 else x = 0\n";

	Tokenizer lexer(t1);
	std::vector<Token> tokens = lexer.tokenize();
	Environment env;
	Parser parser(tokens, &env);

	CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected \"then\" after condition } at line 1", SyntaxError);
}

TEST_CASE("Missing Instruction If True")
{
	std::string t1 = "if 60 > 30 then\n";

	Tokenizer lexer(t1);
	std::vector<Token> tokens = lexer.tokenize();
	Environment env;
	Parser parser(tokens, &env);

	CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected a declaration } at line 1", SyntaxError);
}

TEST_CASE("Missing 'else'")
{
	std::string t1 = "if 60 > 30 then x = 1\n";

	Tokenizer lexer(t1);
	std::vector<Token> tokens = lexer.tokenize();
	Environment env;
	Parser parser(tokens, &env);

	CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected \"else\" after declaration } at line 1", SyntaxError);
}

TEST_CASE("Missing Instruction If False")
{
	std::string t1 = "if 60 > 30 then x = 1 else\n";

	Tokenizer lexer(t1);
	std::vector<Token> tokens = lexer.tokenize();
	Environment env;
	Parser parser(tokens, &env);

	CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected a declaration } at line 1", SyntaxError);
}

TEST_CASE("Expressions Instead Of Declarations")
{
	SUBCASE("If True")
	{
		std::string t1 = "if 60 > 30 then 12 + 5 else x = 0\n";

		Tokenizer lexer(t1);
		std::vector<Token> tokens = lexer.tokenize();
		Environment env;
		Parser parser(tokens, &env);

		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected a declaration } at line 1", SyntaxError);
	}

	SUBCASE("If False")
	{
		std::string t1 = "if 60 > 30 then x = 1 else (5 + 5)\n";

		Tokenizer lexer(t1);
		std::vector<Token> tokens = lexer.tokenize();
		Environment env;
		Parser parser(tokens, &env);

		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected a declaration } at line 1", SyntaxError);
	}
}

TEST_CASE("Using 'else' Without 'if'")
{
	std::string t1 = "else (5 + 5)\n";

	Tokenizer lexer(t1);
	std::vector<Token> tokens = lexer.tokenize();
	Environment env;
	Parser parser(tokens, &env);

	CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { \"else\" can not be used without an \"if\" expression } at line 1", SyntaxError);
}

TEST_CASE("Using 'then' Without 'if'")
{
	std::string t1 = "then (5 + 5)\n";

	Tokenizer lexer(t1);
	std::vector<Token> tokens = lexer.tokenize();
	Environment env;
	Parser parser(tokens, &env);

	CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { \"then\" must be used after an \"if\" condition } at line 1", SyntaxError);
}

TEST_CASE("More Than One Instruction On Row")
{
	std::string t1 = "if 60 < 30 then x = 1 else x = (5 + 5) print 2\n";
	Tokenizer lexer(t1);
	std::vector<Token> tokens = lexer.tokenize();
	Environment env;
	Parser parser(tokens, &env);

	CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected one instruction per row only } at line 1", SyntaxError);

	t1 = "if 60 < 30 then x = 1 else x = (5 + 5) 27 + 99\n";
	lexer = Tokenizer(t1);
	tokens = lexer.tokenize();
	parser = Parser(tokens, &env);

	CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected one instruction per row only } at line 1", SyntaxError);
}