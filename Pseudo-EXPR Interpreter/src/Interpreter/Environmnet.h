#ifndef ENVIRONMENT_H

#define ENVIRONMENT_H

#include <string>
#include <unordered_map>

#include "../Utility/Helper.h"

class Environment
{
public:
	Environment();
	Environment(Environment* prevEnv);
	~Environment();

	void set(const std::string& key, Expression* expr);
	void setOnSelf(const std::string& key, Expression* expr);

	bool contains(const std::string& key) const;
	const Expression* get(const std::string& key) const;

private:
	std::unordered_map<std::string, Expression*> m_Table;
	Environment* m_PrevEnv;

	bool hasKey(const std::string& key) const;

	Environment(const Environment& other) = delete;
	Environment(Environment&& other) = delete;

	Environment& operator=(const Environment& other) = delete;
	Environment& operator=(Environment&& other) = delete;
};

#endif // !ENVIRONMENT_H