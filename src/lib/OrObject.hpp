#pragma once
#include <cstdint>
#include <string>

#include "Token.hpp"

namespace LibOrichalcum {

class OrNone {};


enum class OrValueType {
	INT,
	FLOAT,
	BOOL,
	NONE,
};

std::string to_string(OrValueType type);


class OrValue {
public:
	OrValue();
	OrValue(int64_t value);
	OrValue(int64_t value, std::shared_ptr<Token> token);
	OrValue(double value);
	OrValue(double value, std::shared_ptr<Token> token);
	OrValue(bool value);
	OrValue(bool value, std::shared_ptr<Token> token);
	OrValue(OrNone value);
	OrValue(OrNone value, std::shared_ptr<Token> token);
	OrValueType type;
	union {
		int64_t INT;
		double FLOAT;
		bool BOOL;
		OrNone NONE;
	} value;
	std::shared_ptr<Token> token;
	bool has_token;
};

} // LibOrichalcum
