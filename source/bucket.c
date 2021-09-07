#include "include/bucket.h"

Bucket *init_bucket(int key, void *data) {
    Bucket *bucket = (Bucket *)malloc(sizeof(Bucket));
    bucket->key = key;
    bucket->data = data;
    return bucket;
}

int hashCode(int key, int size) {
    return key % size;
}

bool valid_bucket(Table *table, int index) {
    bool ret = false;

    if (table->buckets[index] != NULL && table->buckets[index]->data != NULL) {
        ret = true;
    }
    return ret;
}

void bucket_free(Bucket *bucket) {
    free(bucket->data);
    free(bucket);
}

void table_free(Table *table) {
    for (int i = 0; i < table->size; i++) {
        if (table->buckets[i] != NULL) {
            if (table->release != NULL) {
                table->release(table->buckets[i]->data);
            }
            bucket_free(table->buckets[i]);
        }
    }
    free(table->buckets);
    free(table);
}

void table_delete_item(Table *table, int key) {
    int index = hashCode(key, table->size);

    while (table->buckets[index] != NULL) {
        if (table->buckets[index]->key == key) {
            if (table->release != NULL) {
                table->release(table->buckets[index]->data);
            }
            bucket_free(table->buckets[index]);
            table->buckets[index] = NULL;
            table->count--;
            break;
        }
        ++index;
        index %= table->size;
    }
}

void table_resize(Table *table) {
    table->size = table->size * 2;
    table->buckets = (Bucket **)realloc(table->buckets, (sizeof(Bucket *)) * table->size);
    for (int i = table->count; i < table->size; i++) {
        table->buckets[i] = NULL;
    }
}

void *table_search(Table *table, int key) {
    int index = hashCode(key, table->size);

    while (table->buckets[index] != NULL) {
        if (table->buckets[index]->key == key) {
            return table->buckets[index]->data;
        }
        ++index;
        index %= table->size;
    }
    return NULL;
}

Table *table_init(int size) {
    Table *table = (Table *)malloc(sizeof(Table));
    table->size = size;
    table->count = 0;
    table->buckets = (Bucket **)calloc(table->size, sizeof(Bucket *));
    for (int i = 0; i < table->size; i++) {
        table->buckets[i] = NULL;
    }
    return table;
}

void table_insert(Table *table, int key, void *data) {
    int index = hashCode(key, table->size);
    Bucket *bucket = init_bucket(key, data);

    if (table_search(table, key) == NULL) {
        if (table->count == table->size) {
            table_resize(table);
        }
        while (table->buckets[index] != NULL) {
            ++index;
            index %= table->size;
        }
        table->buckets[index] = bucket;
        table->count++;
    }
}
