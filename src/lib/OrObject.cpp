#include "OrObject.hpp"

namespace LibOrichalcum {

std::string to_string(OrValueType type) {
	switch (type) {
		case OrValueType::INT: return "int";
		case OrValueType::FLOAT: return "float";
		case OrValueType::BOOL: return "bool";
		case OrValueType::NONE: return "None";
	}
}

OrValue::OrValue() { }

OrValue::OrValue(int64_t _value):
type(OrValueType::INT), has_token(false) {
	value.INT = _value;
}

OrValue::OrValue(int64_t _value, std::shared_ptr<Token> _token):
type(OrValueType::INT), token(_token), has_token(true) {
	value.INT = _value;
}

OrValue::OrValue(double _value):
type(OrValueType::FLOAT), has_token(false) {
	value.FLOAT = _value;
}

OrValue::OrValue(double _value, std::shared_ptr<Token> _token):
type(OrValueType::FLOAT), token(_token), has_token(true) {
	value.FLOAT = _value;
}

OrValue::OrValue(bool _value):
type(OrValueType::BOOL), has_token(false) {
	value.BOOL = _value;
}

OrValue::OrValue(bool _value, std::shared_ptr<Token> _token):
type(OrValueType::BOOL), token(_token), has_token(true) {
	value.BOOL = _value;
}

OrValue::OrValue(OrNone _value):
type(OrValueType::NONE), has_token(false) {
	value.NONE = _value;
}

OrValue::OrValue(OrNone _value, std::shared_ptr<Token> _token):
type(OrValueType::NONE), token(_token), has_token(true) {
	value.NONE = _value;
}

} // LibOrichalcum
