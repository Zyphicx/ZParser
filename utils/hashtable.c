#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

/*Simple hashtable used for grammartable and more*/

HashTable *table_new(size_t size){
	HashTable *table = (HashTable *)malloc(sizeof(HashTable));
	table->entries = malloc(size * sizeof(HashEntry *));
	table->size = size;
	HashEntry **entry;
	for(entry = table->entries; entry < (table->entries + size); entry++){
		*entry = NULL;
	}
	return table;
}

void table_free(HashTable *table){
	HashEntry **entry;

	for(entry = table->entries; entry < (table->entries + table->size); entry++){
		while(*entry != NULL){
			//printf("Thing - %s\n", (char *)((*entry)->value));
			HashEntry *temp = (*entry)->next;
			entry_free(*entry);
			*entry = temp;
		}
	}
	free(table->entries);
	free(table);
}

int hash(char *value, int size){
	int hash = 0;

	while(*value){
		hash >>= 3;
		hash ^= ((*value - 1) * 23);
		++value;
	}

	return hash % size;
}

void table_insert(HashTable *table, char *key, void *value){
	HashEntry *entry = entry_new(key, value);

	int hashval = hash(entry->key, table->size);

	if(table->entries[hashval] == NULL){
		table->entries[hashval] = entry;
	}else{
		HashEntry *e;
		for(e = table->entries[hashval]; (e->next != NULL) && strcmp(e->key, key); e = e->next) //strcmp if equal?
			;
		e->next = entry;
	}
}

void *table_lookup(HashTable *table, char *s){
	int hashval = hash(s, table->size);
	HashEntry *entry = NULL;

	if((entry = table->entries[hashval]) != NULL){
		while(strcmp(entry->key, s)) {
			if(entry->next == NULL){
				return NULL;
			}
			else
				entry = entry->next;
		}
	}

	return entry->value;
}

void *table_drop(HashTable *table, HashEntry *entry){
	int hashval = hash(entry->key, table->size);
	HashEntry *e;

	if((e = table->entries[hashval]) != NULL){
		if(entry->next == NULL){
			if(e == entry){
				table->entries[hashval] = NULL;
			}
			while(e->next != NULL){
				if(e->next == entry){
					e->next = NULL;
				}else{
					e = e->next;
				}
			}
		}else{
			if(e == entry){
				table->entries[hashval] = entry->next;
			}
			while(e->next != NULL){
				if(e->next == entry){
					e->next = entry->next;
				}else{
					e = e->next;
				}
			}
		}
	}else{
		return NULL;
	}

	return entry->value;
}

HashEntry *entry_new(char *key, void *value){
	HashEntry *entry = (HashEntry *)malloc(sizeof(HashEntry));

	entry->key = malloc(strlen(key) + 1);
	strcpy(entry->key, key);

	entry->value = value;

	entry->next = NULL;
	return entry;
}

void entry_free(HashEntry *entry){
	if(entry->key != NULL)
		free(entry->key);

	/* if(entry->value != NULL)
		free(entry->value);*/

	free(entry);
}

void entry_setval(HashEntry *entry, void *value){
	if(entry->value != NULL)
		free(entry->value);
	entry->value = malloc(sizeof(value));
	entry->value = value;
}