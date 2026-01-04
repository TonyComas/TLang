#pragma once

typedef enum {
  TOK_EOF,
  TOK_FN,
  TOK_RETURN,
  TOK_LET,
  TOK_CONST,
  TOK_IDENT,
  TOK_LPAREN,
  TOK_RPAREN,
  TOK_LBRACE,
  TOK_RBRACE,
  TOK_ARROW,
  TOK_ASSIGN,
  TOK_SEMI,
  TOK_INT,
  TOK_PLUS,
  TOK_MINUS,
  TOK_STAR,
  TOK_SLASH,
  TOK_EQ,
  TOK_NE,
  TOK_LT,
  TOK_LTE,
  TOK_GT,
  TOK_GTE,
  TOK_IF
} TokenType;

typedef struct {
  TokenType type;
  const char* pos;
  const char* name;
  int value;
} Token;
