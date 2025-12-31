#pragma once

typedef enum { AST_RETURN, AST_FUNC, AST_INT, AST_ADD, AST_SUB, AST_MUL, AST_DIV } ASTType;

typedef struct AST {
  ASTType type;
  struct AST* lhs;
  struct AST* rhs;
  long value;
} AST;
AST* new_ast(ASTType type, AST* lhs, AST* rhs, long value);