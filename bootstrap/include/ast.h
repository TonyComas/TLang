#pragma once

typedef enum {
  AST_RETURN,
  AST_FUNC,
  AST_INT,
  AST_ADD,
  AST_SUB,
  AST_MUL,
  AST_DIV,
  AST_VAR,
  AST_LET,
  AST_ASSIGN,
  AST_SEQ,
  AST_BLOCK,
  AST_IF,
  AST_EQ,
  AST_NE,
  AST_LT,
  AST_LTE,
  AST_GT,
  AST_GTE,
  AST_AND,
  AST_OR,
  AST_NOT

} ASTType;

typedef struct AST {
  ASTType type;
  struct AST* lhs;
  struct AST* rhs;
  long value;
  const char* name;
} AST;
AST* new_ast(ASTType type, AST* lhs, AST* rhs, long value, const char* name);