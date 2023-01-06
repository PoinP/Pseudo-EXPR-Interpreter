#include "Token.h"

Token::Token(TokenType type, uint64_t line, const BigInteger& lit, const std::string& identifier)
	: m_Type(type), m_Line(line), m_Literal(lit), m_Identifier(identifier)
{
}

TokenType Token::getType() const
{
	return m_Type;
}

uint64_t Token::getLine() const
{
	return m_Line;
}

const BigInteger& Token::getLiteral() const
{
	return m_Literal;
}

const std::string& Token::getIdentifier() const
{
	return m_Identifier;
}
