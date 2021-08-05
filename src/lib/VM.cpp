#include "VM.hpp"

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
	}
}

VM::VM() { }
VM::VM(bool _debug):debug(_debug) { }

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
	Log::debug("Vm is executing chunk " + chunk->name);
	if (debug) {
		Debug::print_stack(stack);
		Debug::disassemble_chunk(chunk);
	}

	size_t i = 0;
	while(true) {
		Instruction instruction = chunk->get(i);
		i++;
		switch(instruction.op_code) {
			case OP_CODE::RETURN: {
				std::cout << "CHUNK RETURNED ";
				Debug::print_const(stack.pop());
				std::cout << std::endl;
				return INTERPRET_RESULT::OK;
			}

			case OP_CODE::CONST: {
				OrValue constant = chunk->get_const(instruction.index);
				stack.push(constant);
			} break;

			case OP_CODE::NEGATE: {
				negate();
			} break;
			case OP_CODE::ADD: binary_op(BINARY_OP::ADD); break;
			case OP_CODE::SUBTRACT: binary_op(BINARY_OP::SUBTRACT); break;
			case OP_CODE::MULTIPLY: binary_op(BINARY_OP::MULTIPLY); break;
			case OP_CODE::DIVIDE: binary_op(BINARY_OP::DIVIDE); break;
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

void VM::binary_op(BINARY_OP op) {
	// b first, this is important
	OrValue const_b = stack.pop();
	OrValue const_a = stack.pop();
	OrValue const_c;
	switch (const_a.type) {
		case OrValueType::INT: {
			switch (const_b.type) {
				case OrValueType::INT: {
					const_c = OrValue(calc(op, const_a.value.INT, const_b.value.INT));
				} break;
				case OrValueType::FLOAT: {
					const_c = OrValue(calc(op, static_cast<double>(const_a.value.INT), const_b.value.FLOAT));
				} break;
				case OrValueType::BOOL:
				case OrValueType::NONE: {
					throw Error(
						COMPILE_RESULT::RUNTIME_ERROR,
						"Unable to perform operation " + to_string(op) + " on types int and " + to_string(const_b.type),
						const_b.token
					);
				}
			}
		} break;
		case OrValueType::FLOAT: {
			switch (const_b.type) {
				case OrValueType::INT: {
					const_c = OrValue(calc(op, const_a.value.FLOAT, static_cast<double>(const_b.value.INT)));
				} break;
				case OrValueType::FLOAT: {
					const_c = OrValue(calc(op, const_a.value.FLOAT, const_b.value.FLOAT));
				} break;
				case OrValueType::BOOL:
				case OrValueType::NONE: {
					throw Error(
						COMPILE_RESULT::RUNTIME_ERROR,
						"Unable to perform operation " + to_string(op) + " on types float and " + to_string(const_b.type),
						const_b.token
					);
				}
			}
		} break;
		case OrValueType::BOOL:
		case OrValueType::NONE: {
			throw Error(
				COMPILE_RESULT::RUNTIME_ERROR,
				"Unable to perform operation " + to_string(op) + " on types " +
				to_string(const_a.type) + " and " + to_string(const_b.type),
				const_a.token
			);
		}
	}
	stack.push(const_c);
}

int64_t VM::calc(BINARY_OP op, int64_t a, int64_t b) {
	switch (op) {
		case BINARY_OP::ADD: return a + b;
		case BINARY_OP::SUBTRACT: return a - b;
		case BINARY_OP::MULTIPLY: return a * b;
		case BINARY_OP::DIVIDE: return a / b;
	}
}

double VM::calc(BINARY_OP op, double a, double b) {
	switch (op) {
		case BINARY_OP::ADD: return a + b;
		case BINARY_OP::SUBTRACT: return a - b;
		case BINARY_OP::MULTIPLY: return a * b;
		case BINARY_OP::DIVIDE: return a / b;
	}
}

} // LibOrichalcum
