#include "a_hash_table.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Hash algoritim FNV-1a http://www.isthe.com/chongo/tech/comp/fnv
static uint32_t hash_string(char *key) {
	uint32_t hash = 2166136261u;
	for(int i = 0; i < 3; i++) {
		hash ^= (uint8_t) key[i];
		hash *= 16777619;
	}
	return hash;
}

bool hash_table_init(HashTable *table) {
	table->count = 0;
	if(table->entries == NULL){
		table->entries = (Entry*) calloc(HASH_TABLE_MAX_ENTRIES, sizeof(Entry));
		if(table->entries == NULL) {
			fprintf(stderr, "Fail to alloc memory to HashTable!\n");
			return false;
		}
	}
	return true;
}

void hash_table_deinit(HashTable *table) {
	table->count = 0;
	free(table->entries);
}

bool hash_table_get(HashTable *table, char *key, uint16_t *address) {
	uint32_t index = hash_string(key) % HASH_TABLE_MAX_ENTRIES;
	if(index > HASH_TABLE_MAX_ENTRIES) {
		return false;
	}
	Entry *entry = &table->entries[index];
	if(entry->declared) {
		*address = entry->address;
		return true;
	}
	return false;

}

bool hash_table_set(HashTable *table, char *key, uint16_t address) {
	if(table->count > HASH_TABLE_MAX_ENTRIES) {
		fprintf(stderr, "No space left on this HashTable!\n");
		return false;
	}
	uint32_t index = hash_string(key) % HASH_TABLE_MAX_ENTRIES;
	Entry *entry = &table->entries[index];
	if(!entry->declared) {
		table->entries[index].declared = true;
		table->entries[index].address = address;
		table->count++;
		return true;
	}
	return false;
}
