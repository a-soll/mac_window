#ifndef HASH_H
#define HASH_H

#include "include/base.h"

typedef struct Bucket {
    void *data;
    int key;
} Bucket;

typedef struct Table {
    int size;
    int count;
    struct Bucket **buckets;
} Table;

// used when iterating through a table's buckets
bool valid_bucket(Table *table, int index);

void *table_search(Table *table, int key);

void table_insert(Table *table, int key, void *data);

void table_delete_item(Table *table, int key);

void table_free(Table *table);

Table *table_init(int size);

#endif /* HASH_H */
