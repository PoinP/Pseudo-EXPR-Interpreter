#ifndef TOKENIZER_H

#define TOKENIZER_H

#include "Token.h"
#include "TokenType.h"

#include <string>
#include <vector>

/// Splits the source code into tokens
///
/// This class splits the source code into tokens
/// while it also validates if a token is even supported.
class Tokenizer
{
public:
	/// Tokenizer Constructor
	///
	/// Creates an instance of the Tokenizer class
	/// @param srcCode The program's source code which
	/// you need to tokenize
	Tokenizer(const std::string& srcCode);


	/// The main function of the class
	///
	/// It goes through each character of the source code
	/// and splits every element into a seperate token.
	/// @returns The generated tokens from the source code
	/// @throws SyntaxError exception if a token is invalid or isn't supported
	/// @warning Even if the function throws an exception, if you call tokenize()
	/// again after catching it, the tokenizing will continue right where it threw
	/// the exception while ignoring the invalid token.
	std::vector<Token> tokenize();

	/// Gets the generated tokens
	///
	/// It checks if there are any generated tokens
	/// and returns them. If there are no genrated tokens
	/// then this function calls tokenize().
	/// @see tokenize() for more info
	std::vector<Token> getTokens();

	/// Checks if all of the source code was tokenized
	///
	/// @return If the source code iterator is at '\0' character
	bool isAtEnd() const;

private:
	/// A copy of the passed source code
	std::string m_Src;
	/// A container storing all of the generated tokens
	std::vector<Token> m_Tokens;

	/// Current position in m_Src
	size_t m_Iterator;
	/// Current line in m_Src
	size_t m_CurrLine;

	/// Parses the source code into tokens
	///
	/// It moves the m_Iterator while taking
	/// all of the valid tokens and storing them
	/// into m_Tokens. This is the heart of this class
	/// and parses all of the passed source code.
	/// @throws SyntaxError if there was an error
	/// when parsing the source code into tokens
	void collectTokens();

	// -------- Source Code Traversal --------

	/// Skips the current character in m_Src
	/// and increases the value of m_Iterator with 1
	void next();

	/// Consumes the current character while
	/// also calling next() afterwards
	/// 
	/// @return The current character right before skipping it
	char consume();

	/// Allows you to check what character are you on
	///
	/// @retruns The current character
	char peek() const;

	/// Allows you to peek what the next character is
	///
	/// @return The next character
	/// @attention This function uses std::string's
	/// operator[] so it will you throw if you are
	/// located at the end of m_Src
	char peekNext() const;

	/// @returns The current position of the m_Iterator
	size_t currPos() const;

	// -------- Utility --------

	/// Used to generate a word from the
	/// current postion of m_Iterator
	/// 
	/// @returns The generated word
	/// @attention This function moves m_Iterator
	std::string constructWord();

	/// Used to generate a whole number from
	/// the current postion of m_Iterator
	/// 
	/// @returns The generated number
	/// @attention This function moves m_Iterator
	BigInteger constructNumber();

	/// Adds a token to m_Tokens
	///
	/// @param type The token's type
	/// @param lit  The token's literal value
	/// @attention Leave 0 for non numbers
	/// @param name The name of the token
	/// @attention Leave blank if the token isn't
	/// an identifier
	void addToken(TokenType type, const BigInteger& lit = 0, const std::string& name = "");

	/// @param c The character to check
	/// @returns If the c was a digit
	bool isDigit(char c) const;

	/// @param c The character to check
	/// @returns If the c was a letter
	bool isLetter(char c) const;

	/// @param c The character to check
	/// @returns If the c is an uppercase letter
	bool isUppercase(char c) const;

	/// @param word The word to check
	/// @returns If word has only uppercase letters
	bool onlyUppercase(const std::string& word) const;

	/// @param word The word to check
	/// @returns If word has only lowercase letters
	bool onlyLowercase(const std::string& word) const;

	/// Adds word as either a variable or a function
	///
	/// @param word The word to add to m_Tokens
	/// @throws SyntaxError if word isn't a valid
	/// name for a variable or a function
	void addWord(const std::string& word);
};

#endif // !TOKENIZER_H