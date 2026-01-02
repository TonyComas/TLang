#include "parser.h"
#include "lexer.h"
#include "ast.h"
#include "common.h"
#include "symtab.h"

static Token tok;

static void advance(void) {
  tok = next_token();
}

static void expect(TokenType type) {
  if (tok.type != type) {
    fprintf(stderr, "Unexpected token %s\n", tok.type);
    exit(1);
  }
  advance();
}

void error(const char* msg) {
  fprintf(stderr, "Error: %s\n", msg);
  exit(1);
}

AST* parse_primary() {
  if (tok.type == TOK_INT) {
    AST* n = new_ast(AST_INT, NULL, NULL, tok.value, NULL);
    advance();
    return n;
  }

  if (tok.type == TOK_LPAREN) {
    advance();
    AST* n = parse_expr();
    expect(TOK_RPAREN);
    return n;
  }

  if (tok.type == TOK_IDENT) {
    int offset = sym_lookup(tok.name);
    if (offset < 0)
      error("Undefined variable");

    AST* n = new_ast(AST_VAR, NULL, NULL, 0, tok.name);
    advance();
    return n;
  }

  fprintf(stderr, "Expected expression\n");
  exit(1);
}

AST* parse_mul() {
  AST* node = parse_primary();

  while (tok.type == TOK_STAR || tok.type == TOK_SLASH) {
    TokenType op = tok.type;
    advance();
    AST* rhs = parse_primary();
    node = new_ast(op == TOK_STAR ? AST_MUL : AST_DIV, node, rhs, 0, NULL);
  }
  return node;
}

AST* parse_add() {
  AST* node = parse_mul();

  while (tok.type == TOK_PLUS || tok.type == TOK_MINUS) {
    TokenType op = tok.type;
    advance();
    AST* rhs = parse_mul();
    node = new_ast(op == TOK_PLUS ? AST_ADD : AST_SUB, node, rhs, 0, NULL);
  }
  return node;
}

AST* parse_expr() {
  return parse_add();
}

AST* parse_stmt() {
  if (tok.type == TOK_LET) {
    advance();
    Token name = tok;
    expect(TOK_IDENT);
    expect(TOK_ASSIGN);
    AST* expr = parse_expr();
    expect(TOK_SEMI);

    sym_define(name.name);
    return new_ast(AST_LET, expr, NULL, 0, name.name);
  }

  if (tok.type == TOK_RETURN) {
    advance();
    AST* expr = parse_expr();
    expect(TOK_SEMI);
    return new_ast(AST_RETURN, expr, NULL, 0, NULL);
  }

  if (tok.type == TOK_IDENT) {
    Token name = tok;
    advance();
    expect(TOK_ASSIGN);
    AST* expr = parse_expr();
    expect(TOK_SEMI);
    return new_ast(AST_ASSIGN, expr, NULL, 0, name.name);
  }

  fprintf(stderr, "Unexpected stmt in parse_stmt: %s\n", tok.type);
  exit(1);
}

AST* parse_program() {
  advance();
  AST* root = NULL;
  while (tok.type != TOK_EOF) {
    AST* stmt = parse_stmt();
    if (!root) {
      root = stmt;
    } else {
      root = new_ast(AST_SEQ, root, stmt, 0, NULL);
    }
  }
  return root;
}
