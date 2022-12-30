#ifndef ENVIRONMENT_H

#define ENVIRONMENT_H

#include <string>
#include <unordered_map>
#include "Expressions/Expression.h"

class Environment
{
public:
	Environment();
	~Environment();

	void set(const std::string& key, Expression* expr);
	bool contains(const std::string& key) const;
	const Expression* get(const std::string& key) const;

private:
	std::unordered_map<std::string, Expression*> m_Table;
};

#endif // !ENVIRONMENT_H
