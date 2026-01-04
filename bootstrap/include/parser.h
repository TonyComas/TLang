#pragma once
#include "common.h"
#include "ast.h"
#include "lexer.h"

AST* parse_primary(void);
AST* parse_mul(void);
AST* parse_add(void);
AST* parse_expr(void);
AST* parse_stmt(void);
AST* parse_program(void);
