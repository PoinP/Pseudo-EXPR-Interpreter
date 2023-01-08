#ifndef PRIMITIVE_H

#define PRIMITIVE_H

#include "../../Token/Token.h"
#include "Expression.h"

class Primitive : public Expression
{
public:
	Primitive(const Token& primitive)
		: m_Primitive(primitive)
	{}
	Primitive(const BigInteger& value)
		: m_Primitive(Token(TokenType::NUMBER, 0, value))
	{}

	BigInteger evaluate(Environment* env) const override
	{
		return m_Primitive.getLiteral();
	}

private:
	Token m_Primitive;
};

#endif // !PRIMITIVE_H
