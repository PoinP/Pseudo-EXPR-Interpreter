#include "Environmnet.h"

#include "../Utility/Helper.h"
#include "../Parser/Expressions/Expression.h"

Environment::Environment()
    : m_PrevEnv(nullptr)
{}

Environment::Environment(Environment* prevEnv)
    : m_PrevEnv(prevEnv)
{}

Environment::~Environment()
{
    for (std::pair<std::string, Expression*> entry : m_Table)
    {
        delete entry.second;
    }
}

void Environment::set(const std::string& key, Expression* expr)
{   
    if (hasKey(key))
    {
        delete m_Table[key];
        m_Table[key] = expr;
        return;
    }

    if (!contains(key))
    {
        m_Table[key] = expr;
        return;
    }

    if (m_PrevEnv)
        m_PrevEnv->set(key, expr);
}

void Environment::setOnSelf(const std::string& key, Expression* expr)
{
    if (hasKey(key))
        delete m_Table[key];

    m_Table[key] = expr;
}

bool Environment::contains(const std::string& key) const 
{ 
    if (!hasKey(key))
    {
        if (m_PrevEnv) 
            return m_PrevEnv->contains(key);

        return false;
    }

    return true;
}
const Expression* Environment::get(const std::string& key) const 
{ 
    if (!hasKey(key))
    {
        if (m_PrevEnv)
            return m_PrevEnv->get(key);
    }

    return m_Table.at(key); 
}

bool Environment::hasKey(const std::string& key) const
{
    return m_Table.find(key) != m_Table.end();
}
