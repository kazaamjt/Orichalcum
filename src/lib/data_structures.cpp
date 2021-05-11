#include "data_structures.hpp"

namespace LibOrichalcum {
BigInt::BigInt():
positive(true),
base(BigInt::default_base),
skip(0) { }

BigInt::BigInt(const BigInt &b):
number(b.number),
positive(b.positive),
base(b.base),
skip(b.skip) { }


BigInt::BigInt(std::string int_string):
base(BigInt::default_base),
skip(0) {
	size_t size = int_string.length();
	positive = (int_string[0] != '-');

	while (true) {
		if (size <= 0) break;
		if (!positive && size <= 1) break;

		size_t length = 0;
		int num = 0;
		int prefix = 1;
		for (size_t i(size - 1); i >= size - 9; i--) {
			num += (int_string[i] - '0') * prefix;
			prefix *= 10;
			length++;
		}
		number.push_back(num);
		size -= length;
	}
}

BigInt BigInt::operator+(BigInt const &b) const {
	BigInt c = *this;
	c += b;

	return c;
}

BigInt &BigInt::operator+=(BigInt const &b) {
	if (!b.positive) {
		return *this -= b;
	}

	std::vector<int>::iterator it1 = number.begin();
	std::vector<int>::const_iterator it2 = b.number.begin();

	int sum = 0;
	while (it1 != number.end() || it2 != b.number.end()) {
		if (it1 != number.end()) {
			sum += *it1;
		} else {
			number.push_back(0);
			it1 = number.end()-1;
		}
		if (it2 != b.number.end()) {
			sum += *it2;
			++it2;
		}
		*it1 = sum % base;
		++it1;
		sum /= base;
	}

	if (sum) number.push_back(1);

	return *this;
}

BigInt BigInt::operator-(BigInt const &b) const {
	BigInt c = *this;
	c -= b;

	return c;
}

BigInt &BigInt::operator-=(BigInt const &b) {
	std::vector<int>::iterator it1 = number.begin();
	std::vector<int>::const_iterator it2 = b.number.begin();

	int dif = 0;
	while (it1 != number.end() || it2 != b.number.end()) {
		if (it1 != number.end()) {
			dif += *it1;
			++it1;
		}
		if (it2 != b.number.end()) {
			dif -= *it2;
			++it2;
		}
		if (dif < 0) {
			*(it1 - 1) = dif + base;
			dif = -1;
		} else {
			*(it1 - 1) = dif % base;
			dif /= base;
		}
	}
	if (dif < 0) positive = false;

	if (number.size() > 1)
	{
		do
		{
			it1 = number.end() - 1;
			if (*it1 == 0) number.pop_back();
			else break;
		} while (number.size() > 1);
	}

	return *this;
}

} // namespace LibOrichalcum
