#pragma once
#include <cstdint>

namespace LibOrichalcum {

class OrNone {};


enum class OrValueType {
	INT,
	FLOAT,
	BOOL,
	NONE,
};


struct OrValue {
	OrValueType type;
	union {
		int64_t INT;
		double FLOAT;
		bool BOOL;
		char CHAR;
		OrNone NONE;
	} value;
};

} // LibOrichalcum
