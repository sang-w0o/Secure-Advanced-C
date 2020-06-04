#ifndef _HASHMAP_H_
#define _HASHMAP_H_

#define DEFAULT_BUCKETSIZE (1)
#define MAX_BUCKETSIZE (4096)

typedef struct Node Node;
typedef struct Hashmap Hashmap;
typedef int (*HashFunction)(void *key);
typedef int (*EqualsFunction)(void *key1, void *key2);

Hashmap *hashmapCreate(HashFunction hashFunc, EqualsFunction equalsFunc);
void hashmapDestroy(Hashmap *map);
void *hashmapPut(Hashmap *map, void *key, void *value);
void *hashmapGet(const Hashmap *map, void *key);
void *hashmapRemove(Hashmap *map, void *key);
void hashmapDisplay(const Hashmap *map, const char *(*displayFunc)(const void *));
int hashmapForEach(Hashmap *map, int (*userFunc)(void *, void *));

#endif