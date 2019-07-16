#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  Hash table key/value pair with linked list pointer.

  Note that an instance of `LinkedPair` is also a node in a linked list.
  More specifically, the `next` field is a pointer pointing to the the 
  next `LinkedPair` in the list of `LinkedPair` nodes. 
 */
typedef struct LinkedPair {
  char *key;
  char *value;
  struct LinkedPair *next;
} LinkedPair;

/*
  Hash table with linked pairs.
 */
typedef struct HashTable {
  int capacity;
  LinkedPair **storage;
} HashTable;

/*
  Create a key/value linked pair to be stored in the hash table.
 */
LinkedPair *create_pair(char *key, char *value)
{
  LinkedPair *pair = malloc(sizeof(LinkedPair));
  pair->key = strdup(key);
  pair->value = strdup(value);
  pair->next = NULL;

  return pair;
}

/*
  Use this function to safely destroy a hashtable pair.
 */
void destroy_pair(LinkedPair *pair)
{
  if (pair != NULL) {
    free(pair->key);
    free(pair->value);
    free(pair);
  }
}

/*
  djb2 hash function

  Do not modify this!
 */
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

/*
  Fill this in.

  All values in storage should be initialized to NULL
 */
HashTable *create_hash_table(int capacity)
{
  HashTable *ht;

  if (capacity < 1) {
    return NULL;
  }

  ht = malloc(sizeof(HashTable));
  if (ht == NULL) {
    return NULL;
  }

  ht->storage = calloc(capacity, sizeof(LinkedPair *));
  if(ht->storage == NULL) {
    return (NULL);
  }

  ht-> capacity = capacity;

  return ht;
}

/*
  Fill this in.

  Inserting values to the same index with different keys should be
  added to the corresponding LinkedPair list.

  Inserting values to the same index with existing keys can overwrite
  the value in th existing LinkedPair list.
 */
void hash_table_insert(HashTable *ht, char *key, char *value)
{
  LinkedPair *node = create_pair(key, value);
  

  if(ht == NULL) {
    return;
  }
  if(node == NULL) {
    return;
  }

  unsigned int i = hash(node->key, ht->capacity);
  LinkedPair *tmp = ht->storage[i];

  if (ht->storage[i] != NULL) {
    tmp = ht->storage[i];
    while(tmp != NULL) {
      if (strcmp(tmp->key, node->key) == 0) {
        break;
      }
      tmp = tmp->next;
    }
    if (tmp == NULL) {
      node->next = ht->storage[i];
      ht->storage[i] = node;
    } else {
      free(tmp->value);
      tmp->value = strdup(node->value);
      destroy_pair(node);
    }
  } else {
    node->next = NULL;
    ht->storage[i] = node;
  }
  return;

}

/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys and remove matching LinkedPairs safely.

  Don't forget to free any malloc'ed memory!
 */
void hash_table_remove(HashTable *ht, char *key)
{
  unsigned int i;
  LinkedPair *tmp;

  if (ht == NULL) {
    return;
  }

  i = hash(key, ht-> capacity);

  for (int j = 0; j < ht->capacity; j++) {
    if (ht->storage[i] != NULL) {
      while(ht->storage[i] != NULL) {
        tmp = ht->storage[i]->next;
        destroy_pair(ht->storage[i]);
        ht->storage[i] = tmp;
      }
      destroy_pair(ht->storage[i]);
    }
  }


}

/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys.

  Return NULL if the key is not found.
 */
char *hash_table_retrieve(HashTable *ht, char *key)
{
  char *key_cp;
  unsigned int i;
  LinkedPair *tmp;

  if(ht == NULL) {
    return NULL;
  }
  key_cp = strdup(key);
  i = hash(key, ht->capacity);
  tmp = ht->storage[i];

  while (tmp != NULL) {
    if (strcmp(tmp->key, key_cp) == 0) {
      break;
    }
    tmp = tmp->next;
  }
  free(key_cp);

  if(tmp == NULL) {
    return NULL;
  }

  return tmp->value;
}

/*
  Fill this in.

  Don't forget to free any malloc'ed memory!
 */
void destroy_hash_table(HashTable *ht)
{

  int i;

  if(ht == NULL) {
    return;
  }

  for (i = 0; i<ht->capacity; i++) {
    if (ht->storage[i] != NULL) {
      destroy_pair(ht->storage[i]);
    }
  }
  free(ht->storage);
  free(ht);

}

/*
  Fill this in.

  Should create a new hash table with double the capacity
  of the original and copy all elements into the new hash table.

  Don't forget to free any malloc'ed memory!
 */
HashTable *hash_table_resize(HashTable *ht)
{
  HashTable *new_ht;

  new_ht = create_hash_table(ht->capacity*2);

  new_ht->capacity = ht->capacity*2;

  LinkedPair *tmp;
  int i;

  for (i = 0; i< ht->capacity; i++) {
    tmp = ht->storage[i];

    while(tmp != NULL) {
      hash_table_insert(new_ht, tmp->key, tmp->value);
      tmp = tmp->next;
    }
  }

  destroy_hash_table(ht);

  return new_ht;
}


#ifndef TESTING
int main(void)
{
  struct HashTable *ht = create_hash_table(2);

  hash_table_insert(ht, "line_1", "Tiny hash table\n");
  hash_table_insert(ht, "line_2", "Filled beyond capacity\n");
  hash_table_insert(ht, "line_3", "Linked list saves the day!\n");

  printf("%s", hash_table_retrieve(ht, "line_1"));
  printf("%s", hash_table_retrieve(ht, "line_2"));
  printf("%s", hash_table_retrieve(ht, "line_3"));

  int old_capacity = ht->capacity;
  ht = hash_table_resize(ht);
  int new_capacity = ht->capacity;

  printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  destroy_hash_table(ht);

  return 0;
}
#endif
