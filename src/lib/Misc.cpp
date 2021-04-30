#include "Misc.hpp"

#include <sstream>
#include <iomanip>

namespace LibOrichalcum::Misc {

std::string to_hex(size_t number, int width) {
	std::stringstream stream;
	stream << "0x" << std::setfill('0') << std::setw(width) << number;
	return stream.str();
}

std::string remove_white_space(const std::string &string) {
	std::string new_string;
	bool indent = true;
	for (const char e: string) {
		if (e != '\t' && e != ' ') {
			new_string += e;
			indent = false;
		}
		else if (!indent) {
			new_string += e;
		}
	}
	return new_string;
}

int count_whitespace(const std::string &string) {
	int count = 0;
	for (const char c : string) {
		if (c == '\t' || c == ' ') {
			count++;
		}
		else {
			break;
		}
	}
	return count;
}

} // LibOrichalcum
