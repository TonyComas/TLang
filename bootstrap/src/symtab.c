#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
  char* name;
  int offset; // offset from RBP
} Symbol;

#define MAX_SYMBOLS 256
static Symbol symbols[MAX_SYMBOLS];
static int symbol_count = 0;

// Define a new variable in the symbol table
int sym_define(const char* name) {
  if (symbol_count >= MAX_SYMBOLS) {
    fprintf(stderr, "Symbol table full\n");
    exit(1);
  }

  // Allocate a copy of the name (safe)
  symbols[symbol_count].name = strdup(name);
  if (!symbols[symbol_count].name) {
    fprintf(stderr, "Out of memory\n");
    exit(1);
  }

  // Offset from RBP, stack grows downward
  symbols[symbol_count].offset = (symbol_count + 1) * 8;
  return symbol_count++;
}

int sym_lookup(const char* name) {
  for (int i = symbol_count - 1; i >= 0; i--) {
    if (strcmp(symbols[i].name, name) == 0) {
      return symbols[i].offset;
    }
  }
  return -1;
}

int sym_count(void) {
  return symbol_count;
}

void symtab_cleanup(void) {
  for (int i = 0; i < symbol_count; i++) {
    free(symbols[i].name);
    symbols[i].name = NULL;
  }
  symbol_count = 0;
}
