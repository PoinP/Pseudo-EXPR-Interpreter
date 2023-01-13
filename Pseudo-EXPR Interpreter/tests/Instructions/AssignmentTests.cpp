#include "../doctest.h"

#include "../../src/Parser/Parser.h"
#include "../../src/Token/Tokenizer.h"

#include "../../src/Errors/SyntaxError.h"


TEST_CASE("Number Assignment")
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

TEST_CASE("Expression Assignment")
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

TEST_CASE("Function Assignment")
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

TEST_CASE("Variable Assignment")
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

TEST_CASE("Non expression assignment")
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

TEST_CASE("More Than One Instruction On Row")
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
