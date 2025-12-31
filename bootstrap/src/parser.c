#include "parser.h"
#include "lexer.h"
#include "ast.h"
#include "common.h"

static Token tok;

static void advance(void) {
  tok = next_token();
}

static void expect(TokenType type) {
  if (tok.type != type) {
    fprintf(stderr, "Unexpected token\n");
    exit(1);
  }
  advance();
}

AST* parse_primary() {
  if (tok.type == TOK_INT) {
    AST* n = new_ast(AST_INT, NULL, NULL, tok.value);
    advance();
    return n;
  }

  if (tok.type == TOK_LPAREN) {
    advance();
    AST* n = parse_expr();
    expect(TOK_RPAREN);
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
    node = new_ast(op == TOK_STAR ? AST_MUL : AST_DIV, node, rhs, 0);
  }
  return node;
}

AST* parse_add() {
  AST* node = parse_mul();

  while (tok.type == TOK_PLUS || tok.type == TOK_MINUS) {
    TokenType op = tok.type;
    advance();
    AST* rhs = parse_mul();
    node = new_ast(op == TOK_PLUS ? AST_ADD : AST_SUB, node, rhs, 0);
  }
  return node;
}

AST* parse_expr() {
  return parse_add();
}

AST* parse_stmt() {
  advance();
  expect(TOK_RETURN);
  AST* expr = parse_expr();
  expect(TOK_SEMI);
  return expr;
}