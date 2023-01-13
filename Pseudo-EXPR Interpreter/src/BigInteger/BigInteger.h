#ifndef BIG_INTEGER_H

#define BIG_INTEGER_H

#include <iostream>
#include <deque>
#include <string>

/// A class that supports arbitrary integer length
///
/// This class adds support of so-called BigIntegers.
/// It allows a user to store an unsigned integer with
/// as much length as possible. This class adds support
/// for big integeres through storing a container of numbers
/// with a specified radix. In this implementation the chosen
/// radix is 100. That means that each element of the container
/// doesn't store a bigger number than 100. 
class BigInteger
{
public:
	/// This implementation uses a base of 100
	/// while the maximum supprted value of each
	/// element is of size uint16_t. The set up
	/// base is 100 beacuse of the overflow problem
	/// that appears when mutiplying big elements (Ex. 90 * 90) 
	using RadixSize = uint16_t;

public:
	/// Constructs a BigInteger with a value if 0
	BigInteger();

	/// Constructs a BigInteger with a value of number
	/// @param number The value that the object will have
	/// @throws std::invalid_argumnet if number is negative
	BigInteger(int32_t number); // Used for implicitly converting zero

	/// Constructs a BigInteger with a value of number
	/// @param number The value that the object will have
	BigInteger(uint64_t number);

	/// Converts strNumber into a number and
	/// constructs an object with its value
	/// @param strNumber The number in string format
	/// @throws std::invalid_argument if strNumber has
	/// other characters other than digits
	BigInteger(const char* strNumber);

	/// Converts strNumber into a number and
	/// constructs an object with its value.
	/// It invokes BigInteger(const char* strNumber)
	/// @see BigInteger(const char* strNumber) for more information
	BigInteger(const std::string& strNumber);

	// -------- OPERATOR OVERLOADING --------
	operator std::string() const { return convertToString(); }
	explicit operator bool() const { return size() > 1 || m_Numbers[0] != 0; } // Add tests

	/// @returns If lhs is equal to rhs
	friend bool operator==(const BigInteger& lhs, const BigInteger& rhs);
	/// @returns If lhs is not equal to rhs
	friend bool operator!=(const BigInteger& lhs, const BigInteger& rhs);
	/// @returns If lhs is greater than rhs
	friend bool operator>(const BigInteger& lhs, const BigInteger& rhs);
	/// @returns If lhs is greater than or equal to rhs
	friend bool operator>=(const BigInteger& lhs, const BigInteger& rhs);
	/// @returns If lhs is less than rhs
	friend bool operator<(const BigInteger& lhs, const BigInteger& rhs);
	/// @returns If lhs is less than or equal to rhs
	friend bool operator<=(const BigInteger& lhs, const BigInteger& rhs);

	/// Adds two numbers together by adding each number's coresponding numeration
	/// @returns The sum of lhs and rhs
	friend BigInteger operator+(const BigInteger& lhs, const BigInteger& rhs);
	/// Subtracts two numbers together by subtracting each number's coresponding numeration
	/// @returns The difference of lhs and rhs
	friend BigInteger operator-(const BigInteger& lhs, const BigInteger& rhs);
	/// Multiplies two numbers by using long multiplication method
	/// @return The products of lhs and rhs
	/// @throws std::runtime_error if trying to subtract a larger number from a smaller one
	friend BigInteger operator*(const BigInteger& lhs, const BigInteger& rhs);
	/// Divides two numbers by using long division method
	/// @returns The quotient of dividing lhs by rhs
	/// @throws std::runtime_error if trying to divide by zero
	friend BigInteger operator/(const BigInteger& lhs, const BigInteger& rhs);
	/// Divides two numbers by using long division method
	/// @returns The reminder of dividing lhs by rhs
	/// @throws std::runtime_error if trying to divide by zero
	friend BigInteger operator%(const BigInteger& lhs, const BigInteger& rhs);

	/// Increments the object by 1
	/// @returns A reference to the current object
	BigInteger& operator++();
	/// Increments the object by 1
	/// @returns A copy of the object before incrementing
	BigInteger operator++(int);

	/// Decrements the object by 1
	/// @returns A reference to the current object
	/// @throws std::runtime_error if decremeneting
	/// causes a negative number
	BigInteger& operator--();
	/// Decrements the object by 1
	/// @returns A copy of the object before decrementing
	/// @throws std::runtime_error if decremeneting
	/// causes a negative number
	BigInteger operator--(int);

	/// Adds the value of rhs to the current object
	/// @return A reference to the current object
	/// @see operator+(const BigInteger& lhs, const BigIntegers& rhs)
	/// for additional information
	BigInteger& operator+=(const BigInteger& rhs);

	/// Subtracts the value of rhs from the current object
	/// @return A reference to the current object
	/// @see operator-(const BigInteger& lhs, const BigIntegers& rhs)
	/// for additional information
	BigInteger& operator-=(const BigInteger& rhs);

	/// Mutiplies the current object with rhs
	/// @return A reference to the current object
	/// @see operator*(const BigInteger& lhs, const BigIntegers& rhs)
	/// for additional information
	BigInteger& operator*=(const BigInteger& rhs);

	/// Divides the current object with rhs
	/// @return A reference to the current object
	/// @see operator/(const BigInteger& lhs, const BigIntegers& rhs)
	/// for additional information
	BigInteger& operator/=(const BigInteger& rhs);

	/// Sets the reminder to the object when dividing the object with rhs
	/// @return A reference to the current object
	/// @see operator%(const BigInteger& lhs, const BigIntegers& rhs)
	/// for additional information
	BigInteger& operator%=(const BigInteger& rhs);

private:
	/// Stores the numerators(base 100) of an object
	std::deque<RadixSize> m_Numbers;

	/// The base of the numerators
	static const RadixSize BASE_NUM = 100;

	/// The length of the base
	static const uint8_t BASE_LENGTH = 3;

	/// Used to divide two BigIntegers together
	/// @returns It doesn't return anything but 
	/// you can get the quotient or the reminder
	/// by passing a BigInteger to the quotient or
	/// the reminder by reference
	/// @throws std::runtime_error if rhs is equal to zero
	static void divide(BigInteger lhs,
		BigInteger rhs,
		BigInteger& quotient,
		BigInteger& reminder);

	/// Gets a reference to a numerator with the selected index
	/// @returns A reference to the the numerator with the selected index
	RadixSize& operator[](size_t index) { return m_Numbers[index]; };

	/// Gets a copy to a numerator with the selected index
	/// @returns A copy to the the numerator with the selected index
	RadixSize  operator[](size_t index) const { return m_Numbers[index]; };

	/// @returns If c is a digit
	bool isDigit(char c) const;

	/// Converts the internal BigInteger representation into
	/// a string so it can be displayed properly
	/// @returns The object as a string
	std::string convertToString() const;

	/// Converts a string number into a numerator.
	/// It is mainly used for converting from string
	/// into an internal representation of BigInteger
	/// @return A numerator that is converted into a number from numStr 
	RadixSize toNumber(const char* numStr) const;

	/// Allows you to resize the container
	/// @param initalSize The size that the container will resize to
	/// @attention It sets all of the available size to a zero
	BigInteger setSize(size_t initalSize);

	// -------- DEQUE SPECIFIC METHODS --------
	
	///@returns The amount of numerators
	size_t size() const { return m_Numbers.size(); }

	/// Adds a value to the front of the container
	/// @param value The value to add
	void pushFront(RadixSize value) { m_Numbers.push_front(value); }
	/// Adds a value to the back of the container
	/// @param value The value to add
	void pushBack(RadixSize value) { m_Numbers.push_back(value); }

	/// Pops the element from the front of the container
	void popFront() { m_Numbers.pop_front(); }
	/// Pops the element from the back of the container
	void popBack() { m_Numbers.pop_back(); }

	/// @returns the first element in the container
	const RadixSize& front() const { return m_Numbers.front(); }
	/// @returns the last element in the container
	const RadixSize& back() const { return m_Numbers.back(); }

	/// @returns An iterator to the beginning of the container
	std::deque<RadixSize>::iterator begin() { return m_Numbers.begin(); }
	/// @returns An iterator to the end of the container
	std::deque<RadixSize>::iterator end() { return m_Numbers.end(); }

	/// Inserts elements at the specified location in the container
	/// @param pos Iterator before which the content will be inserted
	/// @param begin A beginning iterator of the container to insert
	/// @param end An ending iterator of the container to insert
	void insert(std::deque<RadixSize>::iterator pos,
		std::deque<RadixSize>::iterator begin,
		std::deque<RadixSize>::iterator end)
	{
		m_Numbers.insert(pos, begin, end);
	}
};

/// Takes a string as an input from the stream and converts it into a BigInteger
std::istream& operator>>(std::istream& input, BigInteger& bigInt);

/// Takes a BigInteger, converts it into a string and then prints it on the stream
std::ostream& operator<<(std::ostream& output, const BigInteger& bigInt);

#endif // !BIG_INTEGER_H