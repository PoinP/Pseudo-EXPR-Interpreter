#ifndef FUNCTION_CALLER_H

#define FUNCTION_CALLER_H

#include "Expression.h"
#include "FunctionDeclaration.h"

/// Handles function calling
///
/// This class is responsible for
/// searching for a declared function
/// in the Environmnet and invoking it
/// though a FunctionDeclaration object
class FunctionCaller : public Expression
{
public:
	/// A FunctionCaller constructor
	///
	/// Makes an object that stores in itself
	/// the token of the function you want to
	/// invoke and the argument which is passed
	/// to said function
	/// @param func The token of the function to invoke
	/// @param arg The argument that is passed to the
	/// invoked function
	FunctionCaller(const Token& func, Expression* arg)
		: m_Func(func), m_Arg(arg)
	{}

	/// Deletes the argument
	~FunctionCaller()
	{
		delete m_Arg;
	}

	/// Invokes the function in the secified environment
	///
	/// Before invkoing a function, a local function
	/// Environment is created which is used for invoking
	/// the function. This allows for the creation of 
	/// recursive functions.
	/// @returns The value of the function's evaluation
	/// @throws RunTimeError if the function is undefined 
	/// @throws RunTimeError if there was an internal error 
	BigInteger evaluate(Environment* env) const override
	{
		Environment functionScope(env);

		if (!functionScope.contains(m_Func.getIdentifier()))
			throw RunTimeError("Function \"" + m_Func.getIdentifier() + "\" undefined", m_Func.getLine());

		const Expression* funcExpr = functionScope.get(m_Func.getIdentifier());
		const FunctionDeclaration* func = dynamic_cast<const FunctionDeclaration*>(funcExpr);

		if (!func)
			throw RunTimeError("Internal error! Expression not a function!", m_Func.getLine());

		BigInteger argValue = m_Arg->evaluate(&functionScope);
		functionScope.setOnSelf(func->getParamName(), new Primitive(argValue));

		return func->getBody()->evaluate(&functionScope);
	}

private:
	/// The function's token
	Token m_Func;
	/// The argument that is passed
	/// to the function
	Expression* m_Arg;
};

#endif // !FUNCTION_CALLER_H
