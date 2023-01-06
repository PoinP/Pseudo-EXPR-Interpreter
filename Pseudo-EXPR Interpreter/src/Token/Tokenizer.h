#ifndef TOKENIZER_H

#define TOKENIZER_H

#include "Token.h"
#include "TokenType.h"

#include <string>
#include <list>

class Tokenizer
{
public:
	Tokenizer(const std::string& srcCode);

	std::list<Token> tokenize();
	std::list<Token> getTokens();

private:
	std::string m_Src;
	std::list<Token> m_Tokens;

	size_t m_Iterator;
	size_t m_CurrLine;

	void collectTokens();

	// -------- Source Code Traversal --------
	void next();
	char consume();

	char peek() const;
	char peekNext() const;
	size_t currPos() const;

	// -------- Utility --------
	std::string constructWord();
	BigInteger constructNumber();
	void addToken(TokenType type, const BigInteger& lit = 0, const std::string& name = "");

	bool isDigit(char c) const;
	bool isLetter(char c) const;
	bool isUppercase(char c) const;
	bool onlyUppercase(const std::string& word) const;
	bool onlyLowercase(const std::string& word) const;

	void addWord(const std::string& word);
};

#endif // !TOKENIZER_H