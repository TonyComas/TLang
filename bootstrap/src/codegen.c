#include <stdio.h>
#include "codegen.h"

void codegen(AST* node) {
  if (!node)
    return;

  if (node->type == AST_INT) {
    printf("    mov $%ld, %%rax\n", node->value);
    return;
  }

  codegen(node->lhs);
  printf("    push %%rax\n");

  codegen(node->rhs);
  printf("    pop %%rcx\n");

  switch (node->type) {
  case AST_ADD:
    printf("    add %%rcx, %%rax\n");
    break;

  case AST_SUB:
    printf("    sub %%rax, %%rcx\n");
    printf("    mov %%rcx, %%rax\n");
    break;

  case AST_MUL:
    printf("    imul %%rcx, %%rax\n");
    break;

  case AST_DIV:
    printf("    mov %%rax, %%rbx\n");
    printf("    mov %%rcx, %%rax\n");
    printf("    cqo\n");
    printf("    idiv %%rbx\n");
    break;
  }
}
