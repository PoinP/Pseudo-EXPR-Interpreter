#ifndef EXPRESSION_H

#define EXPRESSION_H

#include "../../Utility/Helper.h"
#include "../../Errors/RunTimeError.h"
#include "../../BigInteger/BigInteger.h"

/// Expression interface
///
/// This is the interface that is
/// inherited by all of the other
/// expressions.
/// @note The copy and move constructors /
/// assignment operators are deleted
class Expression
{
public:
	Expression() {}
	virtual ~Expression() {}

	/// Evaluates an expression
	/// @param env The Environmnt in which
	/// the expression will be evaluated
	/// @returns The result of the evaluation
	virtual BigInteger evaluate(Environment* env) const = 0;

private:
	/// Deleted Constructor
	Expression(const Expression& other) = delete;
	/// Deleted Constructor
	Expression(Expression&& other) = delete;

	/// Deleted Constructor
	Expression& operator=(const Expression& other) = delete;
	/// Deleted Constructor
	Expression& operator=(Expression&& other) = delete;
};

#endif // !EXPRESSION_H