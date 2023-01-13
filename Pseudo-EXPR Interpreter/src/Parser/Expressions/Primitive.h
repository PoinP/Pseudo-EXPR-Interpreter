#ifndef PRIMITIVE_H

#define PRIMITIVE_H

#include "../../Token/Token.h"
#include "Expression.h"

/// Handles primitives(numbers)
class Primitive : public Expression
{
public:
	/// Constructs a primitive object through a primitive token
	/// @param primitive The primitive token
	Primitive(const Token& primitive)
		: m_Primitive(primitive)
	{}

	/// Constructs a primitive object through a value(number)
	/// @param value The value that the primitive gets set to
	Primitive(const BigInteger& value)
		: m_Primitive(Token(TokenType::NUMBER, 0, value))
	{}

	/// @returns the value of the primitive
	/// @note The passed environment does not matter!
	BigInteger evaluate(Environment* env) const override
	{
		return m_Primitive.getLiteral();
	}

private:
	/// The token that keeps the primitive's value
	Token m_Primitive;
};

#endif // !PRIMITIVE_H
