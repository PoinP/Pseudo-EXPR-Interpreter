#include "../doctest.h"

#include "../../src/Parser/Parser.h"
#include "../../src/Token/Tokenizer.h"
#include "../../src/Errors/SyntaxError.h"

TEST_CASE("Declaration")
{
	std::string t1 = "x = 0\n"
		"y = 0\n"
		"while x < 5 do\n"
		"x = x + 1\n"
		"y = y + 10\n"
		"done\n";

	Tokenizer lexer(t1);
	std::vector<Token> tokens = lexer.tokenize();
	Environment env;
	Parser parser(tokens, &env);
	std::vector<Instruction*> instrs = parser.parse();

	instrs[0]->run();
	instrs[1]->run();
	instrs[2]->run();

	CHECK(env.get("x")->evaluate(&env) == 5);
	CHECK(env.get("y")->evaluate(&env) == 50);

	for (Instruction* i : instrs)
		delete i;
}

TEST_CASE("Invalid Declarations")
{

	SUBCASE("Invalid Condition")
	{
		std::string t1 = "x = 0\n"
			"y = 0\n"
			"while print 5 do\n"
			"x = x + 1\n"
			"y = y + 10\n"
			"done\n";

		Tokenizer lexer(t1);
		std::vector<Token> tokens = lexer.tokenize();
		Environment env;
		Parser parser(tokens, &env);

		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected an expression } at line 3", SyntaxError);
	}

	SUBCASE("Missing 'do'")
	{
		std::string t1 = "x = 0\n"
			"y = 0\n"
			"while x < 5\n"
			"x = x + 1\n"
			"y = y + 10\n"
			"done\n";

		Tokenizer lexer(t1);
		std::vector<Token> tokens = lexer.tokenize();
		Environment env;
		Parser parser(tokens, &env);

		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected \"do\" after condition } at line 3", SyntaxError);
	}

	SUBCASE("Missing 'done'")
	{
		std::string t1 = "x = 0\n"
			"y = 0\n"
			"while x < 5 do\n"
			"x = x + 1\n"
			"y = y + 10\n"
			"\n";

		Tokenizer lexer(t1);
		std::vector<Token> tokens = lexer.tokenize();
		Environment env;
		Parser parser(tokens, &env);

		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected \"done\" } at line 7", SyntaxError);
	}

	SUBCASE("Expressions Used Instead Of Instructions")
	{
		std::string t1 = "x = 0\n"
			"y = 0\n"
			"while x < 5 do\n"
			"x + 1\n"
			"23 + 10\n"
			"done\n";

		Tokenizer lexer(t1);
		std::vector<Token> tokens = lexer.tokenize();
		Environment env;
		Parser parser(tokens, &env);

		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected variable initalization } at line 4", SyntaxError);
		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected a declaration } at line 5", SyntaxError);
	}

	SUBCASE("Using 'do' without a loop")
	{
		std::string t1 = "do x = x + 5\n";

		Tokenizer lexer(t1);
		std::vector<Token> tokens = lexer.tokenize();
		Environment env;
		Parser parser(tokens, &env);

		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { \"do\" must be used after a loop condition } at line 1", SyntaxError);
	}

	SUBCASE("Using 'done' without a loop")
	{
		std::string t1 = "done";

		Tokenizer lexer(t1);
		std::vector<Token> tokens = lexer.tokenize();
		Environment env;
		Parser parser(tokens, &env);

		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { \"done\" can not be used witout a while loop } at line 1", SyntaxError);
	}

	SUBCASE("Instructions After 'done' On Same Line")
	{
		std::string t1 = "x = 0\n"
			"y = 0\n"
			"while x < 5 do\n"
			"x = x + 1\n"
			"y = x + 10\n"
			"done print x\n";

		Tokenizer lexer(t1);
		std::vector<Token> tokens = lexer.tokenize();
		Environment env;
		Parser parser(tokens, &env);

		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected one instruction per row only } at line 6", SyntaxError);

		t1 = "x = 0\n"
			"y = 0\n"
			"while x < 5 do\n"
			"x = x + 1\n"
			"y = x + 10\n"
			"done 12 + 9\n";

		lexer = Tokenizer(t1);
		tokens = lexer.tokenize();
		parser = Parser(tokens, &env);

		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected one instruction per row only } at line 6", SyntaxError);
	}
}