#ifndef TOKEN_H

#define TOKEN_H

#include <string>

#include "TokenType.h"
#include "../BigInteger/BigInteger.h"

/// Token representation
class Token
{
public:
	/// Token Constructor
	///
	/// Constructs an instance of the Token class
	/// @param type The type of token
	/// @param line On which line is a token located
	/// @param lit  A literal that is bound to the Token
	/// @attention Set to zero if there isn't a literal or is an identifier
	/// @param identifier The name of the token
	/// @attention Set to blank if the token isn't an identifier(function or variable)
	Token(TokenType type, uint64_t line, const BigInteger& lit, const std::string& identifier = "");

	// @returns The type of the token
	TokenType getType() const;

	// @returns The line of the token
	uint64_t getLine() const;

	// @returns The literal value of the token
	const BigInteger& getLiteral() const;

	// @returns The token's identifier(name)
	const std::string& getIdentifier() const;

private:
	// The token's type
	TokenType m_Type;

	// The line that the token is located on
	uint64_t m_Line;

	// The literal bound to the Token
	BigInteger m_Literal;

	// The identifer(name) of the token
	std::string m_Identifier;
};

#endif // !TOKEN_H
