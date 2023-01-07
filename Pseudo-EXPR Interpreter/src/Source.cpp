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

#include <fstream>
#include <iostream>
#include "Interpreter/Interpreter.h"

//
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
		const char* path = "test2.expr";
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