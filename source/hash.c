#include "include/hash.h"

void init_bucket();

// struct bucket *hashArray[SIZE];
// struct bucket *dummyItem;
// struct bucket *item;

int hashCode(int key, int size) {
    return key % size;
}

void *bucket_search(int key, Bucket *bucket_array[], int size) {
    int hashIndex = hashCode(key, size);

    while (bucket_array[hashIndex] != NULL) {
        if (bucket_array[hashIndex]->key == key) {
            return bucket_array[hashIndex]->data;
        }
        ++hashIndex;
        hashIndex %= size;
    }
    return NULL;
}

void bucket_insert(Bucket *bucket, Bucket *bucket_array[], int key, void *data) {
    bucket->data = data;
    bucket->key = key;

    int hashIndex = hashCode(key, bucket->capacity);

    while(bucket_array[hashIndex] != NULL && bucket_array[hashIndex]->key != -1) {
        ++hashIndex;
        hashIndex %= bucket->capacity;
    }
    bucket_array[hashIndex] = bucket;
}
