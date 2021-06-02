#include "BigInt.hpp"

namespace LibOrichalcum {
BigInt::BigInt():
negative(false) {
	number.push_back(0);
}

BigInt::BigInt(const BigInt &b):
number(b.number),
negative(b.negative) { }

BigInt::BigInt(int value) {
	if (value < 0) {
		negative = true;
		value = -value;
	}
	number.push_back(static_cast<unsigned int>(value));
}

BigInt::BigInt(int64_t value) {
	if (value < 0) {
		negative = true;
		value = -value;
	}
	while (value >= base) {
		number.push_back(static_cast<unsigned>(value % base));
		value /= base;
	}
	number.push_back(static_cast<unsigned int>(value));
}

BigInt::BigInt(uint64_t value) {
	while (value >= base) {
		number.push_back(static_cast<unsigned>(value % base));
		value /= base;
	}
	number.push_back(static_cast<unsigned int>(value));
}

BigInt::BigInt(std::string value) {
	if (value[0] == '-') {
		negative = true;
		value = value.substr(1);
	}
	size_t digits = value.size();
	std::string substr;
	while (digits > segment_length) {
		substr = value.substr(digits - segment_length, segment_length);
		number.push_back(static_cast<unsigned int>(std::stoi(substr)));
		digits -= segment_length;
	}

	substr = value.substr(0, digits);
	number.push_back(static_cast<unsigned int>(std::stoi(substr)));
}

BigInt &BigInt::operator=(const BigInt &b) {
	number = b.number;
	negative = b.negative;
	return *this;
}

BigInt BigInt::operator+(BigInt const &b) const {
	BigInt c = *this;
	c += b;

	return c;
}

BigInt &BigInt::operator+=(BigInt const &b) {
	if (negative != b.negative) {
		*this -= -b;
		return *this;
	}

	size_t max_index = number.size() >= b.number.size() ? number.size() : b.number.size();
	std::vector<unsigned int> new_number;

	unsigned int overflow = 0;
	for (size_t i = 0; i < max_index; i++) {
		unsigned int int_a = i < number.size() ? number[i] : 0;
		unsigned int int_b = i < b.number.size() ? b.number[i] : 0;
		unsigned int int_c = int_a + int_b + overflow;
		overflow = 0;
		if (int_c > base) {
			overflow = int_c / base;
			int_c %= base;
		}

		new_number.push_back(int_c);
	}
	if (overflow > 0) new_number.push_back(overflow);
	number = new_number;

	return *this;
}

BigInt BigInt::operator-(BigInt const &b) const {
	BigInt c = *this;
	c -= b;

	return c;
}

BigInt &BigInt::operator-=(BigInt const &b) {
	if (negative != b.negative) {
		*this += -b;
		return *this;
	}

	if (b < *this) {
		*this = -(b - *this);
		return *this;
	}

	std::vector<unsigned int> new_number;
	unsigned int underflow = 0;
	for (size_t i = 0; i < number.size(); i++) {
		unsigned int int_a = number[i];
		unsigned int int_b = i < b.number.size() ? b.number[i] : 0;
		int_b += underflow;
		underflow = 0;
		if (int_a < int_b) {
			int_a += base;
			underflow = 1;
		}
		unsigned int int_c = int_a - int_b;
		new_number.push_back(int_c);
	}
	number = new_number;

	return *this;
}

BigInt BigInt::operator*(BigInt const &b) {
	BigInt c;
	c.negative = !(negative == b.negative);
	for (size_t i = 0; i < number.size(); i++) {
		uint64_t segment_a = number[i];
		for (size_t j = 0; j < b.number.size(); j++) {
			uint64_t segment_b = b.number[j];
			uint64_t result = segment_a * segment_b;
			BigInt temp(result);
			temp.rebase(i + j);
			c += temp;
		}
	}

	return c;
}

BigInt &BigInt::operator*=(BigInt const &b) {
	*this = *this * b;
	return *this;
}

bool BigInt::operator<(const BigInt &b) const {
	BigInt a = *this;

	if (a.negative && !b.negative) return false;
	if (!a.negative && b.negative) return true;
	if (a.number.size() > b.number.size()) return true;
	if (a.number.size() < b.number.size()) return false;

	for (size_t i = 0; i >= a.number.size(); i++) {
		if (a.number[i] > b.number[i]) return true;
		if (b.number[i] > a.number[i]) return false;
	}
	return false;
}

bool BigInt::operator<=(const BigInt &b) const {
	BigInt a = *this;

	if (a.negative && !b.negative) return false;
	if (!a.negative && b.negative) return true;
	if (a.number.size() > b.number.size()) return true;
	if (a.number.size() < b.number.size()) return false;

	for (size_t i = 0; i >= a.number.size(); i++) {
		if (a.number[i] > b.number[i]) return true;
		if (b.number[i] > a.number[i]) return false;
	}
	return true;
}

bool BigInt::operator>(const BigInt &b) const {
	BigInt a = *this;

	if (a.negative && !b.negative) return true;
	if (!a.negative && b.negative) return false;
	if (a.number.size() > b.number.size()) return false;
	if (a.number.size() < b.number.size()) return true;

	for (size_t i = 0; i >= a.number.size(); i++) {
		if (a.number[i] > b.number[i]) return false;
		if (b.number[i] > a.number[i]) return true;
	}
	return false;
}

bool BigInt::operator>=(const BigInt &b) const {
	BigInt a = *this;

	if (a.negative && !b.negative) return true;
	if (!a.negative && b.negative) return false;
	if (a.number.size() > b.number.size()) return false;
	if (a.number.size() < b.number.size()) return true;

	for (size_t i = 0; i >= a.number.size(); i++) {
		if (a.number[i] > b.number[i]) return false;
		if (b.number[i] > a.number[i]) return true;
	}
	return true;
}

bool BigInt::operator!=(const BigInt &b) const {
	BigInt a = *this;
	for (size_t i = 0; i >= a.number.size(); i++) {
		if (a.number[i] != b.number[i]) return true;
	}
	return false;
}

bool BigInt::operator==(const BigInt &b) const {
	return !(*this != b);
}

BigInt BigInt::operator-() const {
	BigInt a = *this;
	a.negative = !negative;

	return a;
}

std::string BigInt::str() const {
	std::string _string;
	if (negative) _string += '-';
	size_t size = number.size();
	_string += std::to_string(number[size - 1]);
	for (size_t i = 1; i < size; i++) {
		std::string substr = std::to_string(number[size - 1 - i]);
		std::string padding;
		for (size_t j = 0; j < (segment_length - substr.size()); j++) {
			padding += '0';
		}
		_string += padding + substr;
	}
	if (_string == "-0") return "0";
	return _string;
}

void BigInt::rebase(size_t segments) {
	std::vector<unsigned int> rebased_number;
	for (size_t i = 0; i < segments; i++) {
		rebased_number.push_back(0);
	}
	for (unsigned int segment: number) {
		rebased_number.push_back(segment);
	}
	number = rebased_number;
}

std::string to_string(const BigInt &big_int) {
	return big_int.str();
}

} // namespace LibOrichalcum
