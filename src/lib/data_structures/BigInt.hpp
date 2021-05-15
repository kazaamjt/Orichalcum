#pragma once
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace LibOrichalcum {

class BigInt {
public:
	BigInt();
	BigInt(const BigInt &b);
	BigInt(std::string _string);

	// binary math operators
	BigInt operator+(BigInt const &b) const;
	BigInt &operator+=(BigInt const &b);
	BigInt operator-(BigInt const &b) const;
	BigInt &operator-=(BigInt const &b);
	BigInt operator*(BigInt const &b);
	BigInt &operator*=(BigInt const &b);
	BigInt operator/(BigInt const &b);
	BigInt &operator/=(BigInt const &b);

	// Comparison operators
	bool operator<(const BigInt &b) const;
	bool operator>(const BigInt &b) const;
	bool operator<=(const BigInt &b) const;
	bool operator>=(const BigInt &b) const;
	bool operator==(const BigInt &b) const;
	bool operator!=(const BigInt &b) const;

	// power
	BigInt &pow(BigInt const &b);

	// unary neg
	BigInt operator-() const;

	// Helpers
	std::string str() const;

private:
	std::vector<int> number;
	bool negative;

	const size_t segment_length = 9;
};

std::string to_string(const BigInt &big_int);

} // namespace LibOrichalcum

