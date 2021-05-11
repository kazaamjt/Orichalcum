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

} // namespace LibOrichalcum
