#include "Token.h"

Token::Token(TokenType type, unsigned long long line, unsigned long long literal, const std::string& identifier)
	: m_Type(type), m_Line(line), m_Literal(literal), m_Identifier(identifier)
{
}

TokenType Token::getType() const
{
	return m_Type;
}

unsigned long long Token::getLine() const
{
	return m_Line;
}

unsigned long long Token::getLiteral() const
{
	return m_Literal;
}

const std::string& Token::getIdentifier() const
{
	return m_Identifier;
}
