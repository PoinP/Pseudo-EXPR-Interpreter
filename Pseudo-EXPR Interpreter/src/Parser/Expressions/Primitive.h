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

	unsigned long long evaluate() override
	{
		return m_Primitive.getLiteral();
	}

private:
	Token m_Primitive;
};

#endif // !PRIMITIVE_H
