#include "VM.hpp"

#include <cmath>
#include <iostream>

#include "Debug.hpp"
#include "Log.hpp"

namespace LibOrichalcum {

std::string to_string(BINARY_OP op) {
	switch (op) {
		case BINARY_OP::ADD: return "ADD";
		case BINARY_OP::SUBTRACT: return "SUBTRACT";
		case BINARY_OP::MULTIPLY: return "MULTIPLY";
		case BINARY_OP::DIVIDE: return "DIVIDE";
		case BINARY_OP::INT_DIVIDE: return "INT_DIVIDE";
		case BINARY_OP::EXPONENTIATION: return "EXPONENTIATION";
	}
}

BINARY_OP to_opcode(Instruction instruction) {
	if (instruction.op_code == OP_CODE::ADD) return BINARY_OP::ADD;
	if (instruction.op_code == OP_CODE::SUBTRACT) return BINARY_OP::SUBTRACT;
	if (instruction.op_code == OP_CODE::MULTIPLY) return BINARY_OP::MULTIPLY;
	if (instruction.op_code == OP_CODE::DIVIDE) return BINARY_OP::DIVIDE;
	if (instruction.op_code == OP_CODE::INT_DIVIDE) return BINARY_OP::INT_DIVIDE;
	if (instruction.op_code == OP_CODE::EXPONENTIATION) return BINARY_OP::EXPONENTIATION;

	throw Error(
		COMPILE_RESULT::INTERNAL_ERROR,
		"Error occured in CPP::VM::to_opcode.",
		instruction.token
	);
}

bool truthiness(OrValue or_value) {
	if (or_value.type == OrValueType::BOOL)
		return or_value.value.BOOL;

	if (or_value.type == OrValueType::INT) {
		if (or_value.value.INT == 0)
			return false;
		return true;
	}

	if (or_value.type == OrValueType::FLOAT) {
		if (or_value.value.FLOAT > 0 && or_value.value.FLOAT < 0)
			return true;
		return false;
	}

	if (or_value.type == OrValueType::NONE)
		return false;

	return false;
}

VM::VM():
current(OP_CODE::RETURN) { }

VM::VM(bool _debug):
debug(_debug),
current(OP_CODE::RETURN) { }

InterpretReport VM::interpret(std::shared_ptr<Chunk> _chunk) {
	InterpretReport interpret_report;
	init_chunk(_chunk);

	interpret_report.result = run();

	return interpret_report;
}

void VM::init_chunk(std::shared_ptr<Chunk> _chunk) {
	chunk = _chunk;
	chunk->write(OP_CODE::RETURN);
}

INTERPRET_RESULT VM::run() {
	Log::debug("VM is executing chunk " + chunk->name);
	if (debug) {
		Debug::print_stack(stack);
		Debug::disassemble_chunk(chunk);
	}

	size_t i = 0;
	while(true) {
		current = chunk->get(i++);
		switch(current.op_code) {
			case OP_CODE::RETURN: {
				std::cout << "CHUNK RETURNED ";
				Debug::print_const(stack.pop());
				std::cout << std::endl;
				return INTERPRET_RESULT::OK;
			}

			case OP_CODE::CONST: {
				OrValue constant = chunk->get_const(current.index);
				stack.push(constant);
			} break;

			case OP_CODE::NEGATE: negate(); break;
			case OP_CODE::NOT: unary_not(); break;
			case OP_CODE::ADD:
			case OP_CODE::SUBTRACT:
			case OP_CODE::MULTIPLY:
			case OP_CODE::DIVIDE:
			case OP_CODE::INT_DIVIDE:
			case OP_CODE::EXPONENTIATION:
				binary_op(); break;
		}
	}
}

void VM::negate() {
	OrValue constant = stack.pop();
	switch (constant.type) {
		case OrValueType::INT: {
			constant.value.INT = -constant.value.INT;
		} break;

		case OrValueType::FLOAT: {
			constant.value.FLOAT = -constant.value.FLOAT;
		} break;
		case OrValueType::BOOL:
		case OrValueType::NONE: {
			throw Error(
				COMPILE_RESULT::RUNTIME_ERROR,
				"Unable to perform unary negate operation on type " + to_string(constant.type),
				constant.token
			);
		}
	}
	stack.push(constant);
}

void VM::unary_not() {
	bool value;
	OrValue constant = stack.pop();
	if (constant.type != OrValueType::BOOL)
		value = constant.value.BOOL;
	else
		value = truthiness(constant);

	OrValue new_constant(!value, current.token);
	stack.push(new_constant);
}

void VM::binary_op() {
	// b first, this is important
	OrValue const_b = stack.pop();
	OrValue const_a = stack.pop();
	OrValue const_c;
	BINARY_OP op = to_opcode(current.op_code);
	switch (op) {
		case BINARY_OP::ADD:
		case BINARY_OP::SUBTRACT:
		case BINARY_OP::MULTIPLY:
			const_c = calc_1(op, const_a, const_b); break;

		case BINARY_OP::DIVIDE:
		case BINARY_OP::INT_DIVIDE:
		case BINARY_OP::EXPONENTIATION:
			const_c = calc_2(op, const_a, const_b); break;
	}
	stack.push(const_c);
}

[[noreturn]] static void bin_op_type_error(BINARY_OP op, OrValue a, OrValue b) {
	throw Error(
		COMPILE_RESULT::RUNTIME_ERROR,
		"Unable to perform operation " + to_string(op) + " on types " +
		to_string(a.type) + " and " + to_string(b.type),
		a.token
	);
}

OrValue VM::calc_1(BINARY_OP op, OrValue a, OrValue b) {
	if (a.type == OrValueType::INT) {
		if (b.type == OrValueType::INT) {
			return OrValue(sub_calc_1(op, a.value.INT, b.value.INT));
		}
	}
	double val_b;
	if (a.type == OrValueType::FLOAT) {
		switch (b.type) {
			case OrValueType::INT:
				val_b = static_cast<double>(b.value.INT);
				break;
			case OrValueType::FLOAT:
				val_b = b.value.FLOAT;
				break;
			case OrValueType::BOOL:
			case OrValueType::NONE:
				bin_op_type_error(op, a, b);
		}
		return OrValue(sub_calc_2(op, a.value.FLOAT, val_b));
	}

	bin_op_type_error(op, a, b);
}

int64_t VM::sub_calc_1(BINARY_OP op, int64_t a, int64_t b) {
	if (op == BINARY_OP::ADD) return a + b;
	if (op == BINARY_OP::SUBTRACT) return a - b;
	if (op == BINARY_OP::MULTIPLY) return a * b;

	throw Error(
		COMPILE_RESULT::INTERNAL_ERROR,
		"Error occured in CPP::VM::sub_calc_1.",
		current.token
	);
}

OrValue VM::calc_2(BINARY_OP op, OrValue a, OrValue b) {
	double val_a, val_b;
	if (a.type == OrValueType::INT)
		val_a = static_cast<double>(a.value.INT);
	else val_a = a.value.FLOAT;

	if (b.type == OrValueType::INT)
		val_b = static_cast<double>(b.value.INT);
	else val_b = b.value.FLOAT;

	double val_c = sub_calc_2(op, val_a, val_b);

	if (op == BINARY_OP::INT_DIVIDE)
		return OrValue(static_cast<int64_t>(val_c));

	if (op == BINARY_OP::EXPONENTIATION)
		if (b.type == OrValueType::INT && b.value.INT >= 0)
			return OrValue(static_cast<int64_t>(val_c));

	return OrValue(val_c);
}

double VM::sub_calc_2(BINARY_OP op, double a, double b) {
	if (op == BINARY_OP::ADD) return a + b;
	if (op == BINARY_OP::SUBTRACT) return a - b;
	if (op == BINARY_OP::MULTIPLY) return a * b;
	if (op == BINARY_OP::DIVIDE || op == BINARY_OP::INT_DIVIDE)
		return a / b;
	if (op == BINARY_OP::EXPONENTIATION)
		return std::pow(a, b);

	throw Error(
		COMPILE_RESULT::INTERNAL_ERROR,
		"Error occured in CPP::VM::sub_calc_2.",
		current.token
	);
}

} // LibOrichalcum
