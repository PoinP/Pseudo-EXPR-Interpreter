#include "doctest.h"

#include "../src/Parser/Expressions/Expression.h"
#include "../src/Interpreter/Interpreter.h"
#include <fstream>

TEST_CASE("Constructor")
{
	Interpreter inter("x = 10");
	CHECK_NOTHROW(inter.interpret());
}

TEST_CASE("Are Error Handled Correctly")
{
	SUBCASE("Tokenizer Error")
	{
		std::string src = "invalidtoken@ = 2 // Invalid token\n";
		Interpreter interpreter(src);
		CHECK_NOTHROW(interpreter.interpret());
	}

	SUBCASE("Parser Error")
	{		
		std::string src = "x = print 2 // Expected an expression\n";
		Interpreter interpreter(src);
		CHECK_NOTHROW(interpreter.interpret());
	}

	SUBCASE("Run-Time Error") 
	{
		std::string src = "x = 2 / 0 // Division by zero";
		Interpreter interpreter(src);
		CHECK_NOTHROW(interpreter.interpret());
	}
}