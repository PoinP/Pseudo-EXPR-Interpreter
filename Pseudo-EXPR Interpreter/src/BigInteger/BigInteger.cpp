#include "BigInteger.h"
#include <stdexcept>

using RadixSize = BigInteger::RadixSize;

BigInteger::BigInteger()
{
	m_Numbers.push_back(0);
}

BigInteger::BigInteger(int32_t number)
{
	if (number < 0)
		throw std::invalid_argument("Negative values are not supported!");

	*this = (uint64_t)number;
}

BigInteger::BigInteger(uint64_t number)
{
	if (number == 0)
		m_Numbers.push_back(0);

	while (number > 0)
	{
		m_Numbers.push_back(number % BASE_NUM);
		number /= BASE_NUM;
	}
}

BigInteger::BigInteger(const char* strNumber)
{
	uint8_t lengthCounter = BASE_LENGTH;
	char* rawDigits = new char[lengthCounter];
	rawDigits[--lengthCounter] = '\0';

	size_t strLength = strlen(strNumber);

	for (int64_t i = strLength - 1; i >= 0; --i)
	{
		if (!isDigit(strNumber[i]))
			throw std::invalid_argument("The string number is invalid!");

		rawDigits[--lengthCounter] = strNumber[i];

		if (i == 0)
		{
			m_Numbers.push_back(toNumber(rawDigits + lengthCounter));
			break;
		}

		if (lengthCounter <= 0)
		{
			lengthCounter = BASE_LENGTH - 1;
			m_Numbers.push_back(toNumber(rawDigits));
		}
	}

	delete[] rawDigits;
}

BigInteger::BigInteger(const std::string& strNumber)
	: BigInteger(strNumber.c_str())
{
}

bool operator==(const BigInteger& lhs, const BigInteger& rhs)
{
	if (lhs.size() != rhs.size())
		return false;

	for (size_t i = 0; i < lhs.size(); i++)
	{
		if (lhs[i] != rhs[i])
			return false;
	}

	return true;
}

bool operator!=(const BigInteger& lhs, const BigInteger& rhs)
{
	return !(lhs == rhs);
}

bool operator>(const BigInteger& lhs, const BigInteger& rhs)
{
	if (lhs.size() > rhs.size())
		return true;

	if (lhs.size() < rhs.size())
		return false;

	for (int64_t i = lhs.size() - 1; i >= 0; --i)
	{
		if (lhs[i] != rhs[i])
			return lhs[i] > rhs[i];
	}

	return false;
}

bool operator>=(const BigInteger& lhs, const BigInteger& rhs)
{
	return !(lhs < rhs);
}

bool operator<(const BigInteger& lhs, const BigInteger& rhs)
{
	return !(lhs > rhs) && lhs != rhs;
}

bool operator<=(const BigInteger& lhs, const BigInteger& rhs)
{
	return !(lhs > rhs);
}

BigInteger operator+(const BigInteger& lhs, const BigInteger& rhs)
{
	const BigInteger* lhsPtr = &lhs;
	const BigInteger* rhsPtr = &rhs;

	if (lhs > rhs)
		std::swap(lhsPtr, rhsPtr);

	BigInteger result = *rhsPtr;

	size_t i;
	for (i = 0; i < lhsPtr->size(); ++i)
	{
		result[i] += lhsPtr->operator[](i);

		if (result[i] >= BigInteger::BASE_NUM)
		{
			size_t j = i;
			// Compensate for overflowing radix
			while (result[j] >= BigInteger::BASE_NUM)
			{
				result[j] -= BigInteger::BASE_NUM;

				if (j + 1 < result.size())
					result[j + 1] += 1;
				else
					result.pushBack(1);

				j++;
			}

		}
	}

	return result;
}

BigInteger operator-(const BigInteger& lhs, const BigInteger& rhs)
{
	if (lhs < rhs)
		throw std::underflow_error("Negative numbers are not supported!");

	BigInteger result = lhs;

	for (size_t i = 0; i < rhs.size(); i++)
	{
		if (result[i] < rhs[i])
		{
			result[i] += BigInteger::BASE_NUM - rhs[i];

			size_t j = i + 1;
			while (result[j] == 0)
			{
				result[j++] = BigInteger::BASE_NUM - 1;
			}

			result[j] -= 1;

			continue;
		}

		result[i] -= rhs[i];
	}

	while (result.size() > 1 && result.back() == 0)
		result.popBack();

	return result;
}

BigInteger operator*(const BigInteger& lhs, const BigInteger& rhs)
{
	if (lhs.size() == 1)
	{
		if (lhs == "0") return BigInteger();
		if (lhs == 1) return BigInteger(rhs);
	}

	if (rhs.size() == 1)
	{
		if (rhs == "0") return BigInteger();
		if (rhs == 1) return BigInteger(lhs);
	}

	const BigInteger* lhsPtr = &lhs;
	const BigInteger* rhsPtr = &rhs;

	if (lhs > rhs)
		std::swap(lhsPtr, rhsPtr);

	BigInteger* products = new BigInteger[lhsPtr->size()];

	// j + i -> i is the offset that shows when mutiplying numbers

	for (size_t i = 0; i < lhsPtr->size(); i++)
	{
		products[i] = BigInteger().setSize(rhsPtr->size() + i);

		for (size_t j = 0; j < rhsPtr->size(); j++)
		{
			products[i][j + i] += lhsPtr->operator[](i) * rhsPtr->operator[](j);

			if (products[i][j + i] >= BigInteger::BASE_NUM)
			{
				RadixSize carry = products[i][j + i] / BigInteger::BASE_NUM;
				products[i][j + i] %= BigInteger::BASE_NUM;

				if (j + i + 1 < products[i].size())
					products[i][j + i + 1] += carry;
				else
					products[i].pushBack(carry);
			}
		}
	}

	BigInteger result;

	for (size_t i = 0; i < lhsPtr->size(); i++)
	{
		// most likely a bug in vs intelisense bullshit
		result = result + products[i];
	}

	delete[] products;
	return result;
}

BigInteger operator/(const BigInteger& lhs, const BigInteger& rhs)
{
	BigInteger quotient;
	BigInteger reminder;
	BigInteger::divide(lhs, rhs, quotient, reminder);

	return quotient;
}

BigInteger operator%(const BigInteger& lhs, const BigInteger& rhs)
{
	BigInteger quotient;
	BigInteger reminder;
	BigInteger::divide(lhs, rhs, quotient, reminder);

	return reminder;
}

BigInteger& BigInteger::operator++()
{
	*this = *this + 1;
	return *this;
}

BigInteger BigInteger::operator++(int)
{
	BigInteger oldValue = *this;
	++(*this);
	return oldValue;
}

BigInteger& BigInteger::operator--()
{
	*this = *this - 1;
	return *this;
}

BigInteger BigInteger::operator--(int)
{
	BigInteger oldValue = *this;
	--(*this);
	return oldValue;
}

BigInteger& BigInteger::operator+=(const BigInteger& rhs)
{
	*this = *this + rhs;
	return *this;
}

BigInteger& BigInteger::operator-=(const BigInteger& rhs)
{
	*this = *this - rhs;
	return *this;
}

BigInteger& BigInteger::operator*=(const BigInteger& rhs)
{
	*this = *this * rhs;
	return *this;
}

BigInteger& BigInteger::operator/=(const BigInteger& rhs)
{
	*this = *this / rhs;
	return *this;
}

BigInteger& BigInteger::operator%=(const BigInteger& rhs)
{
	*this = *this % rhs;
	return *this;
}

void BigInteger::divide(BigInteger lhs, BigInteger rhs, BigInteger& quotient, BigInteger& reminder)
{
	if (rhs == "0")
		throw std::runtime_error("Division by zero!");

	if (lhs == "0")
	{
		quotient = "0";
		reminder = "0";
		return;
	}

	if (lhs < rhs)
	{
		quotient = "0";
		reminder = lhs;
		return;
	}

	if (lhs == rhs)
	{
		quotient = 1;
		reminder = "0";
		return;
	}

	if (lhs.size() == 1 && rhs.size() == 1)
	{
		quotient = lhs[0] / rhs[0];
		reminder = lhs[0] % rhs[0];
		return;
	}

	quotient.popBack();
	reminder.popBack();

	reminder.insert(reminder.begin(), lhs.end() - rhs.size(), lhs.end());

	int64_t lhsIndex = lhs.size() - rhs.size() - 1;
	while (reminder < rhs)
	{
		reminder.pushFront(lhs[lhsIndex--]);
	}

	while (lhsIndex >= -1)
	{
		int subtractions = 0;
		while (reminder >= rhs)
		{
			reminder -= rhs;
			subtractions++;
		}

		if (subtractions != 0)
		{
			quotient.pushFront(subtractions);
			subtractions = 0;
		}

		if (lhsIndex == -1)
			break;

		while (reminder < rhs && lhsIndex >= 0)
		{
			reminder.pushFront(lhs[lhsIndex--]);

			if (reminder.size() > 1 && reminder.back() == 0)
				reminder.popBack();

			if (reminder < rhs)
				quotient.pushFront(subtractions);
		}
	}
}

bool BigInteger::isDigit(char c) const
{
	return c >= '0' && c <= '9';
}

uint8_t BigInteger::toDigit(char c) const
{
	return c - '0';
}

std::string BigInteger::convertToString() const
{
	std::string bigInt;
	std::string currNum;
	size_t baseCounter;

	bigInt = std::to_string(m_Numbers.back());

	for (int64_t i = m_Numbers.size() - 2; i >= 0; --i)
	{
		baseCounter = BASE_LENGTH - 1;
		currNum = std::to_string(m_Numbers[i]);
		baseCounter -= currNum.length();

		for (int j = 0; j < baseCounter; ++j)
		{
			bigInt.push_back('0');
		}

		bigInt += currNum;
	}

	return bigInt;
}

RadixSize BigInteger::toNumber(const char* numStr) const
{
	RadixSize number = 0;

	while (*numStr != '\0')
	{
		number = number * 10 + *numStr - '0';
		numStr++;
	}

	return number;
}

BigInteger BigInteger::setSize(size_t initalSize)
{
	m_Numbers = std::deque<RadixSize>(initalSize);
	return *this;
}

std::istream& operator>>(std::istream& input, BigInteger& bigInt)
{
	std::string number;
	input >> number;
	bigInt = number;
	return input;
}

std::ostream& operator<<(std::ostream& input, const BigInteger& bigInt)
{
	input << static_cast<std::string>(bigInt);
	return input;
}