#include "ast.h"
#include <stdlib.h>

AST* new_ast(ASTType type, AST* lhs, AST* rhs, long value) {
  AST* node = malloc(sizeof(AST));
  node->type = type;
  node->lhs = lhs;
  node->rhs = rhs;
  node->value = value;
  return node;
}
