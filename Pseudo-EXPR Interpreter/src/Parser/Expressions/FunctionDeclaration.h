#ifndef FUNCTION_DECLARATION_H

#define FUNCTION_DECLARATION_H

#include "Expression.h"

/// Handles function declarations
///
/// This class is used for declaring a function
/// and storing its members in the Environment
/// @note Only functions with single parameters
/// are supported
class FunctionDeclaration : public Expression 
{
public:
	/// Function declaration Constructor
	///
	/// Takes all of the needed parts
	/// for a function to work and stores
	/// them into an expression which can
	/// be stored into an environment
	/// @param func The function's token
	/// @param param The function's parameter token
	/// @body The body that gets evaluated when the
	/// function is invoked
	/// @attention The body expression is handled by
	/// Declaration and it shouldn't be deleted inside
	/// this class
	FunctionDeclaration(const Token& func, const Token& param, Expression* body)
		: m_Func(func), m_Param(param), m_Body(body)
	{}

	/// @returns The body of the function
	const Expression* getBody() const { return m_Body; }
	/// @returns the name of the function
	const std::string& getName() const { return m_Func.getIdentifier(); }
	/// @returns the name of the parameter
	const std::string& getParamName() const { return m_Param.getIdentifier(); }

	/// !DOES NOT DO ANYTHING!
	/// @returns It always evaluates to 0
	/// @attention This override does not do anything.
	/// It is put because it inherits the interface Expression
	/// so an override must be made!
	BigInteger evaluate(Environment* env) const override { return 0; }

private:
	/// The function's token
	Token m_Func;
	/// The parameter's token
	Token m_Param;
	/// The body of the function
	Expression* m_Body;
};

#endif // !FUNCTION_DECLARATION_H
