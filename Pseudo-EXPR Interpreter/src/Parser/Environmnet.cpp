#include "Environmnet.h"

#include <cassert>

Environment::Environment()
{
}

Environment::~Environment()
{
    for (std::pair<std::string, Expression*> ex : m_Table)
    {
        delete ex.second;
    }
}

bool Environment::contains(const std::string& key) const
{
    return m_Table.find(key) != m_Table.end();
}

const Expression* Environment::get(const std::string& key) const
{
    return m_Table.at(key);
}

void Environment::set(const std::string& key, Expression* expr)
{
    if (contains(key))
        delete m_Table[key];

    m_Table[key] = expr;
}
