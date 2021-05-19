#pragma once
#include <cstdint>
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
	BigInt(int value);
	BigInt(int64_t value);
	BigInt(uint64_t value);
	BigInt(std::string value);

	BigInt &operator=(const BigInt &b);

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
	std::vector<unsigned int> number;
	bool negative;

	const size_t segment_length = 9;
	const unsigned int base = 1000000000;

	void rebase(size_t zeros);
};

std::string to_string(const BigInt &big_int);

} // namespace LibOrichalcum

