#include <fstream>
#include <iostream>
#include <stdexcept>

#include "Interpreter/Interpreter.h"

#define DOCTEST_CONFIG_IMPLEMENT
#include "../tests/doctest.h"

//#define TESTING
//#define CMD_INPUT

#ifndef TESTING

void run(const char* path)
{
	std::ifstream input(path);

	if (!input.is_open() || !input)
		throw std::invalid_argument("File doesn't exist or someone's using it!");

	input.seekg(0, std::ios::end);
	size_t length = input.tellg();
	input.seekg(0, std::ios::beg);

	std::string srcCode(length, ' ');
	input.read(&srcCode[0], length);

	input.close();

	Interpreter interpreter(srcCode);
	interpreter.interpret();
}

int main(int argc, char* argv[])
{
#ifdef CMD_INPUT
	if (argc > 1)
	{
		try { run(argv[1]); }
		catch (const std::invalid_argument& ex) { std::cout << ex.what() << std::endl; }
	}
#else

	//run("test-code\\digitsSum.expr");
	//run("test-code\\factorial.expr");
	//run("test-code\\factorialRec.expr");
	//run("test-code\\fib.expr");
	//run("test-code\\fibRec.expr");
	//run("test-code\\isPowerOfTwo.expr");
	//run("test-code\\maxNum.expr");
	//run("test-code\\suffixPalindrom.expr");
	//run("test-code\\sumNums.expr");
	//run("test-code\\toBinary.expr");
	//run("test-code\\toBinaryRec.expr");
	//run("test-code\\unions.expr");

	//run("test-code\\invalidCode.expr");
	//run("test-code\\runtimeErrorCode.expr");

#endif // CMD_INPUT
}

#else

int main()
{
	doctest::Context().run();
}

#endif // TESTING