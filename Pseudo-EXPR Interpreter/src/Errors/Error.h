#ifndef ERROR_H

#define ERROR_H

#include <stdexcept>
#include <string>

/// Used to throw Errors while interpreting
class Error : public std::exception
{
public:
	/// Sets a value to m_What
	Error(const char* what)
		: m_What(what)
	{}

	/// Same as Error(const char* what)
	Error(const std::string& what)
		: m_What(what)
	{}

	/// @returns The value of m_What
	const char* what() const override
	{
		return m_What.c_str();
	}

private:
	/// The massage that would be diplayed
	/// if the Error gets thrown
	std::string m_What;
};

#endif // !ERROR_H

