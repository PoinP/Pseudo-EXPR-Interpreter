#ifndef BIG_INTEGER_H

#define BIG_INTEGER_H

#include <iostream>
#include <deque>
#include <string>

class BigInteger
{
public:
	using RadixSize = uint16_t;

public:
	BigInteger();
	BigInteger(int32_t number); // Used for implicitly converting zero
	BigInteger(uint64_t number);
	BigInteger(const char* strNumber);
	BigInteger(const std::string& strNumber);

	// -------- OPERATOR OVERLOADING --------
	operator std::string() const { return convertToString(); }
	explicit operator bool() const { return size() > 1 || m_Numbers[0] != 0; } // Add tests

	friend bool operator==(const BigInteger& lhs, const BigInteger& rhs);
	friend bool operator!=(const BigInteger& lhs, const BigInteger& rhs);
	friend bool operator>(const BigInteger& lhs, const BigInteger& rhs);
	friend bool operator>=(const BigInteger& lhs, const BigInteger& rhs);
	friend bool operator<(const BigInteger& lhs, const BigInteger& rhs);
	friend bool operator<=(const BigInteger& lhs, const BigInteger& rhs);

	friend BigInteger operator+(const BigInteger& lhs, const BigInteger& rhs);
	friend BigInteger operator-(const BigInteger& lhs, const BigInteger& rhs);
	friend BigInteger operator*(const BigInteger& lhs, const BigInteger& rhs);
	friend BigInteger operator/(const BigInteger& lhs, const BigInteger& rhs);
	friend BigInteger operator%(const BigInteger& lhs, const BigInteger& rhs);

	BigInteger& operator++();
	BigInteger operator++(int);

	BigInteger& operator--();
	BigInteger operator--(int);

	BigInteger& operator+=(const BigInteger& rhs);
	BigInteger& operator-=(const BigInteger& rhs);
	BigInteger& operator*=(const BigInteger& rhs);
	BigInteger& operator/=(const BigInteger& rhs);
	BigInteger& operator%=(const BigInteger& rhs);

private:
	std::deque<RadixSize> m_Numbers;

	static const RadixSize BASE_NUM = 100;
	static const uint8_t BASE_LENGTH = 3;

	static void divide(BigInteger lhs,
		BigInteger rhs,
		BigInteger& quotient,
		BigInteger& reminder);


	RadixSize& operator[](size_t index) { return m_Numbers[index]; };
	RadixSize  operator[](size_t index) const { return m_Numbers[index]; };

	bool isDigit(char c) const;
	std::string convertToString() const;
	RadixSize toNumber(const char* numStr) const;

	BigInteger setSize(size_t initalSize);

	// -------- DEQUE SPECIFIC METHODS --------
	size_t size() const { return m_Numbers.size(); }

	void pushFront(RadixSize value) { m_Numbers.push_front(value); }
	void pushBack(RadixSize value) { m_Numbers.push_back(value); }

	void popFront() { m_Numbers.pop_front(); }
	void popBack() { m_Numbers.pop_back(); }

	const RadixSize& front() const { return m_Numbers.front(); }
	const RadixSize& back() const { return m_Numbers.back(); }

	std::deque<RadixSize>::iterator begin() { return m_Numbers.begin(); }
	std::deque<RadixSize>::iterator end() { return m_Numbers.end(); }

	void insert(std::deque<RadixSize>::iterator pos,
		std::deque<RadixSize>::iterator begin,
		std::deque<RadixSize>::iterator end)
	{
		m_Numbers.insert(pos, begin, end);
	}
};

std::istream& operator>>(std::istream& input, BigInteger& bigInt);
std::ostream& operator<<(std::ostream& input, const BigInteger& bigInt);

#endif // !BIG_INTEGER_H