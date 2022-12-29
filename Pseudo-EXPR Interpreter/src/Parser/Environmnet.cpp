#include "Environmnet.h"

#include <cassert>

Environment::Environment()
{
}

void Environment::add(const std::string& key, unsigned long long value)
{
    if (!contains(key))
    {
        add(key, value);
        return;
    }

    assert("This variable already exists!");
}

bool Environment::contains(const std::string& key) const
{
    return m_Table.find(key) != m_Table.end();
}

unsigned long long Environment::get(const std::string& key) const
{
    return m_Table.at(key);
}

unsigned long long& Environment::operator[](const std::string& key)
{
    return m_Table[key];
}

unsigned long long Environment::operator[](const std::string& key) const
{
    return get(key);
}
