/* SymTable Linked List Implementation:
This file implements a symbol table of string keys and void pointer values using a linked list data structure.
*/

#include "symtable.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

/* Defines a linked list node that stores a key-value pair. */
typedef struct Node {
  /* The key of this binding, stored as a dynamically allocated array of characters. */
  char *key; 
  /* The value of this binding, stored as a generic pointer. */
  void *value; 
  /* Pointer to the next node in the linked list. */
  struct Node *next; 
/* End of Node struct definition. */
} Node;

/* Defines a symbol table structure. */
struct SymTable {
  /* Pointer to the first node in the linked list. */
  struct Node *first; 
  /* The total number of key-value bindings in the symbol table. */
  size_t length; 
};

/* Creates a new symbol table and returns a pointer to it */
SymTable_T SymTable_new(void) {
  SymTable_T symTable = (SymTable_T) malloc (sizeof (struct SymTable));
  if (symTable == NULL) {
    return NULL;
  }
  symTable -> first = NULL;
  symTable -> length = 0;
  return symTable;
}

/* Frees all the memory taken by oSymTable */
void SymTable_free(SymTable_T oSymTable) {
  Node *currNode;
  Node *nextNode;
  assert(oSymTable != NULL);
    currNode = oSymTable -> first;
    while (currNode != NULL) {
        nextNode = currNode -> next;
        free (currNode->key);
        free (currNode);
        currNode = nextNode;
    }
    free(oSymTable);
}

/* Returns the number of bindings in oSymTable */
size_t SymTable_getLength(SymTable_T oSymTable) {
  assert (oSymTable != NULL);
  return (oSymTable -> length);
}

/* Returns 1 if a new binding with key pcKey and value pvValue was successfully added to oSymTable, returns 0 if it was unsuccessful */
int SymTable_put(SymTable_T oSymTable, const char *pcKey, const void *pvValue) {
  Node *newNode;
  assert (oSymTable != NULL);
  assert (pcKey != NULL);

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

/* Replaces the value bound to pcKey with pvValue in oSymTable. */
void *SymTable_replace(SymTable_T oSymTable, const char *pcKey, const void *pvValue) {
  Node *currNode;
  void *ogValue;
  assert (oSymTable != NULL);
  assert (pcKey != NULL);

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

/* Returns 1 if oSymTable has a binding for pcKey, returns 0 if it doesn't */
int SymTable_contains(SymTable_T oSymTable, const char *pcKey) {
  Node *currNode;
  assert (oSymTable != NULL);
  assert (pcKey != NULL);

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

/* Returns the value bound to pcKey or NULL if not found in oSymTable. */
void *SymTable_get(SymTable_T oSymTable, const char *pcKey) {
  Node *currNode;
  assert (oSymTable != NULL);
  assert (pcKey != NULL);
  
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

/* Removes the value bound to pcKey, returns the removed value or NULL if not found in oSymTable. */
void *SymTable_remove(SymTable_T oSymTable, const char *pcKey) {
  Node *currNode;
  Node *prevNode;
  void *currValue;
  assert (oSymTable != NULL);
  assert (pcKey != NULL);
  
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

/* Applies the function pointed to by pfApply to each binding with key pcKey and value pvValue in the oSymTable, passing an additional 
user-specified argument pvExtra. */
void SymTable_map(SymTable_T oSymTable, void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra), const void *pvExtra) {
  Node *currNode;
  assert (oSymTable != NULL);
  assert (pfApply != NULL);
  currNode = oSymTable -> first;
  while (currNode != NULL) {
    (*pfApply)(currNode -> key, currNode -> value, (void *) pvExtra);
        currNode = currNode -> next;
  }
}
