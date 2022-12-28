#include "Token.h"

Token::Token(TokenType type, void* liter, unsigned long long line)
	: m_Type(type), m_Literal(liter), m_Line(line)
{
}

Token::Token(const Token& other)
{
	copy(other);
}

Token::Token(Token&& other) noexcept
{
	move(std::move(other));
}

Token::~Token()
{
	if (m_Literal)
	{
		if (m_Type == TokenType::VARIABLE)
		{
			delete (std::string*)(m_Literal);
		}
		else if (m_Type == TokenType::NUMBER)
		{
			delete (unsigned long long*)(m_Literal);
		}

		// unreachable
	}
}

void Token::copy(const Token& other)
{
	m_Type = other.m_Type;
	m_Line = other.m_Line;

	if (other.m_Type == TokenType::VARIABLE)
	{
		std::string varName = *(static_cast<std::string*>(other.m_Literal));
		m_Literal = new std::string(varName);
	}

	if (other.m_Type == TokenType::NUMBER)
	{
		unsigned long long number = *(static_cast<unsigned long long*>(other.m_Literal));
		m_Literal = new unsigned long long(number);
	}
}

void Token::move(Token&& other)
{
	std::swap(m_Type, other.m_Type);
	std::swap(m_Line, other.m_Line);
	std::swap(m_Literal, other.m_Literal);
}

Token& Token::operator=(const Token& other)
{
	if (this != &other)
	{
		delete m_Literal;
		copy(other);
	}

	return *this;
}

Token& Token::operator=(Token&& other) noexcept
{
	move(std::move(other));
	
	return *this;
}

TokenType Token::getType() const
{
	return m_Type;
}

unsigned long long Token::getLine() const
{
	return m_Line;
}

void* Token::getLiteral() const
{
	return m_Literal;
}
