/* SymTable Hash Table Implementation:
This file implements a symbol table of string keys and void pointer values using a hash table data structure.
*/

#include "symtable.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

/* Sets the initial number of buckets to be 509. */
#define INITIAL_BUCKET_COUNT 509 
static const size_t PRIME_BUCKET_SIZES[] = {509, 1021, 2039, 4093, 8191, 16381, 32749, 65521};
#define NUM_PRIMES (sizeof(PRIME_BUCKET_SIZES) / sizeof(PRIME_BUCKET_SIZES[0]))

/* Defines a linked list node that stores a key-value pair for separate chaining */
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
  /* Pointer to the array of pointers to the bucket nodes. */
  Node **buckets; 
  /* The total number of key-value bindings in the symbol table. */
  size_t length; 
  /* The total number of buckets in the hash table. */
  size_t totalNumBuckets;
  /* The current expansion size in the sequence of prime bucket sizes. */
  size_t expandIndex;
};

/* Return a hash code for pcKey that is between 0 and uBucketCount-1, inclusive. */
static size_t SymTable_hash(const char *pcKey, size_t uBucketCount)
{
   const size_t HASH_MULTIPLIER = 65599;
   size_t u;
   size_t uHash = 0;

   assert(pcKey != NULL);

   for (u = 0; pcKey[u] != '\0'; u++)
      uHash = uHash * HASH_MULTIPLIER + (size_t)pcKey[u];

   return uHash % uBucketCount;
}

/* Creates a new symbol table and returns a pointer to it */
SymTable_T SymTable_new(void) {
    SymTable_T oSymTable = (SymTable_T) malloc (sizeof (struct SymTable));
    if (oSymTable == NULL) {
      return NULL;
    }
  
    oSymTable -> totalNumBuckets = INITIAL_BUCKET_COUNT;
    oSymTable -> length = 0;
    oSymTable -> buckets = (Node **) calloc (oSymTable -> totalNumBuckets, sizeof(Node *));
    oSymTable -> expandIndex = 0;

    if (oSymTable -> buckets == NULL) {
      free(oSymTable);
      return NULL;
    }
    return oSymTable;
}

/* Frees all the memory taken by oSymTable */
void SymTable_free(SymTable_T oSymTable) {
  Node *currNode;
  Node *nextNode;
  size_t i;
  assert(oSymTable != NULL);
  for (i = 0; i < oSymTable -> totalNumBuckets; i++) {
    currNode = oSymTable -> buckets [i];
    while (currNode != NULL) {
        nextNode = currNode -> next;
        free (currNode->key);
        free (currNode);
        currNode = nextNode;
    }
  }
  free (oSymTable -> buckets);
  free (oSymTable);
}

/* Returns the number of bindings in oSymTable */
size_t SymTable_getLength(SymTable_T oSymTable) {
  assert (oSymTable != NULL);
  return (oSymTable -> length);
}

/*
/* Expands the size of oSymTable if needed. */
static void SymTable_expand(SymTable_T oSymTable) {
    size_t newBucketCount;
    Node **newBuckets;
    Node *currBucket;
    Node *nextBucket;
    size_t newIndex;
    size_t i;
  
    if (oSymTable -> expandIndex >= NUM_PRIMES - 1 || oSymTable -> totalNumBuckets >= PRIME_BUCKET_SIZES[NUM_PRIMES - 1]) {
        return;
    }
    
    newBucketCount = PRIME_BUCKET_SIZES [oSymTable -> expandIndex + 1];
    
    newBuckets = (Node **) calloc (newBucketCount, sizeof(Node *));
    if (newBuckets == NULL) {
        return;
    }
    
    for (i = 0; i < oSymTable -> totalNumBuckets; i++) {
        currBucket = oSymTable -> buckets[i];
        while (currBucket != NULL) {
            nextBucket = currBucket -> next;
            newIndex = SymTable_hash(currBucket->key, newBucketCount);
            currBucket -> next = newBuckets[newIndex];
            newBuckets[newIndex] = currBucket;
            currBucket = nextBucket;
        }
    }
    
    free (oSymTable -> buckets);
    oSymTable -> buckets = newBuckets;
    oSymTable -> totalNumBuckets = newBucketCount;
    oSymTable -> expandIndex++;
}
*/

/* Returns 1 if a new binding with key pcKey and value pvValue was successfully added to oSymTable, returns 0 if it was unsuccessful. */
int SymTable_put(SymTable_T oSymTable, const char *pcKey, const void *pvValue) {
  Node *newNode;
  size_t hashIndex;
  assert (oSymTable != NULL);
  assert (pcKey != NULL);
  
  if (!SymTable_contains (oSymTable, pcKey)) {
    hashIndex = SymTable_hash (pcKey, oSymTable -> totalNumBuckets);
    newNode = (Node*) malloc (sizeof(Node));
    
    if (newNode == NULL) {
      return 0;
    }
    
    newNode -> key = malloc (strlen (pcKey) + 1);
    
    if (newNode -> key == NULL) {
        free (newNode);
        return 0;
    }
    
    strcpy(newNode -> key, pcKey);
    newNode -> value = (void *) pvValue;
    newNode -> next = oSymTable -> buckets [hashIndex];
    oSymTable -> buckets[hashIndex] = newNode;
    oSymTable -> length++;
    
    /*if (oSymTable -> length > oSymTable -> totalNumBuckets) {
      SymTable_expand (oSymTable);
    } */
    
    return 1;
    }
  else {
    return 0;
  }
}

/* Replaces the value bound to pcKey with pvValue in oSymTable. */
void *SymTable_replace(SymTable_T oSymTable, const char *pcKey, const void *pvValue) {
  Node *currBucket;
  void *ogValue;
  size_t hashIndex;
  assert (oSymTable != NULL);
  assert (pcKey != NULL);
  
  hashIndex = SymTable_hash (pcKey, oSymTable -> totalNumBuckets);
  currBucket = oSymTable -> buckets [hashIndex];

  while (currBucket != NULL) {
    if (strcmp (currBucket -> key, pcKey) == 0) {
      ogValue = currBucket -> value;
      currBucket -> value = (void *) pvValue;
      return ogValue;
    }
    currBucket = currBucket -> next;
  }
  return NULL;
}

/* Returns 1 if oSymTable has a binding for pcKey, returns 0 if it doesn't */
int SymTable_contains(SymTable_T oSymTable, const char *pcKey) {
  Node *currBucket;
  size_t hashIndex;
  assert (oSymTable != NULL);
  assert (pcKey != NULL);
  
  hashIndex = SymTable_hash(pcKey, oSymTable -> totalNumBuckets);
  currBucket = oSymTable -> buckets [hashIndex];
  while (currBucket != NULL) {
    if (strcmp (currBucket -> key, pcKey) == 0) {
      return 1;
    }
    currBucket = currBucket -> next;
  }
  return 0;
}

/* Returns the value bound to pcKey or NULL if not found in oSymTable. */
void *SymTable_get(SymTable_T oSymTable, const char *pcKey) {
  Node *currBucket;
  size_t hashIndex;
  assert (oSymTable != NULL);
  assert (pcKey != NULL);

  hashIndex = SymTable_hash(pcKey, oSymTable -> totalNumBuckets);
  currBucket = oSymTable -> buckets [hashIndex];
  while (currBucket != NULL) {
    if (strcmp (currBucket -> key, pcKey) == 0) {
      return currBucket -> value;
    }
    currBucket = currBucket -> next;
  }
  return NULL;
}

/* Removes the value bound to pcKey, returns the removed value or NULL if not found in oSymTable. */
void *SymTable_remove(SymTable_T oSymTable, const char *pcKey) {
  Node *currBucket;
  Node *prevBucket;
  void *currValue;
  size_t hashIndex;
  assert (oSymTable != NULL);
  assert (pcKey != NULL);

  hashIndex = SymTable_hash(pcKey, oSymTable -> totalNumBuckets);  
  currBucket = oSymTable -> buckets [hashIndex];  
  prevBucket = NULL;
    
  while (currBucket != NULL) {
    if (strcmp(currBucket -> key, pcKey) == 0) {
      if (prevBucket != NULL) {
        prevBucket -> next = currBucket -> next;
      }
      else {
        oSymTable -> buckets [hashIndex] = currBucket -> next;
      }
      currValue = currBucket -> value;
      free(currBucket -> key);
      free(currBucket);
      oSymTable -> length--;
      return currValue;
    }
    prevBucket = currBucket;
    currBucket = currBucket -> next;
  }
return NULL;   
}

/* Applies the function pointed to by pfApply to each binding with key pcKey and value pvValue in the oSymTable, passing an additional 
user-specified argument pvExtra. */
void SymTable_map(SymTable_T oSymTable, void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra), const void *pvExtra) {
  Node *currBucket;
  size_t i;
  assert (oSymTable != NULL);
  assert (pfApply != NULL);

  for (i = 0; i < oSymTable -> totalNumBuckets; i++) {
    currBucket = oSymTable -> buckets [i];
    while (currBucket) {
      (*pfApply) (currBucket -> key, currBucket -> value, (void *) pvExtra);
      currBucket = currBucket -> next;
    }
  }
}
