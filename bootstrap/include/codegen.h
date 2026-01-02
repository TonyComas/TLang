#pragma once
#include "ast.h"

void codegen_program(AST* root);
void codegen_stmt(AST* node);
void codegen_expr(AST* node);