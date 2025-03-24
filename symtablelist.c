#include "symtable.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
  char *key;
  void *value;
  struct Node *next;
} Node;

struct SymTable {
  struct Node *first;
  size_t length;
};

SymTable_T SymTable_new(void) {
  SymTable_T symTable = (SymTable_T) malloc (sizeof (struct SymTable));
  if (symTable == NULL) {
    return NULL;
  }
  symTable -> first = NULL;
  symTable -> length = 0;
  return symTable;
}

void SymTable_free(SymTable_T oSymTable) {
  Node *currNode;
  Node *nextNode;
  assert(oSymTable);
    currNode = oSymTable -> first;
    while (currNode != NULL) {
        nextNode = currNode -> next;
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
  Node *newNode;
  assert (oSymTable);
  assert (pcKey);

  if (!SymTable_contains (oSymTable, pcKey)) {
    newNode = (Node*) malloc (sizeof(Node));
    
    if (newNode == NULL)
    {
      return 0;
    }
    
    newNode -> key = malloc (strlen (pcKey) + 1);
    
    if (newNode -> key == NULL) {
        free (newNode);
        return 0;
    }
    
    strcpy(newNode -> key, pcKey);
    newNode -> value = (void *) pvValue;
    newNode -> next = oSymTable -> first;
    oSymTable -> first = newNode;
    oSymTable -> length++;
    return 1;
    }
  else {
    return 0;
  }
}

void *SymTable_replace(SymTable_T oSymTable, const char *pcKey, const void *pvValue) {
  Node *currNode;
  Void *ogValue;
  assert (oSymTable);
  assert (pcKey);

  currNode = oSymTable -> first;
  while (currNode != NULL) {
    if (strcmp (currNode -> key, pcKey) == 0)
    {
      ogValue = currNode -> value;
      currNode -> value = (void *)pvValue;
      return ogValue;
    }
    currNode = currNode -> next;
  }
  return NULL;
}

int SymTable_contains(SymTable_T oSymTable, const char *pcKey) {
  Node *currNode;
  assert (oSymTable);
  assert (pcKey);

  currNode = oSymTable -> first;
  while (currNode != NULL) {
    if (strcmp (currNode -> key, pcKey) == 0)
    {
      return 1;
    }
    currNode = currNode -> next;
  }
  return 0;
}

void *SymTable_get(SymTable_T oSymTable, const char *pcKey) {
  Node *currNode;
  assert (oSymTable);
  assert (pcKey);
  
  currNode = oSymTable -> first;
  while (currNode != NULL) {
    if (strcmp (currNode -> key, pcKey) == 0)
    {
      return currNode -> value;
    }
    currNode = currNode -> next;
  }
  return NULL;
}

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey) {
  Node *currNode;
  Node *prevNode;
  void *currValue;
  assert (oSymTable);
  assert (pcKey);
  
  currNode = oSymTable -> first;
  prevNode = NULL;
    
  while (currNode != NULL) {
    if (strcmp(currNode -> key, pcKey) == 0) {
      currValue = currNode -> value;
      
      if (prevNode != NULL) {
        prevNode -> next = currNode -> next;
      }
      else {
        oSymTable -> first = currNode -> next;
      }
      
      free(currNode -> key);
      free(currNode);
      oSymTable -> length--;
      return currValue;
    }
    prevNode = currNode;
    currNode = currNode -> next;
  }
return NULL;   
}

void SymTable_map(SymTable_T oSymTable, void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra), const void *pvExtra) {
  Node *currNode;
  assert (oSymTable);
  assert (pfApply);
  currNode = oSymTable -> first;
  while (currNode != NULL) {
    (*pfApply)(currNode -> key, currNode -> value, (void *) pvExtra);
        currNode = currNode -> next;
  }
}
