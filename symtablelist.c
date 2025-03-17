#include "symtable.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
  const char* key;
  int value;
  struct Node* next;
};

struct List {
  struct Node *first;
};

SymTable_T SymTable_new(void) {
  SymTable_T symTable = (SymTable_T) malloc (sizeOf (struct List));
  if (symTable == NULL) {
    return NULL;
  }
  symTable -> first = NULL;
  return symTable;
