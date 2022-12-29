#ifndef ERROR_H

#define ERROR_H

#include <stdexcept>
#include <string>

class Error : public std::exception
{
public:
	Error(const char* what)
		: m_What(what)
	{}

	Error(const std::string& what)
		: m_What(what)
	{}

	const char* what() const override
	{
		return m_What.c_str();
	}

private:
	std::string m_What;
};

#endif // !ERROR_H

