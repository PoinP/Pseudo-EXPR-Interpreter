#ifndef BIG_INTEGER

#define BIG_INTEGER

#include <iostream>
#include <vector>
#include <string>

class BigInteger
{
public:
	BigInteger();
	BigInteger(const char* number);
	BigInteger(std::string number);
	BigInteger(unsigned long long number);

	// -------- OPERATOR OVERLOADING --------
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
	const static unsigned long long MAX_VALUE = 9'999'999'999'999'999'999;
	std::vector<unsigned long long> numbers;
};

std::istream& operator>>(std::istream& input, BigInteger& bigInt);
std::istream& operator<<(std::ostream& input, const BigInteger& bigInt);

#endif // !BIG_INTEGER