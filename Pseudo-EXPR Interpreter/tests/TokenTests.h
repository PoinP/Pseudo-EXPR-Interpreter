#include "../src/Token/Token.h"

#include "doctest.h"

TEST_CASE("Constructors")
{
	Token token(TokenType::NUMBER, new int(12), 2);

	SUBCASE("Normal onstructor")
	{
		CHECK(token.getType() == TokenType::NUMBER);
		CHECK(token.getLine() == 2);
		CHECK(*(int*)token.getLiteral() == 12);
	}

	SUBCASE("Copy constructor")
	{
		Token copy(token);

		CHECK(copy.getType() == TokenType::NUMBER);
		CHECK(copy.getLine() == 2);
		CHECK(*(int*)copy.getLiteral() == 12);

		*(int*)copy.getLiteral() = 234;
		CHECK(*(int*)token.getLiteral() == 12);
		CHECK(*(int*)copy.getLiteral() == 234);
	}

	SUBCASE("Move constructor")
	{
		Token copy(std::move(token));

		CHECK(copy.getType() == TokenType::NUMBER);
		CHECK(copy.getLine() == 2);
		CHECK(*(int*)copy.getLiteral() == 12);
	}
}

TEST_CASE("Assignment Operators")
{
	Token token(TokenType::VARIABLE, new std::string("var"), 2);

	SUBCASE("Copy assignment")
	{
		Token copy(TokenType::NUMBER, new int(12), 5);
		copy = token;

		CHECK(copy.getType() == TokenType::VARIABLE);
		CHECK(copy.getLine() == 2);
		CHECK(*(std::string*)copy.getLiteral() == "var");
	}

	SUBCASE("Move assignment")
	{
		Token copy(TokenType::NUMBER, new int(12), 5);
		copy = std::move(token);

		CHECK(copy.getType() == TokenType::VARIABLE);
		CHECK(copy.getLine() == 2);
		CHECK(*(std::string*)copy.getLiteral() == "var");

		CHECK(token.getType() == TokenType::NUMBER);
		CHECK(token.getLine() == 5);
		CHECK(*(int*)token.getLiteral() == 12);
	}
}