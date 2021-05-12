#pragma once
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "Chunk.hpp"

namespace LibOrichalcum {

struct ConstStack {
	void push(const Constant &value) {
		internal.push_back(value);
	}

	Constant pop() {
		Constant value = internal.back();
		internal.pop_back();
		return value;
	}

	std::vector<Constant> internal;
};

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

	// helpers
	int digits();
	int trailing_zeros();
	friend std::ostream &operator<<(std::ostream &out, BigInt const &a);

private:
	std::vector<int> number;
	bool positive;
	int base;
	unsigned int skip;
	static const int default_base=1000000000;

	int segment_length(int segment) const;
};

std::string to_string(const BigInt &value);

} // namespace LibOrichalcum
