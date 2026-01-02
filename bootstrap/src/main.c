#include "common.h"
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "codegen.h"

int main(int argc, char** argv) {
  if (argc < 2) {
    fatal("usage: myc0 <file>");
  }

  char* source = read_file(argv[1]);

  lexer_init(source);
  AST* tree = parse_program();
  codegen_program(tree);
}
