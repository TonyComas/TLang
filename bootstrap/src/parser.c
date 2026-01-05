#include "parser.h"
#include "lexer.h"
#include "ast.h"
#include "common.h"
#include "symtab.h"

static void advance(void);
static void expect(TokenType type);
static void error(const char* msg);
AST* parse_primary(void);
AST* parse_mul(void);
AST* parse_add(void);
AST* parse_expr(void);
AST* parse_stmt(void);
AST* parse_program(void);
AST* parse_block(void);
AST* parse_eq(void);
AST* parse_cmp(void);
AST* parse_and(void);
AST* parse_or(void);
AST* parse_not(void);

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
  AST* node = parse_not();

  while (tok.type == TOK_STAR || tok.type == TOK_SLASH) {
    TokenType op = tok.type;
    advance();
    AST* rhs = parse_not();
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
  return parse_or();
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

  if (tok.type == TOK_LBRACE) {
    return parse_block();
  }

  if (tok.type == TOK_IF) {
    advance();
    expect(TOK_LPAREN);
    AST* cond = parse_expr();
    expect(TOK_RPAREN);

    AST* then = parse_stmt();
    return new_ast(AST_IF, cond, then, 0, NULL);
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

AST* parse_block() {
  advance();

  AST* block = NULL;

  while (tok.type != TOK_RBRACE && tok.type != TOK_EOF) {
    AST* stmt = parse_stmt();
    if (!block)
      block = stmt;
    else
      block = new_ast(AST_SEQ, block, stmt, 0, NULL);
  }

  expect(TOK_RBRACE);

  return block;
}

AST* parse_cmp() {
  AST* node = parse_add();

  while (tok.type == TOK_LT || tok.type == TOK_LTE || tok.type == TOK_GT ||
         tok.type == TOK_GTE) {

    TokenType op = tok.type;
    advance();
    AST* rhs = parse_add();

    ASTType type;
    switch (op) {
    case TOK_LT:
      type = AST_LT;
      break;
    case TOK_LTE:
      type = AST_LTE;
      break;
    case TOK_GT:
      type = AST_GT;
      break;
    case TOK_GTE:
      type = AST_GTE;
      break;
    default:
      error("Invalid comparison op");
    }

    node = new_ast(type, node, rhs, 0, NULL);
  }

  return node;
}

AST* parse_eq() {
  AST* node = parse_cmp();

  while (tok.type == TOK_EQ || tok.type == TOK_NE) {
    TokenType op = tok.type;
    advance();
    AST* rhs = parse_cmp();

    node = new_ast(op == TOK_EQ ? AST_EQ : AST_NE, node, rhs, 0, NULL);
  }

  return node;
}

AST* parse_or(void) {
  AST* node = parse_and();

  while (tok.type == TOK_OR) {
    advance();
    node = new_ast(AST_OR, node, parse_and(), 0, NULL);
  }

  return node;
}

AST* parse_and(void) {
  AST* node = parse_eq();

  while (tok.type == TOK_AND) {
    advance();
    node = new_ast(AST_AND, node, parse_eq(), 0, NULL);
  }

  return node;
}

AST* parse_not(void) {
  if (tok.type == TOK_NOT) {
    advance();
    return new_ast(AST_NOT, parse_not(), NULL, 0, NULL);
  }

  return parse_primary();
}
