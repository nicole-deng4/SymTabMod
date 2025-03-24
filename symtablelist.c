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
      return 0;
    }
    
    newNode -> key = malloc (strlen (pcKey) + 1);
    
    if (newNode -> key == NULL) {
        free (newNode);
        return 0;
    }
    
    strcpy(newNode->key, pcKey);
    newNode -> value = (void *)pvValue;
    newNode -> next = oSymTable -> head;
    oSymTable -> head = newNode;
    oSymTable -> length++;
    return 1;
    }
  else {
    return 0;
  }
}

void *SymTable_replace(SymTable_T oSymTable, const char *pcKey, const void *pvValue) {
  assert (oSymTable);
  assert (pcKey);

  Node *currNode = OSymTable -> head;
  while (currNode != NULL) {
    if (strcmp (currNode -> key, pcKey) == 0)
    {
      void *ogValue = currNode -> value;
      curr -> value = (void *)pvValue;
      return ogValue;
    }
    currNode = currNode -> next;
  }
  return NULL;
}

int SymTable_contains(SymTable_T oSymTable, const char *pcKey) {
  assert (oSymTable);
  assert (pcKey);

  Node *currNode = OSymTable -> head;
  while (currNode != NULL) {
    if (strcmp (currNode -> key, pcKey) == 0)
    {
      return 1;
    }
    currNode = currNode -> next;
  }
  return NULL;
}

void *SymTable_get(SymTable_T oSymTable, const char *pcKey) {
  assert (oSymTable);
  assert (pcKey);
  
  Node *currNode = OSymTable -> head;
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
  assert (oSymTable);
  assert (pcKey);
  
  Node *currNode = oSymTable -> head;
  Node *prevNode = NULL;
    
  while (currNode != NULL) {
    if (strcmp(currNode -> key, pcKey) == 0) {
      void *currValue = current -> value;
      
      if (prevNode != NULL) {
        prevNode -> next = currNode -> next;
      }
      else {
        oSymTable -> head = currNode -> next;
      }
      
      free(currNode -> key);
      free(currNode);
      oSymTable -> length--;
      return currValue;
    }
    else {
      prevNode = currNode;
      currNode = currNode -> next;
    }
return NULL;   
}

void SymTable_map(SymTable_T oSymTable, void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra), const void *pvExtra) {
  assert (oSymTable);
  assert (pfApply);
  Node *currNode = oSymTable -> head;
  while (currNode != NULL) {
    (*pfApply)(currNode -> key, currNode -> value, pvExtra);
        currNode = currNode -> next;
  }
}
