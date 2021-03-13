#include "VM.hpp"

#include <iostream>

#include "Debug.hpp"
#include "Log.hpp"

namespace OrichalcumLib {

VM::VM():
	vm_debug(false), chunk(0), chunk_iter(chunk.get_iterator()) { }

void VM::enable_debug() {
	vm_debug = true;
}

InterpretReport VM::interpret(const Chunk &_chunk) {
	InterpretReport interpret_report;
	init_chunk(_chunk);

	interpret_report.result = run();

	return interpret_report;
}

void VM::init_chunk(const Chunk &_chunk) {
	chunk = _chunk;
	chunk_iter = chunk.get_iterator();
}

INTERPRET_RESULT VM::run() {
	Log::debug("Vm is executing chunk " + chunk.name);
	if (vm_debug) {
		Debug::disassemble_chunk(chunk);
	}

	while(true) {
		increment();
		switch(instruction.op_code) {
			case OP_CODE::RETURN: {
				return INTERPRET_RESULT::OK;
			}

			case OP_CODE::CONST_INT: {
				std::cout << get_const_int() << std::endl;
			} break;

			case OP_CODE::CONST_FLOAT: {
				std::cout << get_const_float() << std::endl;
			} break;
		}
	}
}

int64_t VM::get_const_int() {
	increment();
	return chunk.get_int(instruction.index);
}

double VM::get_const_float() {
	increment();
	return chunk.get_float(instruction.index);
}

// Gets the currents instruction, then increments the iterator
void VM::increment() {
	instruction = *chunk_iter;
	chunk_iter = chunk.next(chunk_iter);
}

} // OrichalcumLib
