#include "lexer.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static const char* src;
static Token current;

void lexer_init(const char* input) {
  src = input;
}

static void skip_ws() {
  while (*src == ' ' || *src == '\n' || *src == '\r' || *src == '\t') {
    src++;
  }
}

Token next_token() {
  skip_ws();

  Token tok = {0};
  tok.pos = src;

  if (*src == '\0') {
    tok.type = TOK_EOF;
    return tok;
  }

  if (*src >= '0' && *src <= '9') {
    long val = 0;
    while (*src >= '0' && *src <= '9') {
      val = val * 10 + (*src - '0');
      src++;
    }
    tok.type = TOK_INT;
    tok.value = val;
    return tok;
  }

  if (!strncmp(src, "return", 6) && !isalnum(src[6])) {
    src += 6;
    tok.type = TOK_RETURN;
    return tok;
  }

  switch (*src++) {
  case '+':
    tok.type = TOK_PLUS;
    break;
  case '-':
    tok.type = TOK_MINUS;
    break;
  case '*':
    tok.type = TOK_STAR;
    break;
  case '/':
    tok.type = TOK_SLASH;
    break;
  case '(':
    tok.type = TOK_LPAREN;
    break;
  case ')':
    tok.type = TOK_RPAREN;
    break;
  case ';':
    tok.type = TOK_SEMI;
    break;
  default:
    fprintf(stderr, "Unexpected character\n");
    exit(1);
  }

  return tok;
}
