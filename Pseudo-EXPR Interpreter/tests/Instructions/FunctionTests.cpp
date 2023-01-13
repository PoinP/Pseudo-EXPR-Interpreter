#include "../doctest.h"

#include "../../src/Parser/Parser.h"
#include "../../src/Token/Tokenizer.h"
#include "../../src/Errors/SyntaxError.h"

TEST_CASE("Declaration")
{
	std::string t1 = "FUNC[x] = x * 20\n"
		"x = FUNC[2]";

	Tokenizer lexer(t1);
	std::vector<Token> tokens = lexer.tokenize();
	Environment env;
	Parser parser(tokens, &env);
	std::vector<Instruction*> instrs = parser.parse();

	instrs[0]->run();
	instrs[1]->run();

	CHECK(env.get("x")->evaluate(&env) == 40);

	for (Instruction* i : instrs)
		delete i;
}

TEST_CASE("Wrong Declarations")
{
	SUBCASE("Missing An Open Bracket")
	{
		std::string t1 = "FUNC = 12\n";
		Tokenizer lexer(t1);
		std::vector<Token> tokens = lexer.tokenize();
		Environment env;
		Parser parser(tokens, &env);

		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected a parameter after function declaration } at line 1", SyntaxError);
	}

	SUBCASE("Zero Parameter Function")
	{
		std::string t1 = "FUNC[] = 12\n";
		Tokenizer lexer(t1);
		std::vector<Token> tokens = lexer.tokenize();
		Environment env;
		Parser parser(tokens, &env);

		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { 0 parameters are not supported } at line 1", SyntaxError);
	}

	SUBCASE("Wrong parameters")
	{
		SUBCASE("A Number")
		{
			std::string t1 = "FUNC[23] = 12\n";
			Tokenizer lexer(t1);
			std::vector<Token> tokens = lexer.tokenize();
			Environment env;
			Parser parser(tokens, &env);

			CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Fuction parameter expected } at line 1", SyntaxError);
		}

		SUBCASE("An Expression")
		{
			std::string t1 = "FUNC[(56 * 9) / 8] = 12\n";
			Tokenizer lexer(t1);
			std::vector<Token> tokens = lexer.tokenize();
			Environment env;
			Parser parser(tokens, &env);

			CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Fuction parameter expected } at line 1", SyntaxError);
		}

		SUBCASE("An Instruction")
		{
			std::string t1 = "FUNC[print 2] = 12\n";
			Tokenizer lexer(t1);
			std::vector<Token> tokens = lexer.tokenize();
			Environment env;
			Parser parser(tokens, &env);

			CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Fuction parameter expected } at line 1", SyntaxError);
		}
	}

	SUBCASE("Multiple Parameters")
	{
		std::string t1 = "FUNC[x y] = 12\n";
		Tokenizer lexer(t1);
		std::vector<Token> tokens = lexer.tokenize();
		Environment env;
		Parser parser(tokens, &env);

		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected a ']' with single parameter } at line 1", SyntaxError);
	}

	SUBCASE("Invalid Declaration")
	{
		std::string t1 = "FUNC[x]\n";
		Tokenizer lexer(t1);
		std::vector<Token> tokens = lexer.tokenize();
		Environment env;
		Parser parser(tokens, &env);

		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Invalid function declaration } at line 1", SyntaxError);
	}

	SUBCASE("No Body")
	{
		std::string t1 = "FUNC[x] = \n";
		Tokenizer lexer(t1);
		std::vector<Token> tokens = lexer.tokenize();
		Environment env;
		Parser parser(tokens, &env);

		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected an expression } at line 1", SyntaxError);
	}

	SUBCASE("Invalid Body")
	{
		std::string t1 = "FUNC[x] = print 12\n";
		Tokenizer lexer(t1);
		std::vector<Token> tokens = lexer.tokenize();
		Environment env;
		Parser parser(tokens, &env);

		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected an expression } at line 1", SyntaxError);
	}
}

TEST_CASE("More Than One Instruction On Row")
{
	std::string t1 = "FUNC[x] = x + 10 print 10\n";
	Tokenizer lexer(t1);
	std::vector<Token> tokens = lexer.tokenize();
	Environment env;
	Parser parser(tokens, &env);

	CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected one instruction per row only } at line 1", SyntaxError);

	t1 = "FUNC[x] = x + 10 12 + 9\n";
	lexer = Tokenizer(t1);
	tokens = lexer.tokenize();
	parser = Parser(tokens, &env);

	CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected one instruction per row only } at line 1", SyntaxError);
}