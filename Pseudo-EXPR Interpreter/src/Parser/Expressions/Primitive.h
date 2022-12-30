#ifndef PRIMITIVE_H

#define PRIMITIVE_H

#include "Expression.h"
#include "../../Token/Token.h"

class Primitive : public Expression
{
public:
	Primitive(const Token& primitive)
		: m_Primitive(primitive)
	{}
	Primitive(unsigned long long value)
		: m_Primitive(Token(TokenType::NUMBER, 0, value))
	{}

	unsigned long long evaluate(Environment* env) const override
	{
		return m_Primitive.getLiteral();
	}

private:
	Token m_Primitive;
};

#endif // !PRIMITIVE_H
