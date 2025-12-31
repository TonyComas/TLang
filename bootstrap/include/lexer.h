#pragma once
#include "token.h"

void lexer_init(const char* src);
Token next_token(void);
