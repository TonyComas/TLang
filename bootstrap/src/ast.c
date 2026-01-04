#include "ast.h"
#include <stdlib.h>

AST* new_ast(ASTType type, AST* lhs, AST* rhs, long value, const char* name) {
  AST* node = malloc(sizeof(AST));
  node->type = type;
  node->lhs = lhs;
  node->rhs = rhs;
  node->value = value;
  node->name = name;
  return node;
}
