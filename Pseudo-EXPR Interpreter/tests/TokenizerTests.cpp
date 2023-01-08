#include "doctest.h"

#include <vector>
#include "../src/Token/Tokenizer.h"
#include "../src/Errors/SyntaxError.h"


TEST_CASE("Empty Source File")
{
	std::string src;
	Tokenizer lexer(src);
	std::vector<Token> tokens = lexer.tokenize();
	
	CHECK(tokens.size() == 2);

	CHECK(tokens[0].getType() == TokenType::END_OF_LINE);
	CHECK(tokens[1].getType() == TokenType::END_OF_FILE);
}

TEST_CASE("Only expressions")
{
	std::string src = "12 + 45 - 88723 * 12 / 34 = 22\n" + std::string("(12+12) - [11 * 3]\n");
	Tokenizer lexer(src);
	std::vector<Token> tokens = lexer.tokenize();

	TokenType tokenTypes[] = {TokenType::NUMBER, TokenType::PLUS, TokenType::NUMBER, TokenType::MINUS,
		TokenType::NUMBER, TokenType::PROD, TokenType::NUMBER, TokenType::DIV, TokenType::NUMBER, TokenType::EQUALS, TokenType::NUMBER,
		TokenType::END_OF_LINE, TokenType::OPEN_PAREN, TokenType::NUMBER, TokenType::PLUS, TokenType::NUMBER, TokenType::CLOSE_PAREN,
		TokenType::MINUS, TokenType::OPEN_BRACKET, TokenType::NUMBER, TokenType::PROD, TokenType::NUMBER, TokenType::CLOSE_BRACKET, 
		TokenType::END_OF_LINE, TokenType::END_OF_LINE, TokenType::END_OF_FILE };

	unsigned long long numbers[] = { 12, 45, 88723, 12, 34, 22, 12, 12, 11, 3 };

	int iter = 0;
	int numbersIter = 0;
	for (Token token : tokens)
	{
		CHECK((int)tokenTypes[iter] == (int)token.getType());
		
		if (tokenTypes[iter++] == TokenType::NUMBER)
		{
			CHECK(numbers[numbersIter++] == token.getLiteral());
		}
	}
}

TEST_CASE("All Tokens")
{
	std::string src = "= + - * / % == != ! < > <= >= and or ? : if then else while do done ( ) [ ] 1234 var print read FUNC\n";
	Tokenizer lexer(src);

	std::vector<Token> tokens = lexer.tokenize();
	
	TokenType types[] = { TokenType::EQUALS, TokenType::PLUS, TokenType::MINUS, TokenType::PROD, TokenType::DIV, 
		TokenType::MOD, TokenType::EQUAL_EQUAL, TokenType::NOT_EQUAL, TokenType::NOT, TokenType::LESS_THAN, 
		TokenType::GREATER_THAN, TokenType::LESS_EQUAL, TokenType::GREATER_EQUAL, TokenType::AND, TokenType::OR,
		TokenType::QUESTION, TokenType::COLON, TokenType::IF, TokenType::THEN, TokenType::ELSE, TokenType::WHILE, 
		TokenType::DO, TokenType::DONE, TokenType::OPEN_PAREN, TokenType::CLOSE_PAREN, TokenType::OPEN_BRACKET, 
		TokenType::CLOSE_BRACKET, TokenType::NUMBER, TokenType::VARIABLE, TokenType::PRINT, TokenType::READ, 
		TokenType::FUNCTION, TokenType::END_OF_LINE, TokenType::END_OF_LINE, TokenType::END_OF_FILE };

	int iter = 0;
	for (Token token : tokens)
	{
		CHECK(token.getType() == types[iter]);

		if (types[iter] == TokenType::VARIABLE)
			CHECK(token.getIdentifier() == "var");

		if (types[iter] == TokenType::FUNCTION)
			CHECK(token.getIdentifier() == "FUNC");

		if (types[iter] == TokenType::NUMBER)
			CHECK(token.getLiteral() == 1234);

		iter++;
	}
}

TEST_CASE("Comments test")
{
	std::string src = "// This is a comment\n" +
		std::string("a = 2 + 5\n") +
		std::string("/* a = 66\n") +
		std::string("b = 66") +
		std::string("*/c = 12");

	Tokenizer lexer(src);

	std::vector<Token> tokens = lexer.getTokens();

	TokenType expectedTypes[] = { TokenType::END_OF_LINE, TokenType::VARIABLE, TokenType::EQUALS, TokenType::NUMBER, 
		TokenType::PLUS, TokenType::NUMBER,TokenType::END_OF_LINE, TokenType::VARIABLE, TokenType::EQUALS, TokenType::NUMBER, 
		TokenType::END_OF_LINE, TokenType::END_OF_FILE };

	int iter = 0;
	for (Token token : tokens)
	{
		CHECK(token.getType() == expectedTypes[iter++]);
	}
}

TEST_CASE("Invalid Names")
{
	SUBCASE("Invalid Variable Name")
	{
		std::string src = "invalidVar = 12";
		Tokenizer lexer(src);
		CHECK_THROWS_WITH_AS(lexer.getTokens(), "Syntax Error: { Invalid variable name } at line 1", SyntaxError);
	}

	SUBCASE("Invalid Function Name")
	{
		std::string src = "MyFunc[x] = x + 10";
		Tokenizer lexer(src);
		CHECK_THROWS_WITH_AS(lexer.getTokens(), "Syntax Error: { Invalid function name } at line 1", SyntaxError);
	}

	SUBCASE("Invalid Name On Other Line")
	{
		std::string src = "MyFunc[x] = x + 10\n"
						  "y = 11\n"
						  "invalidVar = 12";

		Tokenizer lexer(src);

		CHECK_THROWS_WITH_AS(lexer.getTokens(), "Syntax Error: { Invalid function name } at line 1", SyntaxError);
		CHECK_THROWS_WITH_AS(lexer.getTokens(), "Syntax Error: { Invalid variable name } at line 3", SyntaxError);
	}
}

TEST_CASE("Invalid tokens")
{
	std::string src = "MYFUNC[x] = x + 10;\n"
					  "y = #11@\n"
					  "var = 12'";

	Tokenizer lexer(src);

	CHECK_THROWS_WITH_AS(lexer.tokenize(), "Syntax Error: { Invalid or unexpected token } at line 1", SyntaxError);
	CHECK_THROWS_WITH_AS(lexer.tokenize(), "Syntax Error: { Invalid or unexpected token } at line 2", SyntaxError);
	CHECK_THROWS_WITH_AS(lexer.tokenize(), "Syntax Error: { Invalid or unexpected token } at line 2", SyntaxError);
	CHECK_THROWS_WITH_AS(lexer.tokenize(), "Syntax Error: { Invalid or unexpected token } at line 3", SyntaxError);
}

TEST_CASE("Whitespcae characters")
{
	std::string src = "12        +			 45 - 88723 * 12 / 34    ";
	
	Tokenizer lexer(src);

	std::vector<Token> tokens = lexer.getTokens();

	TokenType expectedTypes[] = { TokenType::NUMBER, TokenType::PLUS, TokenType::NUMBER, TokenType::MINUS,
		TokenType::NUMBER, TokenType::PROD, TokenType::NUMBER, TokenType::DIV, TokenType::NUMBER,
		TokenType::END_OF_LINE, TokenType::END_OF_FILE };

	int iter = 0;
	for (Token token : tokens)
	{
		CHECK(token.getType() == expectedTypes[iter++]);
	}
}
