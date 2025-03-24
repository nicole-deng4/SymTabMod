#ifndef SYMTABLE_H
#define SYMTABLE_H
#include <stddef.h>

typedef struct SymTable *SymTable_T;

/* Creates a new symbol table and returns a pointer to it */
SymTable_T SymTable_new(void);

/* Frees all the memory taken by oSymTable */
void SymTable_free(SymTable_T oSymTable);

/* Returns the number of bindings in oSymTable */
size_t SymTable_getLength(SymTable_T oSymTable);

/* Returns 1 if a new binding was successfully added to oSymTable, returns 0 if it was unsuccessful */
int SymTable_put(SymTable_T oSymTable,
  const char *pcKey, const void *pvValue);

/* Replaces the value bound to pcKey with pvValue */
void *SymTable_replace(SymTable_T oSymTable,
  const char *pcKey, const void *pvValue);

/* Returns 1 if oSymTable has a binding for pcKey, returns 1 if it doesn't */
int SymTable_contains(SymTable_T oSymTable, const char *pcKey);

/* Returns the value bound to pcKey or NULL if not found */
void *SymTable_get(SymTable_T oSymTable, const char *pcKey);

/* Removes the value bound to pcKey, returns the removed value or NULL if not found */
void *SymTable_remove(SymTable_T oSymTable, const char *pcKey);

/* STILL NEED COMMENT */
void SymTable_map(SymTable_T oSymTable,
  void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra), 
  const void *pvExtra);

#endif
