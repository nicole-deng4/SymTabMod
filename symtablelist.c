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
  size_t length;
};

SymTable_T SymTable_new(void) {
  SymTable_T symTable = (SymTable_T) malloc (sizeOf (struct List));
  if (symTable == NULL) {
    return NULL;
  }
  symTable -> first = NULL;
  symTable -> length = 0;
  return symTable;
}

void SymTable_free(SymTable_T oSymTable) {
  assert(oSymTable);
    Node* currNode = oSymTable -> first;
    while (currNode != NULL) {
        Node* nextNode = current -> next;
        free (currNode->key);
        free (currNode);
        currNode = nextNode;
    }
    free(oSymTable);
}

size_t SymTable_getLength(SymTable_T oSymTable) {
  assert (oSymTable);
  return (oSymTable -> length);
}

int SymTable_put(SymTable_T oSymTable, const char *pcKey, const void *pvValue) {
  assert (oSymTable);
  assert (pcKey);

  if (!SymTable_contains (oSymTable, pcKey)) {
    Node* newNode = (Node*) malloc (sizeOf(Node));
    if (newNode == NULL)
    {
      return NULL;
    }
    newNode -> key = malloc (strlen (pcKey) + 1);
    if (newNode -> key == NULL) {
        free (newNode);
        return 0;
    }
    strcpy(newNode->key, pcKey);
    newNode->value = (void *)pvValue;
    newNode->next = oSymTable->head;
    oSymTable->head = newNode;
    oSymTable->length++;
    return 1;
  }

