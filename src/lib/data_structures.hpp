#pragma once
#include <iostream>
#include <map>
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
	BigInt::BigInt(const BigInt &b);
	BigInt(std::string);

	// binary math operators
	BigInt operator+(BigInt const &) const;
	BigInt &operator+=(BigInt const &);
	BigInt operator-(BigInt const &) const;
	BigInt &operator-=(BigInt const &);
	BigInt operator*(BigInt const &);
	BigInt &operator*=(BigInt const &);

	// Comparison operators
	bool operator<(const BigInt &) const;
	bool operator>(const BigInt &) const;
	bool operator<=(const BigInt &) const;
	bool operator>=(const BigInt &) const;
	bool operator==(const BigInt &) const;
	bool operator!=(const BigInt &) const;

	// power
	BigInt &pow(BigInt const &);

	// unary neg
	BigInt operator-() const;

	// helpers
	std::string to_string();

private:
	std::vector<int> number;
	bool positive;
	int base;
	unsigned int skip;
	static const int default_base=1000000000;
};

} // namespace LibOrichalcum
