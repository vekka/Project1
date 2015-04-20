#ifndef MAP_H
#define MAP_H

#include "model.h"

typedef int (*comparefunc_t)(vertex_t, vertex_t);
typedef unsigned long (*hashfunc_t)(vertex_t);

typedef struct mapentry
{
    vertex_t key;
    void *value;
    struct mapentry *next;
} mapentry_t;

typedef struct
{
    comparefunc_t cmpfunc;
    hashfunc_t hashfunc;
    int size;
    mapentry_t **buckets;
    int numbuckets;
} hashmap_t;

void Hashmap_Init(hashmap_t *map, comparefunc_t cmpfunc, hashfunc_t hashfunc);
void Hashmap_FreeBuckets(hashmap_t *map);
void Hashmap_Put(hashmap_t *map, vertex_t key, void *value);
int Hashmap_HasKey(hashmap_t *map, vertex_t key);
void *Hashmap_Get(hashmap_t *map, vertex_t key);

#endif
