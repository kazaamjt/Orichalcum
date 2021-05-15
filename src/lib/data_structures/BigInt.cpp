#include "BigInt.hpp"

namespace LibOrichalcum {
BigInt::BigInt():
negative(false) { }

BigInt::BigInt(const BigInt &b):
number(b.number),
negative(b.negative) { }

BigInt::BigInt(std::string _string) {
	size_t start_pos = 0;
	if (_string[0] == '-') {
		negative = true;
		start_pos = 1;
	}
	size_t digits = _string.size();
	std::string substr;
	while (digits > segment_length) {
		substr = _string.substr(digits - segment_length, segment_length);
		number.push_back(std::stoi(substr));
		digits -= segment_length;
	}

	substr = _string.substr(start_pos, digits - start_pos);
	number.push_back(std::stoi(substr));
}

BigInt BigInt::operator-() const {
	BigInt b = *this;
	b.negative = !negative;

	return b;
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

std::string to_string(const BigInt &big_int) {
	return big_int.str();
}

} // namespace LibOrichalcum
