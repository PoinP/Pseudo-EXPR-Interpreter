#include <fstream>
#include <iostream>
#include "Interpreter/Interpreter.h"
#include "Token/Tokenizer.h"
#include "Parser/Parser.h"

//#define DOCTEST_CONFIG_IMPLEMENT
//#include "../tests/doctest.h"

//#define TESTING

#ifndef TESTING

#ifdef _DEBUG
#define new new( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define new new
#endif

//#define CMD_INPUT

int main(int argc, char* argv[])
{
	{

		#ifdef CMD_INPUT

		std::string path = argv[1];

		#else

		//const char* path = "test2.expr";
		const char* path = "toBinary.expr";

		#endif // CMD_INPUT


		std::ifstream input(path);

		input.seekg(0, std::ios::end);
		size_t length = input.tellg();
		input.seekg(0, std::ios::beg);

		std::string srcCode(length, ' ');
		input.read(&srcCode[0], length);

		input.close();

		Interpreter interpreter(srcCode);
		interpreter.interpret();
	}

	_CrtDumpMemoryLeaks();
}


#else

int main()
{
	doctest::Context().run();
}

#endif // TESTING