#ifndef TOKEN_H

#define TOKEN_H

#include <string>

#include "TokenType.h"
#include "../BigInteger/BigInteger.h"

class Token
{
public:
	Token(TokenType type, uint64_t line, const BigInteger& lit, const std::string& identifier = "");

	TokenType getType() const;
	uint64_t getLine() const;
	const BigInteger& getLiteral() const;
	const std::string& getIdentifier() const;

private:
	TokenType m_Type;
	uint64_t m_Line;
	BigInteger m_Literal;
	std::string m_Identifier;
};

#endif // !TOKEN_H
