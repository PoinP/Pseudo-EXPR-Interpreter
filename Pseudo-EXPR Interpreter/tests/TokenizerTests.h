#include <vector>
#include "../src/Token/Tokenizer.h"
#include "doctest.h"

TEST_CASE("Empty Source File")
{
	std::string src;
	Tokenizer scanner(src);
	std::vector<Token> tokens = scanner.tokenize();

	for (Token token : tokens)
	{
		CHECK(token.getType() == TokenType::END_OF_FILE);
	}
}

TEST_CASE("Only expressions")
{
	std::string src = "12 + 45 - 88723 * 12 / 34 = 22\n" + std::string("(12+12) - [11 * 3]\n");
	Tokenizer scanner(src);
	std::vector<Token> tokens = scanner.tokenize();

	TokenType tokenTypes[] = {TokenType::NUMBER, TokenType::PLUS, TokenType::NUMBER, TokenType::MINUS,
		TokenType::NUMBER, TokenType::PROD, TokenType::NUMBER, TokenType::DIV, TokenType::NUMBER, TokenType::EQUALS, TokenType::NUMBER,
		TokenType::END_OF_LINE, TokenType::OPEN_PAREN, TokenType::NUMBER, TokenType::PLUS, TokenType::NUMBER, TokenType::CLOSE_PAREN,
		TokenType::MINUS, TokenType::OPEN_BRACKET, TokenType::NUMBER, TokenType::PROD, TokenType::NUMBER, TokenType::CLOSE_BRACKET, TokenType::END_OF_LINE,
		TokenType::END_OF_FILE };

	unsigned long long numbers[] = { 12, 45, 88723, 12, 34, 22, 12, 12, 11, 3 };

	int iter = 0;
	int numbersIter = 0;
	for (Token token : tokens)
	{
		CHECK((int)tokenTypes[iter] == (int)token.getType());
		
		if (tokenTypes[iter++] == TokenType::NUMBER)
		{
			CHECK(numbers[numbersIter++] == *(static_cast<unsigned long long*>(token.getLiteral())));
		}
	}
}

TEST_CASE("All Tokens")
{
	enum TokenType
	{
		EQUALS, PLUS, MINUS, PROD, DIV,
		EQUAL_EQUAL, NOT_EQUAL, NOT,
		LESS_THAN, GREATER_THAN,
		LESS_EQUAL, GREATER_EQUAL,
		AND, OR,
		IF, THEN, ELSE,
		WHILE, DO, DONE,

		OPEN_PAREN, CLOSE_PAREN,
		OPEN_BRACKET, CLOSE_BRACKET,

		NUMBER, VARIABLE,

		PRINT, READ,

		END_OF_FILE, END_OF_LINE
	};

	std::string src = "= + - * / == != ! < > <= >= and or if then else while do done ( ) [ ] 1234 var print read\n";
	Tokenizer scanner(src);

	std::vector<Token> tokens = scanner.tokenize();
	
	TokenType types[] = { EQUALS, PLUS, MINUS, PROD, DIV, EQUAL_EQUAL, NOT_EQUAL, NOT, LESS_THAN, GREATER_THAN, 
		LESS_EQUAL, GREATER_EQUAL, AND, OR, IF, THEN, ELSE, WHILE, DO, DONE, OPEN_PAREN, CLOSE_PAREN, 
		OPEN_BRACKET, CLOSE_BRACKET, NUMBER, VARIABLE, PRINT, READ, END_OF_LINE, END_OF_FILE };

	int iter = 0;
	for (Token token : tokens)
	{
		CHECK((int)token.getType() == types[iter]);

		if (types[iter] == VARIABLE)
			CHECK(*(std::string*)token.getLiteral() == "var");

		if (types[iter] == NUMBER)
			CHECK(*(unsigned long long*)token.getLiteral() == 1234);

		iter++;
	}
}

TEST_CASE("Comments test")
{
	enum TokenType
	{
		EQUALS, PLUS, MINUS, PROD, DIV,
		EQUAL_EQUAL, NOT_EQUAL, NOT,
		LESS_THAN, GREATER_THAN,
		LESS_EQUAL, GREATER_EQUAL,
		AND, OR,
		IF, THEN, ELSE,
		WHILE, DO, DONE,

		OPEN_PAREN, CLOSE_PAREN,
		OPEN_BRACKET, CLOSE_BRACKET,

		NUMBER, VARIABLE,

		PRINT, READ,

		END_OF_FILE, END_OF_LINE
	};

	std::string src = "// This is a comment\n" +
		std::string("a = 2 + 5\n") +
		std::string("/* a = 66\n") +
		std::string("b = 66") +
		std::string("*/c = 12");

	Tokenizer scanner(src);

	std::vector<Token> tokens = scanner.getTokens();

	TokenType expectedTypes[] = {END_OF_LINE, VARIABLE, EQUALS, NUMBER, PLUS, NUMBER,
		END_OF_LINE, VARIABLE, EQUALS, NUMBER, END_OF_FILE };

	int iter = 0;
	for (Token token : tokens)
	{
		CHECK((int)token.getType() == expectedTypes[iter++]);
	}
}