#include "lexer.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static const char* src;

void lexer_init(const char* input) {
  src = input;
}

static void skip_ws(void) {
  while (*src == ' ' || *src == '\n' || *src == '\r' || *src == '\t')
    src++;
}

static int match_keyword(const char* start, int len, const char* kw) {
  return (int)strlen(kw) == len && strncmp(start, kw, len) == 0;
}

char* strndup(const char* s, size_t n) {
  char* p = malloc(n + 1);
  if (!p) {
    return NULL;
  }
  strncpy(p, s, n);
  p[n] = '\0';
  return p;
}

Token next_token(void) {
  skip_ws();

  Token tok = {0};
  tok.pos = src;

  if (*src == '\0') {
    tok.type = TOK_EOF;
    return tok;
  }

  if (isdigit(*src)) {
    long val = 0;
    while (isdigit(*src)) {
      val = val * 10 + (*src - '0');
      src++;
    }
    tok.type = TOK_INT;
    tok.value = val;
    return tok;
  }

  if (isalpha(*src) || *src == '_') {
    const char* start = src;
    while (isalnum(*src) || *src == '_') {
      src++;
    }

    int len = src - start;

    if (match_keyword(start, len, "return")) {
      tok.type = TOK_RETURN;
      return tok;
    }

    if (match_keyword(start, len, "let")) {
      tok.type = TOK_LET;
      return tok;
    }

    tok.type = TOK_IDENT;
    tok.name = strndup(start, len);
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
  case '=':
    tok.type = TOK_ASSIGN;
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
  case '{':
    tok.type = TOK_LBRACE;
    break;
  case '}':
    tok.type = TOK_RBRACE;
    break;
  default:
    fprintf(stderr, "Unexpected character: '%c'\n", src[-1]);
    exit(1);
  }

  return tok;
}
