#include "../doctest.h"

#include "../../src/Utility/Helper.h"

#include "../../src/Token/Tokenizer.h"
#include "../../src/Parser/Parser.h"

#include "../../src/Interpreter/Environmnet.h"

#include "../../src/Parser/Expressions/Ternary.h"
#include "../../src/Parser/Expressions/Primitive.h"

#include "../../src/Errors/SyntaxError.h"

TEST_CASE("Correct Evaluations")
{
	Expression* expr = nullptr;
	Environment env;

	Expression* cond = new Primitive(1);
	Expression* ifTrue = new Primitive(10);
	Expression* ifFalse = new Primitive(20);

	expr = new Ternary(cond, ifTrue, ifFalse);

	CHECK(expr->evaluate(&env) == 10);

	delete expr;

	cond = new Primitive(0);
	ifTrue = new Primitive(10);
	ifFalse = new Primitive(20);

	expr = new Ternary(cond, ifTrue, ifFalse);

	CHECK(expr->evaluate(&env) == 20);

	delete expr;
}

TEST_CASE("Chaining operators")
{
	Tokenizer t("x = 10 > 5 ? 3 < 1 ? 100 : 50 : 60");
	Environment env;
	Parser p(t.getTokens(), &env);

	p.parse()[0]->run();
	CHECK(env.get("x")->evaluate(&env) == 50);

	t = Tokenizer("x = 10 < 5 ? 3 < 1 ? 100 : 50 : 60");
	p = Parser(t.tokenize(), &env);
	p.parse()[0]->run();

	CHECK(env.get("x")->evaluate(&env) == 60);

	t = Tokenizer("x = 10 > 5 ? 3 > 1 ? 100 : 50 : 60");
	p = Parser(t.tokenize(), &env);
	p.parse()[0]->run();

	CHECK(env.get("x")->evaluate(&env) == 100);
}

TEST_CASE("Parsing")
{
	SUBCASE("Correct parsing")
	{
		Tokenizer lexer("x = 1 > 0 ? 1 : 0");
		Environment env;
		Parser parser(lexer.tokenize(), &env);
		CHECK_NOTHROW(parser.parse());
	}

	SUBCASE("Invalid Condition")
	{
		Tokenizer lexer("x = print 2 ? 1 : 0");
		Environment env;
		Parser parser(lexer.tokenize(), &env);
		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected an expression } at line 1", SyntaxError);
	}

	SUBCASE("Invalid If True Expression")
	{
		Tokenizer lexer("x = 1 > 0 ? print 1 : 0");
		Environment env;
		Parser parser(lexer.tokenize(), &env);
		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected an expression } at line 1", SyntaxError);
	}

	SUBCASE("Invalid If False Expression")
	{
		Tokenizer lexer("x = 1 > 0 ? 1 : print 3");
		Environment env;
		Parser parser(lexer.tokenize(), &env);
		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected an expression } at line 1", SyntaxError);
	}

	SUBCASE("Missing Colon")
	{
		Tokenizer lexer("x = 1 > 0 ? 1 0");
		Environment env;
		Parser parser(lexer.tokenize(), &env);
		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected ':' } at line 1", SyntaxError);
	}
}