#pragma once

int sym_lookup(const char* name);
int sym_define(const char* name);
int sym_count(void);
void symtab_cleanup(void);