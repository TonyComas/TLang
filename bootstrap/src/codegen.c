#include <stdio.h>
#include "codegen.h"
#include "symtab.h"
#include <stdlib.h>

static int label_id = 0;

static int new_label(void) {
  return label_id++;
}

void codegen_program(AST* root) {
  int localsize = sym_count() * 8;

  printf(".global main\n");
  printf("main:\n");

  printf("    push %%rbp\n");
  printf("    mov %%rsp, %%rbp\n");

  if (localsize > 0) {
    localsize = ((localsize + 15) / 16) * 16;
    printf("    sub $%d, %%rsp\n", localsize);
  }

  codegen_stmt(root);

  printf("    mov $0, %%rax\n");
  printf("    leave\n");
  printf("    ret\n");
}

void codegen_stmt(AST* node) {
  if (!node)
    return;

  switch (node->type) {
  case AST_SEQ:
    codegen_stmt(node->lhs);
    codegen_stmt(node->rhs);
    break;

  case AST_LET: {
    codegen_expr(node->lhs);
    int offset = sym_lookup(node->name);
    if (offset < 0) {
      fprintf(stderr, "Undefined variable in codegen: %s\n", node->name);
      return;
    }
    printf("    mov %%rax, -%d(%%rbp)\n", offset);
    break;
  }
  case AST_ASSIGN: {
    int offset = sym_lookup(node->name);
    if (offset < 0) {
      fprintf(stderr, "Assign to undefined variable: %s\n", node->name);
      exit(1);
    }

    codegen_expr(node->lhs); // RHS → rax
    printf("    mov %%rax, -%d(%%rbp)\n", offset);
    break;
  }
  case AST_RETURN:
    codegen_expr(node->lhs);
    printf("    leave\n");
    printf("    ret\n");
    break;
  case AST_IF: {
    int lbl_end = new_label();

    codegen_expr(node->lhs);

    printf("    cmp $0, %%rax\n");
    printf("    je .Lend%d\n", lbl_end);

    codegen_stmt(node->rhs);

    printf(".Lend%d:\n", lbl_end);
    break;
  }
  default:
    codegen_expr(node);
    break;
  }
}

void codegen_expr(AST* node) {
  if (!node)
    return;

  switch (node->type) {
  case AST_INT:
    printf("    mov $%ld, %%rax\n", node->value);
    return;

  case AST_VAR: {
    int offset = sym_lookup(node->name);
    if (offset < 0) {
      fprintf(stderr, "Undefined variable in codegen_expr: %s\n", node->name);
      return;
    }
    printf("    mov -%d(%%rbp), %%rax\n", offset);
    return;
  }

  case AST_ADD:
  case AST_SUB:
  case AST_MUL:
  case AST_DIV:
  case AST_EQ:
  case AST_NE:
  case AST_LT:
  case AST_LTE:
  case AST_GT:
  case AST_GTE:
    break;

  default:
    fprintf(stderr, "Invalid expression\n");
    return;
  }

  codegen_expr(node->lhs);
  printf("    push %%rax\n");
  codegen_expr(node->rhs);
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
    printf("    mov %%rax, %%rdi\n");
    printf("    mov %%rcx, %%rax\n");
    printf("    cqo\n");
    printf("    idiv %%rdi\n");
    break;

  case AST_LT:
    printf("    cmp %%rax, %%rcx\n");
    printf("    setl %%al\n");
    printf("    movzb %%al, %%rax\n");
    break;
  case AST_LTE:
    printf("    cmp %%rax, %%rcx\n");
    printf("    setle %%al\n");
    printf("    movzb %%al, %%rax\n");
    break;
  case AST_GT:
    printf("    cmp %%rax, %%rcx\n");
    printf("    setg %%al\n");
    printf("    movzb %%al, %%rax\n");
    break;
  case AST_GTE:
    printf("    cmp %%rax, %%rcx\n");
    printf("    setge %%al\n");
    printf("    movzb %%al, %%rax\n");
    break;
  case AST_EQ:
    printf("    cmp %%rax, %%rcx\n");
    printf("    sete %%al\n");
    printf("    movzb %%al, %%rax\n");
    break;
  case AST_NE:
    printf("    cmp %%rax, %%rcx\n");
    printf("    setne %%al\n");
    printf("    movzb %%al, %%rax\n");
    break;
  }
}