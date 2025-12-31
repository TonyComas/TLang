#pragma once

typedef enum {
  TOK_EOF,
  TOK_FN,
  TOK_RETURN,
  TOK_IDENT,
  TOK_LPAREN,
  TOK_RPAREN,
  TOK_LBRACE,
  TOK_RBRACE,
  TOK_ARROW,
  TOK_SEMI,
  TOK_INT,
  TOK_PLUS,
  TOK_MINUS,
  TOK_STAR,
  TOK_SLASH
} TokenType;

typedef struct {
  TokenType type;
  const char* pos;
  long value;
} Token;
