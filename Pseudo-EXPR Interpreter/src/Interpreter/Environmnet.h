#ifndef ENVIRONMENT_H

#define ENVIRONMENT_H

#include <string>
#include <unordered_map>

#include "../Utility/Helper.h"

/// Holds information about a specific scope
///
/// This class is used to handle identifier scopes.
/// It stores a hash table of names(indentifiers)
/// and their specific expressions
/// @note The copy and move constructors are deleted
class Environment
{
public:
	/// Constructs an empty environment
	Environment();
	/// Constructs a sub environment(or a parent) of prevEnv
	/// @param prevEnv The environment that encapsulates the current one
	Environment(Environment* prevEnv);
	/// Used to delete the allocated memory of the expressions bound to an identifier
	~Environment();

	/// Sets and expression to a specific key
	/// 
	/// Before actually setting the key it checks if
	/// the key is present in the current hash table.
	/// If the key isn't present then it searches recursively
	/// among the parent environments. If the key is found in
	/// any of the parent environments then the key of that
	/// specific environemnet gets set to the new expr. If it
	/// isn't located in any of the parent Environment then it creates
	/// a new key with the new expression on itself.
	/// @param key The hash table key
	/// @param expr The expression to set the corresponding value of key to
	void set(const std::string& key, Expression* expr);

	/// Sets and expression to a specific key
	///
	/// It sets a key on the current environment while ignoring
	/// the other parent environments
	/// @param key The hash table key
	/// @param expr The expression to set the corresponding value of key to
	void setOnSelf(const std::string& key, Expression* expr);

	/// Chcks if the hash map contains a specific key
	///
	/// This function checks recursively if this or any
	/// of the parent environments contain a specific key
	/// @param key The hash table key to look for
	/// @returns If the key exists in the table
	bool contains(const std::string& key) const;

	/// Gets a specific expression with a key from the table
	///
	/// It first checks the local environment. If the key does
	/// not exist there, then it checks recursively if the key
	/// is contained in a parent environment
	/// @param key The hash table key to look for
	/// @returns The expression associated with the key
	/// @throws std::out_of_range is no enviroment has the
	/// specific key 
	const Expression* get(const std::string& key) const;

private:
	/// The hash table that contains the key-expression pairs
	std::unordered_map<std::string, Expression*> m_Table;
	/// A pointer to a parent environment
	Environment* m_PrevEnv;

	/// Checks if the local enviroment has a specific key
	/// @returns If the local environment has a specific key
	bool hasKey(const std::string& key) const;

	Environment(const Environment& other) = delete;
	Environment(Environment&& other) = delete;

	Environment& operator=(const Environment& other) = delete;
	Environment& operator=(Environment&& other) = delete;
};

#endif // !ENVIRONMENT_H