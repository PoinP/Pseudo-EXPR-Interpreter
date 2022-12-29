#include "Tokenizer.h"
#include "../Exceptions/Log.h"

Tokenizer::Tokenizer(const std::string& srcCode)
    : m_Src(srcCode), m_Iterator(0), m_CurrLine(1)
{
}

std::vector<Token> Tokenizer::tokenize()
{
    char currChar = peek();
    while (currChar != '\0')
    {
        currChar = peek();

        // Handles unsigned long long numbers
        if (isDigit(currChar))
        {
            int number = constructNumber();
            addToken(TokenType::NUMBER, new unsigned long long(number));
            continue;
        }

        // Handles specific language words
        if (isLetter(currChar))
        {
            std::string word = constructWord();

            if (word == "print")        addToken(TokenType::PRINT);
            else if (word == "read")    addToken(TokenType::READ);
            else if (word == "and")     addToken(TokenType::AND);
            else if (word == "or")      addToken(TokenType::OR);
            else if (word == "if")      addToken(TokenType::IF);
            else if (word == "then")    addToken(TokenType::THEN);
            else if (word == "else")    addToken(TokenType::ELSE);
            else if (word == "while")   addToken(TokenType::WHILE);
            else if (word == "do")      addToken(TokenType::DO);
            else if (word == "done")    addToken(TokenType::DONE);
            else                        addToken(TokenType::VARIABLE, new std::string(word));

            continue;
        }
        
        // Handles Comments
        if (currChar == '/')
        {
            if (peekNext() == '/')
            {
                while (peek() != '\n' && peek() != '\0')
                {
                    next();
                }

                continue;
            }
            
            if (peekNext() == '*')
            {
                // Continues while it meets a '*/' or ends the source code
                while ((peek() != '*' || peekNext() != '/') && peek() != '\0')
                {
                    if (peek() == '\n')
                        m_CurrLine++;

                    next();
                }

                // Skips the */
                next();
                next();

                continue;
            }
        }

        TokenType type;

        switch (currChar)
        {
        case ' ':
        case '\t':
        case '\r':
        case '\0': 
            break;

        case '+': addToken(TokenType::PLUS); break;
        case '-': addToken(TokenType::MINUS); break;
        case '*': addToken(TokenType::PROD); break;
        case '/': addToken(TokenType::DIV); break;

        case '[': addToken(TokenType::OPEN_BRACKET); break;
        case ']': addToken(TokenType::CLOSE_BRACKET); break;
        case '(': addToken(TokenType::OPEN_PAREN); break;
        case ')': addToken(TokenType::CLOSE_PAREN); break;

        case '=':
            type = peekNext() == '=' ? TokenType::EQUAL_EQUAL : TokenType::EQUALS;
            if (peekNext() == '=') next();
            addToken(type);
            break;

        case '!':
            if (peekNext() == '=') next();
            type = peek() == '=' ? TokenType::NOT_EQUAL : TokenType::NOT;
            addToken(type);
            break;

        case '>':
            if (peekNext() == '=') next();
            type = peek() == '=' ? TokenType::GREATER_EQUAL : TokenType::GREATER_THAN;
            addToken(type);
            break;
            
        case '<':
            if (peekNext() == '=') next();
            type = peek() == '=' ? TokenType::LESS_EQUAL : TokenType::LESS_THAN;
            addToken(type);
            break;

        case '\n':
            addToken(TokenType::END_OF_LINE);
            m_CurrLine++;
            break;
        default:
            Log::syntaxError("Invalid or unexpected token", m_CurrLine);
        }

        next();
    }

    addToken(TokenType::END_OF_FILE);

    return m_Tokens;
}

std::vector<Token> Tokenizer::getTokens()
{
    if (m_Tokens.empty())
        return tokenize();

    return m_Tokens;
}

void Tokenizer::next()
{
    m_Iterator++;
}

char Tokenizer::peek() const
{
    return m_Src[m_Iterator];
}

char Tokenizer::peekNext() const
{
    if (peek() == '\0') return '\0';
    return m_Src[m_Iterator + 1];
}

size_t Tokenizer::currPos() const
{
    return m_Iterator;
}

char Tokenizer::consume()
{
    return m_Src[m_Iterator++];
}

void Tokenizer::addToken(TokenType type, void* lit)
{
    m_Tokens.push_back(Token(type, lit, m_CurrLine));
}

std::string Tokenizer::constructWord()
{
    size_t startOfWord = currPos();

    while (isLetter(peek()) || isDigit(peek()))
    {
        next();
    }

    std::string::const_iterator begin = m_Src.begin() + startOfWord;
    std::string::const_iterator end = m_Src.begin() + currPos();

    return std::string(begin, end);
}

unsigned long long Tokenizer::constructNumber()
{
    unsigned long long number = 0;

    while (isDigit(peek()))
    {
        int digit = consume() - '0';
        number = number * 10 + digit;
    }

    return number;
}

bool Tokenizer::isDigit(char c) const
{
    return c >= '0' && c <= '9';
}

bool Tokenizer::isLetter(char c) const
{
    return c >= 'a' && c <= 'z';
}