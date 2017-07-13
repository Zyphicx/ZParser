#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdlib.h>

struct hash_entry{
	char *key;
	void *value;
	struct hash_entry *next;
};

struct hash_table{
	size_t size;
	struct hash_entry **entries;
};

typedef struct hash_entry HashEntry;
typedef struct hash_table HashTable;

int hash(char *value, int size);

HashTable *table_new(size_t size);
void table_free(HashTable *table);
void table_insert(HashTable *table, char *key, void *value);
void *table_drop(HashTable *table, HashEntry *entry);
void *table_lookup(HashTable *table, char *s);

HashEntry *entry_new(char *key, void *value);
void entry_free(HashEntry *entry);
void entry_setval(HashEntry *entry, void *value);

#endif