#ifndef TOKEN_H

#define TOKEN_H

#include <string>
#include "TokenType.h"

class Token
{
public:
	Token(TokenType type, void* liter, unsigned long long line);
	Token(const Token& other);
	Token(Token&& other) noexcept;
	~Token();

	Token& operator=(const Token& other);
	Token& operator=(Token&& other) noexcept;

	TokenType getType() const;
	unsigned long long getLine() const;
	void* getLiteral() const;

private:
	TokenType m_Type;
	void* m_Literal;
	unsigned long long m_Line;

	void copy(const Token& other);
	void move(Token&& other);
};

#endif // !TOKEN_H
