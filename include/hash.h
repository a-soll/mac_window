#ifndef HASH_H
#define HASH_H

#include "include/base.h"

typedef struct Bucket {
    void *data;
    int key;
    int capacity;
} Bucket;

void *bucket_search(int key, Bucket *bucket_array[], int size);

void bucket_insert(Bucket *bucket, Bucket *bucket_array[], int key, void *data);

#endif /* HASH_H */
