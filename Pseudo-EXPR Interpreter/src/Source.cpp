//#include "Token/Tokenizer.h"
//#include <fstream>
//#include <iostream>
//
//int main()
//{
//	const char* path = "test.txt";
//
//	std::ifstream input(path);
//
//	input.seekg(0, std::ios::end);
//	size_t length = input.tellg();
//	input.seekg(0, std::ios::beg);
//
//	std::string srcCode(length, ' ');
//	input.read(&srcCode[0], length);
//
//	input.close();
//
//	Tokenizer tokenizer(srcCode);
//
//	std::list<Token> tokens = tokenizer.tokenize();
//
//	std::cout << "Done!";
//}

#include "Token/Tokenizer.h"
#include <fstream>
#include "Parser/Parser.h"
#include <iostream>

#include "Parser/Expressions/Binary.h"
#include <stdexcept>

#include "Parser/Instructions/Instruction.h"
#include "Parser/Expressions/Primitive.h"

//
#ifdef _DEBUG
#define new new( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define new new
#endif

int main()
{
	{
		const char* path = "test.expr";

		std::ifstream input(path);

		input.seekg(0, std::ios::end);
		size_t length = input.tellg();
		input.seekg(0, std::ios::beg);

		std::string srcCode(length, ' ');
		input.read(&srcCode[0], length);

		input.close();

		Tokenizer tokenizer(srcCode);

		std::list<Token> tokens = tokenizer.tokenize();

		Parser parser(tokens);

		unsigned int test = (265 + 76 * 9 + 8 / 12 + (12 - 2 - 3) * 10);

		std::list<Instruction*> parsed;

		try {
			parsed = parser.parse();
		}
		catch (const std::exception& e)
		{
			std::cout << e.what();
		}

		for (auto p : parsed)
		{
			try
			{
				p->run();
			}
			catch (const std::exception& e)
			{
				std::cout << e.what();
				return -1;
			}

			delete p;
		}
	}

	_CrtDumpMemoryLeaks();
}