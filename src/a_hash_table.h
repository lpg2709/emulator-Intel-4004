#ifndef A_HASH_TABLE_H
#define A_HASH_TABLE_H

#include <inttypes.h>
#include <stdbool.h>

// ('a' to 'z') + ('A' to 'Z') + ('0' to '9') = 62
// Max 3 chars for one label
#define HASH_TABLE_MAX_ENTRIES 238328

typedef struct {
	bool declared;
	uint16_t address;
} Entry;

typedef struct {
	uint32_t count;
	Entry *entries;
} HashTable;

bool hash_table_init(HashTable *table);
void hash_table_deinit(HashTable *table);
bool hash_table_get(HashTable *table, char *key, uint16_t *address);
bool hash_table_set(HashTable *table, char *key, uint16_t address);

#endif  // A_HASH_TABLE_H

