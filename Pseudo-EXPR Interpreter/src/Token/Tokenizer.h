#ifndef TOKENIZER_H

#define TOKENIZER_H

#include "Token.h"
#include "TokenType.h"

#include <string>
#include <vector>

class Tokenizer
{
public:
	Tokenizer(const std::string& srcCode);

	std::vector<Token> tokenize();
	std::vector<Token> getTokens();

private:
	std::string m_Src;
	std::vector<Token> m_Tokens;

	size_t m_Iterator;
	size_t m_CurrLine;

	// -------- Source Code Traversal --------
	void next();
	char consume();

	char peek() const;
	char peekNext() const;
	size_t currPos() const;

	// -------- Utility --------
	std::string constructWord();
	unsigned long long constructNumber();
	void addToken(TokenType type, void* lit = nullptr);

	bool isDigit(char c) const;
	bool isLetter(char c) const;
};

#endif // !TOKENIZER_H