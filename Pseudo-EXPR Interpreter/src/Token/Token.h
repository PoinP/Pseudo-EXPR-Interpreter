#ifndef TOKEN_H

#define TOKEN_H

#include <string>
#include "TokenType.h"

class Token
{
public:
	Token(TokenType type, unsigned long long line, unsigned long long literal, const std::string& identifier = "");

	TokenType getType() const;
	unsigned long long getLine() const;
	unsigned long long getLiteral() const;
	const std::string& getIdentifier() const;

private:
	TokenType m_Type;
	unsigned long long m_Line;
	unsigned long long m_Literal;
	std::string m_Identifier;
};

#endif // !TOKEN_H
