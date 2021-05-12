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


BigInt::BigInt(std::string _string):
base(BigInt::default_base),
skip(0) {
	size_t size = _string.length();
	positive = (_string[0] != '-');

	size_t length = 0;
	int num = 0;
	int prefix = 1;
	while (true) {
		if (size <= 0) break;
		if (!positive && size <= 1) break;

		for (size_t i(size - 1); i >= size - 9; i--) {
			if (!isdigit(_string[i])) break;
			num += (_string[i] - '0') * prefix;
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

	if (number.size() > 1) {
		do {
			it1 = number.end() - 1;
			if (*it1 == 0) number.pop_back();
			else break;
		} while (number.size() > 1);
	}

	return *this;
}


std::ostream &operator<<(std::ostream &out, BigInt const &a) {
	if (!a.number.size()) return out << 0;
	int i = a.number.size() - 1;
	for (; i>=0 && a.number[i] == 0; --i);

	if (i == -1) return out << 0;
	if (!a.positive) out << '-';

	std::vector<int>::const_reverse_iterator it = a.number.rbegin() + (a.number.size() - i - 1);

	out << *it++;
	for (; it != a.number.rend(); ++it) {
		for (int i(0), len = a.segment_length(*it); i < 9 - len; ++i) out << '0';
		if (*it) out << *it;
	}

	return out;
}

int BigInt::segment_length(int segment) const {
	int length = 0;
	while (segment) {
		segment /= 10;
		length++;
	}

	return length;
}

std::string to_string(const BigInt &value) {
	std::ostringstream stream;
	stream << value;

	return stream.str();
}

} // namespace LibOrichalcum
