#pragma once
#include <string>

#include "Chunk.hpp"
#include "ExprAST.hpp"
#include "Lexer.hpp"
#include "VM.hpp"

namespace LibOrichalcum::Debug {

void print_token(const Token &token);
void print_expr(ExprAST &expr_ast);

void disassemble_chunk(std::shared_ptr<Chunk> chunk);
size_t disassemble_instruction(size_t index, std::shared_ptr<Chunk> chunk);

std::string to_string(const OrValue &constant);
void print_const(const OrValue &constant);
void print_stack(ConstStack &stack);

std::string to_string(OP_CODE op_code);
} // LibOrichalcum
