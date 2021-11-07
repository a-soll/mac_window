#ifndef HASH_H
#define HASH_H

#include "include/base.h"

typedef struct Bucket {
    void *data;
    int key;
    struct Bucket *next;
} Bucket;

typedef struct Table {
    int size;
    int count;
    struct Bucket **buckets;
    void (*release)(void *);
    struct Bucket *first_bucket;
    int prev_index;
} Table;

// used when iterating through a table's buckets
bool valid_bucket(Table *table, int index);
void *table_search(Table *table, int key);
void table_insert(Table *table, int key, void *data);
void table_delete_item(Table *table, int key);
void table_free(Table *table);
Table *table_init(int size);
void *table_iterate(Table *table, bool first_iterate);

#endif /* HASH_H */
