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
//	std::vector<Token> tokens = tokenizer.tokenize();
//
//	std::cout << "Done!";
//}