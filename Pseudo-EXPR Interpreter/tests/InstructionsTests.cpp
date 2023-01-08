#include "doctest.h"

#include <string>

#include "../src/Parser/Parser.h"
#include "../src/Token/Tokenizer.h"
#include "../src/Errors/SyntaxError.h"
#include "../src/Errors/RuntimeError.h"
#include "../src/Parser/Expressions/Primitive.h"


TEST_CASE("Assignment Tests")
{
	SUBCASE("Number Assignment")
	{
		std::string t1 = "x = 12";

		Tokenizer lexer(t1);
		std::vector<Token> tokens = lexer.tokenize();
		Environment env;
		Parser parser(tokens, &env);
		std::vector<Instruction*> instrs = parser.parse();

		instrs[0]->run();

		CHECK(env.get("x")->evaluate(&env) == 12);
	}

	SUBCASE("Expression Assignment")
	{
		std::string t1 = "x = (10 + 10 * 10)";

		Tokenizer lexer(t1);
		std::vector<Token> tokens = lexer.tokenize();
		Environment env;
		Parser parser(tokens, &env);
		std::vector<Instruction*> instrs = parser.parse();

		instrs[0]->run();

		CHECK(env.get("x")->evaluate(&env) == 110);
	}

	SUBCASE("Function Assignment")
	{

		std::string t1 = "FUNC[x] = x + 10\n"
						 "x = FUNC[10]";

		Tokenizer lexer(t1);
		std::vector<Token> tokens = lexer.tokenize();
		Environment env;
		Parser parser(tokens, &env);
		std::vector<Instruction*> instrs = parser.parse();

		instrs[0]->run();
		instrs[1]->run();

		CHECK(env.get("x")->evaluate(&env) == 20);
	}

	SUBCASE("Variable Assignment")
	{
		std::string t1 = "x = 10\n"
						 "y = x";

		Tokenizer lexer(t1);
		std::vector<Token> tokens = lexer.tokenize();
		Environment env;
		Parser parser(tokens, &env);
		std::vector<Instruction*> instrs = parser.parse();

		instrs[0]->run();
		instrs[1]->run();

		CHECK(env.get("x")->evaluate(&env) == 10);
		CHECK(env.get("y")->evaluate(&env) == 10);
	}

	SUBCASE("Non expression assignment")
	{
		std::string t1 = "x = print 20";

		Tokenizer lexer(t1);
		std::vector<Token> tokens = lexer.tokenize();
		Environment env;
		Parser parser(tokens, &env);

		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected an expression } at line 1", SyntaxError);

		t1 = "x =";
		lexer = Tokenizer(t1);
		tokens = lexer.tokenize();
		parser = Parser(tokens, &env);

		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected an expression } at line 1", SyntaxError);
	}

	SUBCASE("More Than One Instruction On Row")
	{
		std::string t1 = "x = 56 2 + 3";

		Tokenizer lexer(t1);
		std::vector<Token> tokens = lexer.tokenize();
		Environment env;
		Parser parser(tokens, &env);

		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected one instruction per row only } at line 1", SyntaxError);

		t1 = "x = 56 print 2";
		lexer = Tokenizer(t1);
		tokens = lexer.tokenize();
		parser = Parser(tokens, &env);

		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected one instruction per row only } at line 1", SyntaxError);
	}
}

TEST_CASE("Function Tests")
{
	SUBCASE("Declaration")
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
	}

	SUBCASE("Wrong Declarations")
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

	SUBCASE("More Than One Instruction On Row")
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
}

TEST_CASE("Condition(If-Else) Tests")
{
	SUBCASE("Declaration")
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
		}
	}

	SUBCASE("Invalid Condition")
	{
		std::string t1 = "if print 2 then x = 1 else x = 0\n";

		Tokenizer lexer(t1);
		std::vector<Token> tokens = lexer.tokenize();
		Environment env;
		Parser parser(tokens, &env);

		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected an expression } at line 1", SyntaxError);
	}

	SUBCASE("Invalid Condition Without 'then'")
	{
		std::string t1 = "if print 2\n";

		Tokenizer lexer(t1);
		std::vector<Token> tokens = lexer.tokenize();
		Environment env;
		Parser parser(tokens, &env);

		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected an expression } at line 1", SyntaxError);
	}

	SUBCASE("Missing 'then'")
	{
		std::string t1 = "if 60 > 30 x = 1 else x = 0\n";

		Tokenizer lexer(t1);
		std::vector<Token> tokens = lexer.tokenize();
		Environment env;
		Parser parser(tokens, &env);

		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected \"then\" after condition } at line 1", SyntaxError);
	}

	SUBCASE("Missing Instruction If True")
	{
		std::string t1 = "if 60 > 30 then\n";

		Tokenizer lexer(t1);
		std::vector<Token> tokens = lexer.tokenize();
		Environment env;
		Parser parser(tokens, &env);

		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected a declaration } at line 1", SyntaxError);
	}

	SUBCASE("Missing 'else'")
	{
		std::string t1 = "if 60 > 30 then x = 1\n";

		Tokenizer lexer(t1);
		std::vector<Token> tokens = lexer.tokenize();
		Environment env;
		Parser parser(tokens, &env);

		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected \"else\" after declaration } at line 1", SyntaxError);
	}

	SUBCASE("Missing Instruction If False")
	{
		std::string t1 = "if 60 > 30 then x = 1 else\n";

		Tokenizer lexer(t1);
		std::vector<Token> tokens = lexer.tokenize();
		Environment env;
		Parser parser(tokens, &env);

		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected a declaration } at line 1", SyntaxError);
	}

	SUBCASE("Expressions Instead Of Declarations")
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

	SUBCASE("Using 'else' Without 'if'")
	{
		std::string t1 = "else (5 + 5)\n";

		Tokenizer lexer(t1);
		std::vector<Token> tokens = lexer.tokenize();
		Environment env;
		Parser parser(tokens, &env);

		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { \"else\" can not be used without an \"if\" expression } at line 1", SyntaxError);
	}

	SUBCASE("Using 'then' Without 'if'")
	{
		std::string t1 = "then (5 + 5)\n";

		Tokenizer lexer(t1);
		std::vector<Token> tokens = lexer.tokenize();
		Environment env;
		Parser parser(tokens, &env);

		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { \"then\" must be used after an \"if\" condition } at line 1", SyntaxError);
	}

	SUBCASE("More Than One Instruction On Row")
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
}

TEST_CASE("Loop Tests")
{
	SUBCASE("Declaration")
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
	}

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

TEST_CASE("Print Tests")
{
	SUBCASE("Declaration")
	{
		std::string t1 = "print 10";

		Tokenizer lexer(t1);
		std::vector<Token> tokens = lexer.tokenize();
		Environment env;
		Parser parser(tokens, &env);
		CHECK_NOTHROW(parser.parse());
	}

	SUBCASE("Invalid Expression")
	{
		std::string t1 = "print print 2";

		Tokenizer lexer(t1);
		std::vector<Token> tokens = lexer.tokenize();
		Environment env;
		Parser parser(tokens, &env);

		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected an expression } at line 1", SyntaxError);
	}

	SUBCASE("Has More Than One Instruction On Row")
	{
		std::string t1 = "print 5 print 5\n";

		Tokenizer lexer(t1);
		std::vector<Token> tokens = lexer.tokenize();
		Environment env;
		Parser parser(tokens, &env);

		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected one instruction per row only } at line 1", SyntaxError);
	}
}

TEST_CASE("Read Tests")
{
	SUBCASE("Declaration")
	{
		std::string t1 = "read x";

		Tokenizer lexer(t1);
		std::vector<Token> tokens = lexer.tokenize();
		Environment env;
		Parser parser(tokens, &env);
		CHECK_NOTHROW(parser.parse());
	}

	SUBCASE("Non Variable Argument")
	{
		std::string t1 = "read (2 + 2)";

		Tokenizer lexer(t1);
		std::vector<Token> tokens = lexer.tokenize();
		Environment env;
		Parser parser(tokens, &env);

		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Can not apply value to a non variable token } at line 1", SyntaxError);
	}

	SUBCASE("Has More Than One Instruction On Row")
	{
		std::string t1 = "read x print 5\n";

		Tokenizer lexer(t1);
		std::vector<Token> tokens = lexer.tokenize();
		Environment env;
		Parser parser(tokens, &env);

		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected one instruction per row only } at line 1", SyntaxError);
	}
}