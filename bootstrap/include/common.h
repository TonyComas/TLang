#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static inline void fatal(const char* msg) {
  fprintf(stderr, "error: %s\n", msg);
  exit(1);
}

static inline char* read_file(const char* path) {
  FILE* f = fopen(path, "rb");
  if (!f)
    fatal("could not open file");

  fseek(f, 0, SEEK_END);
  long size = ftell(f);
  rewind(f);

  char* buf = malloc(size + 1);
  if (!buf)
    fatal("out of memory");

  fread(buf, 1, size, f);
  buf[size] = 0;
  fclose(f);
  return buf;
}
