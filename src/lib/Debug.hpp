#pragma once
#include <string>

#include "Chunk.hpp"
#include "data_structures.hpp"
#include "Lexer.hpp"
#include "Lexer.hpp"

namespace OrichalcumLib::Debug {

void print_token(const Token &token);
void disassemble_chunk(Chunk &chunk);
size_t disassemble_instruction(size_t index, Chunk &chunk);

std::string to_string(const Constant &constant);
void print_const(const Constant &constant);
void print_stack(ConstStack &stack);

std::string op_to_string(OP_CODE op_code);
std::string to_string(const TOKEN_TYPE token_type);

} // OrichalcumLib
