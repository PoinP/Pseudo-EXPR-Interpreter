#include "../doctest.h"

#include "../../src/Utility/Helper.h"

#include "../../src/Token/Tokenizer.h"
#include "../../src/Parser/Parser.h"

#include "../../src/Interpreter/Environmnet.h"

#include "../../src/Parser/Expressions/Unary.h"
#include "../../src/Parser/Expressions/Primitive.h"


TEST_CASE("Correct Evaluations")
{
	Expression* expr = nullptr;
	Environment env;

	Expression* n1 = new Primitive(1);
	expr = new Unary(Token(TokenType::NOT, 1, 0), n1);

	CHECK(expr->evaluate(&env) == 0);

	delete expr;
	n1 = new Primitive(0);
	expr = new Unary(Token(TokenType::NOT, 1, 0), n1);

	CHECK(expr->evaluate(&env) == 1);

	delete expr;
}

TEST_CASE("Invalid Evaluations")
{
	Expression* expr = nullptr;
	Environment env;

	Expression* n1 = new Primitive(1);
	expr = new Unary(Token(TokenType::MINUS, 1, 0), n1);

	CHECK_THROWS_WITH_AS(expr->evaluate(&env), "Run-Time Error: { Usage of negative numbers } at line 1", RunTimeError);

	delete expr;
	n1 = new Primitive(1);
	expr = new Unary(Token(TokenType::PROD, 1, 0), n1);

	CHECK_THROWS_WITH_AS(expr->evaluate(&env), "Run-Time Error: { Unexpected behavior } at line 1", RunTimeError);

	delete expr;
}

TEST_CASE("Chaining Operators")
{
	Tokenizer t("x = !!!!!!!1");
	Environment env;
	Parser p(t.getTokens(), &env);

	p.parse()[0]->run();
	CHECK(env.get("x")->evaluate(&env) == 0);

	t = Tokenizer("x = !!!!!!1");
	p = Parser(t.tokenize(), &env);
	p.parse()[0]->run();

	CHECK(env.get("x")->evaluate(&env) == 1);
}