#include "../doctest.h"

#include "../../src/Utility/Helper.h"

#include "../../src/Token/Tokenizer.h"
#include "../../src/Parser/Parser.h"

#include "../../src/Interpreter/Environmnet.h"

#include "../../src/Parser/Expressions/Primitive.h"
#include "../../src/Parser/Expressions/Binary.h"

#include "../../src/Errors/SyntaxError.h"


TEST_CASE("Correct Evaluations")
{
	Expression* n1 = new Primitive(10);
	Expression* n2 = new Primitive(5);
	Expression* expr = nullptr;
	Environment env;

	SUBCASE("Addition")
	{
		expr = new Binary(n1, Token(TokenType::PLUS, 1, 0), n2);
		CHECK(expr->evaluate(&env) == 15);
	}

	SUBCASE("Mutiplication")
	{
		expr = new Binary(n1, Token(TokenType::PROD, 1, 0), n2);
		CHECK(expr->evaluate(&env) == 50);
	}

	SUBCASE("Subtraction")
	{
		expr = new Binary(n1, Token(TokenType::MINUS, 1, 0), n2);
		CHECK(expr->evaluate(&env) == 5);
	}

	SUBCASE("Division")
	{
		expr = new Binary(n1, Token(TokenType::DIV, 1, 0), n2);
		CHECK(expr->evaluate(&env) == 2);
	}

	SUBCASE("Modulo")
	{
		delete n2;
		n2 = new Primitive(3);

		expr = new Binary(n1, Token(TokenType::MOD, 1, 0), n2);
		CHECK(expr->evaluate(&env) == 1);
	}

	SUBCASE("Less Than")
	{
		expr = new Binary(n1, Token(TokenType::LESS_THAN, 1, 0), n2);
		CHECK(expr->evaluate(&env) == false);
	}

	SUBCASE("Less Than Or Equal")
	{
		delete n2;
		n2 = new Primitive(10);

		expr = new Binary(n1, Token(TokenType::LESS_EQUAL, 1, 0), n2);
		CHECK(expr->evaluate(&env) == true);
	}

	SUBCASE("Greater Than")
	{
		expr = new Binary(n1, Token(TokenType::GREATER_THAN, 1, 0), n2);
		CHECK(expr->evaluate(&env) == true);
	}

	SUBCASE("Greater Than Or Equal")
	{
		delete n2;
		n2 = new Primitive(11);

		expr = new Binary(n1, Token(TokenType::GREATER_EQUAL, 1, 0), n2);
		CHECK(expr->evaluate(&env) == false);
	}

	SUBCASE("Equal")
	{
		expr = new Binary(n1, Token(TokenType::EQUAL_EQUAL, 1, 0), n2);
		CHECK(expr->evaluate(&env) == false);

		delete expr;

		n1 = new Primitive(10);
		n2 = new Primitive(10);

		expr = new Binary(n1, Token(TokenType::EQUAL_EQUAL, 1, 0), n2);

		CHECK(expr->evaluate(&env) == true);
	}

	SUBCASE("Not Equal")
	{
		expr = new Binary(n1, Token(TokenType::NOT_EQUAL, 1, 0), n2);
		CHECK(expr->evaluate(&env) == true);

		delete expr;

		n1 = new Primitive(10);
		n2 = new Primitive(10);

		expr = new Binary(n1, Token(TokenType::NOT_EQUAL, 1, 0), n2);

		CHECK(expr->evaluate(&env) == false);
	}

	SUBCASE("AND Logic")
	{
		expr = new Binary(n1, Token(TokenType::AND, 1, 0), n2);
		CHECK(expr->evaluate(&env) == true);

		delete expr;

		n1 = new Primitive(10);
		n2 = new Primitive(0);

		expr = new Binary(n1, Token(TokenType::AND, 1, 0), n2);

		CHECK(expr->evaluate(&env) == false);
	}

	SUBCASE("OR Logic")
	{
		delete n1;
		n1 = new Primitive(0);

		expr = new Binary(n1, Token(TokenType::OR, 1, 0), n2);
		CHECK(expr->evaluate(&env) == true);

		delete expr;

		n1 = new Primitive(0);
		n2 = new Primitive(0);
		expr = new Binary(n1, Token(TokenType::OR, 1, 0), n2);

		CHECK(expr->evaluate(&env) == false);
	}

	delete expr;
}

TEST_CASE("Invalid Evaluations")
{
	Expression* n1 = new Primitive(5);
	Expression* n2 = new Primitive(0);
	Expression* expr = nullptr;
	Environment env;

	SUBCASE("Subtraction")
	{
		expr = new Binary(n2, Token(TokenType::MINUS, 1, 0), n1);
		CHECK_THROWS_WITH_AS(expr->evaluate(&env), "Run-Time Error: { Usage of negative numbers } at line 1", RunTimeError);
	}

	SUBCASE("Division")
	{
		expr = new Binary(n1, Token(TokenType::DIV, 1, 0), n2);
		CHECK_THROWS_WITH_AS(expr->evaluate(&env), "Run-Time Error: { Division by zero } at line 1", RunTimeError);
	}

	SUBCASE("Modulo")
	{

		expr = new Binary(n1, Token(TokenType::MOD, 1, 0), n2);
		CHECK_THROWS_WITH_AS(expr->evaluate(&env), "Run-Time Error: { Division by zero } at line 1", RunTimeError);
	}

	delete expr;
}

TEST_CASE("Invalid Operator")
{
	Expression* n1 = new Primitive(1);
	Expression* n2 = new Primitive(2);
	Expression* expr = nullptr;
	Environment env;

	expr = new Binary(n1, Token(TokenType::NOT, 1, 0), n2);
	CHECK_THROWS_WITH_AS(expr->evaluate(&env), "Run-Time Error: { Unexpected behavior } at line 1", RunTimeError);

	delete expr;
}

TEST_CASE("Operator Chaining")
{
	SUBCASE("Logical Operators")
	{
		Tokenizer t("x = 1 and 1 and 1 and 1 and 0 or 1 and 0");
		Environment env;
		Parser p(t.getTokens(), &env);

		p.parse()[0]->run();
		CHECK(env.get("x")->evaluate(&env) == 0);

		t = Tokenizer("x = 0 and 0 or 0 or 0 or 1 or 0 and 0 and 1");
		p = Parser(t.tokenize(), &env);

		p.parse()[0]->run();
		CHECK(env.get("x")->evaluate(&env) == 1);
	}

	SUBCASE("Equality Operators")
	{
		Tokenizer t("x = 1 == 1 == 1 == 1 != 0 != 1 != 0");
		Environment env;
		Parser p(t.getTokens(), &env);

		p.parse()[0]->run();
		CHECK(env.get("x")->evaluate(&env) == 0);

		t = Tokenizer("x = 1 == 1 != 0");
		p = Parser(t.tokenize(), &env);

		p.parse()[0]->run();
		CHECK(env.get("x")->evaluate(&env) == 1);
	}

	SUBCASE("Arithmethic Operators")
	{
		Tokenizer t("x = 10 + 5 + 78 - 53 + 89 - 8 + 19");
		Environment env;
		Parser p(t.getTokens(), &env);

		p.parse()[0]->run();
		CHECK(env.get("x")->evaluate(&env) == 140);
	}

	SUBCASE("Factor Operators")
	{
		Tokenizer t("x = 10 * 5 * 78 / 53 * 89 / 8 % 19");
		Environment env;
		Parser p(t.getTokens(), &env);

		p.parse()[0]->run();
		CHECK(env.get("x")->evaluate(&env) == 14);
	}
}

TEST_CASE("Invalid Parsing")
{
	SUBCASE("Logic Or")
	{
		Tokenizer lexer("x = 1 or print 1");
		Environment env;
		Parser parser(lexer.tokenize(), &env);
		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected an expression } at line 1", SyntaxError);
	}

	SUBCASE("Logic And")
	{
		Tokenizer lexer("x = 1 and print 1");
		Environment env;
		Parser parser(lexer.tokenize(), &env);
		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected an expression } at line 1", SyntaxError);
	}

	SUBCASE("Equality")
	{
		Tokenizer lexer("x = 1 == print 1");
		Environment env;
		Parser parser(lexer.tokenize(), &env);
		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected an expression } at line 1", SyntaxError);
	}

	SUBCASE("Non Equality")
	{
		Tokenizer lexer("x = 1 != print 1");
		Environment env;
		Parser parser(lexer.tokenize(), &env);
		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected an expression } at line 1", SyntaxError);
	}

	SUBCASE("Comparison")
	{
		Tokenizer lexer("x = 1 > print 1");
		Environment env;
		Parser parser(lexer.tokenize(), &env);
		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected an expression } at line 1", SyntaxError);
	}

	SUBCASE("Arithmetic")
	{
		Tokenizer lexer("x = 1 + print 1");
		Environment env;
		Parser parser(lexer.tokenize(), &env);
		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected an expression } at line 1", SyntaxError);
	}

	SUBCASE("Factor")
	{
		Tokenizer lexer("x = 1 * print 1");
		Environment env;
		Parser parser(lexer.tokenize(), &env);
		CHECK_THROWS_WITH_AS(parser.parse(), "Syntax Error: { Expected an expression } at line 1", SyntaxError);
	}
}