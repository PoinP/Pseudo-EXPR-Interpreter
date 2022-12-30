#ifndef ENVIRONMENT_H

#define ENVIRONMENT_H

#include <string>
#include <unordered_map>

class Environment
{
public:
	Environment();

	void add(const std::string& key, unsigned long long value);
	bool contains(const std::string& key) const;
	unsigned long long get(const std::string& key) const;

	unsigned long long& operator[](const std::string& key);
	unsigned long long operator[](const std::string& key) const;

private:
	std::unordered_map<std::string, unsigned long long> m_Table;
};

#endif // !ENVIRONMENT_H