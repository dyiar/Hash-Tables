#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/****
  Basic hash table key/value pair
 ****/
typedef struct Pair {
  char *key;
  char *value;
} Pair;

/****
  Basic hash table
 ****/
typedef struct BasicHashTable {
  int capacity;
  Pair **storage;
} BasicHashTable;

/****
  Create a key/value pair to be stored in the hash table.
 ****/
Pair *create_pair(char *key, char *value)
{
  Pair *pair = malloc(sizeof(Pair));
  pair->key = strdup(key);
  pair->value = strdup(value);

  return pair;
}

/****
  Use this function to safely destroy a hashtable pair.
 ****/
void destroy_pair(Pair *pair)
{
  if (pair != NULL) {
    free(pair->key);
    free(pair->value);
    free(pair);
  }
}

/****
  djb2 hash function

  Do not modify this!
 ****/
unsigned int hash(char *str, int max)
{
  unsigned long hash = 5381;
  int c;
  unsigned char * u_str = (unsigned char *)str;

  while ((c = *u_str++)) {
    hash = ((hash << 5) + hash) + c;
  }

  return hash % max;
}


/****
  Fill this in.

  All values in storage should be initialized to NULL
  (hint: look up `calloc`)
 ****/
BasicHashTable *create_hash_table(int capacity)
{
  BasicHashTable *ht;

  if (capacity < 1) {
    return NULL;
  }

  ht = calloc(capacity, sizeof(BasicHashTable));
  if (ht == NULL) {
    return NULL;
  }

  ht -> storage = (Pair**)calloc(capacity, sizeof(Pair));
  if (ht->storage == NULL) {
    return (NULL);
  }

  ht->capacity=capacity;

  return ht;
}

/****
  Fill this in.

  If you are overwriting a value with a different key, print a warning.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_insert(BasicHashTable *ht, char *key, char *value)
{
  // if (ht->key != NULL) {
  //   printf("Warning, you are overwriting the previous key value pair");
  //   ht->key[value] = value;
  // }
  // ht->key = value;

  Pair *node;

  if (ht == NULL) {
    return;
  }

  node = malloc(sizeof(Pair));
  if (node == NULL) {
    return;
  }

  node-> key = strdup(key);
  node-> value = strdup(value);
  
  unsigned int i = hash(node->key, ht->capacity);

  if (ht->storage[i] != NULL) {
    printf("Value is being overwritten");
    ht->storage[i] = node;
  }
  else{
  
  ht->storage[i] = node;
  }
  return;

}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(BasicHashTable *ht, char *key)
{
  unsigned int i;


  if (ht == NULL) {
    return;
  }

  i = hash(key, ht->capacity);

    if (ht->storage[i] != NULL){
      destroy_pair(ht->storage[i]);
      ht->storage[i] = NULL;
  }


}

/****
  Fill this in.

  Should return NULL if the key is not found.
 ****/
char *hash_table_retrieve(BasicHashTable *ht, char *key)
{
  char *key_cp;
  unsigned int i;
  Pair *tmp;

  if (ht == NULL) {
    return NULL;
  }

  key_cp = strdup(key);
  i = hash(key, ht->capacity);
  tmp = ht->storage[i];

  for (int j = 0; j<ht->capacity; i++) {
    if (strcmp(tmp->key, key_cp) == 0) {
      free(key_cp);
      return tmp->value;
    }
  }

  free(key_cp);
  if(tmp == NULL) {
    return NULL;
  }

  return tmp->value;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(BasicHashTable *ht)
{
  int i;

  if(ht == NULL) {
    return;
  }

  for (i = 0; i < ht->capacity; i++) {
    if (ht->storage[i] != NULL) {
      free(ht->storage[i]);
    }
  }
  free(ht->storage);
  free(ht);
}


#ifndef TESTING
int main(void)
{
  struct BasicHashTable *ht = create_hash_table(16);

  hash_table_insert(ht, "line", "Here today...\n");

  printf("%s", hash_table_retrieve(ht, "line"));

  hash_table_remove(ht, "line");

  if (hash_table_retrieve(ht, "line") == NULL) {
    printf("...gone tomorrow. (success)\n");
  } else {
    fprintf(stderr, "ERROR: STILL HERE\n");
  }
  

  destroy_hash_table(ht);

  return 0;
}
#endif
