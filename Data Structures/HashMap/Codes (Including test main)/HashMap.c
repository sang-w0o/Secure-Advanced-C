#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HashMap.h"

typedef struct Node {
	void *key;
	void *value;
	int hash; 
	struct Node *next;
}Node;

typedef struct Hashmap {
	Node **buckets;
	size_t count;
	size_t bucketSize;
	HashFunction hashFunction;
	EqualsFunction equalsFunction;
}Hashmap;

Hashmap *hashmapCreate(HashFunction hashFunc, EqualsFunction equalsFunc) {
	if (hashFunc == NULL || equalsFunc == NULL) {
		fprintf(stderr, "hashmapCreate : argument is NULL.\n");
		return NULL;
	}

	Node **buckets = calloc(DEFAULT_BUCKETSIZE, sizeof(Node *));
	if (buckets == NULL) {
		fprintf(stderr, "hashmapCreate : calloc failed.\n");
		return NULL;
	}

	Hashmap *map = calloc(1, sizeof(Hashmap));
	if (map == NULL) {
		fprintf(stderr, "hashmapCreate : calloc failed");
		free(buckets);
		return NULL;
	}

	map->hashFunction = hashFunc;
	map->equalsFunction = equalsFunc;
	map->buckets = buckets;
	map->bucketSize = DEFAULT_BUCKETSIZE;
	return map;
}

void hashmapDestroy(Hashmap *map) {
	if (map == NULL)
		return;
	for (size_t i = 0; i < map->bucketSize; i++) {
		Node *node = map->buckets[i];
		while (node != NULL) {
			Node *next = node->next;
			free(node);
			node = next;
		}
	}
	free(map->buckets);
	free(map);
}

static int hashKey(const Hashmap *map, void *key) {
	int hash = map->hashFunction(key);

	// This hash algorithm is made by Doug Lea
	// to defend against bad hashes.
	hash += ~(hash << 9);
	hash ^= (((unsigned int)hash) >> 14);
	hash += (hash << 4);
	hash ^= (((unsigned int)hash) >> 10);
	return hash;

}

static size_t calculateIndex(size_t bucketSize, int hash) {
	return ((size_t)hash) & (bucketSize - 1);
}

static Node *createNode(void *key, int hash, void *value) {
	Node *node = calloc(1, sizeof(Node));
	if (node == NULL) {
		fprintf(stderr, "createNode : calloc failed.\n");
		return NULL;
	}
	node->key = key;
	node->value = value;
	node->hash = hash;
	return node;
}

static int equalsKey(void *key1, int hash1, void *key2, int hash2, EqualsFunction equalsFunc) {
	if (key1 == NULL || key2 == NULL || equalsFunc == NULL) {
		return 0;
	}
	if (key1 == key2) {
		return 1;
	}
	if (hash1 != hash2) {
		return 0;
	}
	return equalsFunc(key1, key2);
}

static int extendIfNecessary(Hashmap *map) {
	if (map == NULL) {
		fprintf(stderr, "increaseSize : argument is NULL.\n");
		return -1;
	}

	if (map->count <= (map->bucketSize * 3 / 4)) {
		return 0;
	}

	size_t newBucketSize = map->bucketSize * 2;
	if (newBucketSize >= MAX_BUCKETSIZE || map->bucketSize == MAX_BUCKETSIZE) {
		fprintf(stderr, "increaseSize : size overflow.\n");
		return -1;
	}

	Node **newBuckets = NULL;
	newBuckets = calloc(newBucketSize, sizeof(Node *));
	if (newBuckets == NULL) {
		fprintf(stderr, "increaseSize : realloc failed.\n");
		return -1;
	}
	// hashing을 다시 해야 하므로 realloc을 사용하지 않는다.
	for (size_t i = 0; i < map->bucketSize; i++) {
		Node *cur = map->buckets[i];
		while (cur != NULL) {
			Node *next = cur->next;
			size_t index = calculateIndex(newBucketSize, cur->hash);
			cur->next = newBuckets[index];
			newBuckets[index] = cur;
			cur = next;
		}
	}

	free(map->buckets);
	map->buckets = newBuckets;
	map->bucketSize = newBucketSize;
	return 0;
}
void *hashmapPut(Hashmap *map, void *key, void *value) {
	if (map == NULL || key == NULL || value == NULL) {
		fprintf(stderr, "hashmapPut : argument is NULL.\n");
		return NULL;
	}

	extendIfNecessary(map);

	int hash = hashKey(map, key);
	size_t index = calculateIndex(map->bucketSize, hash);

	Node **ptr = &(map->buckets[index]);
	while (1) {
		Node *cur = *ptr;

		if (cur == NULL) {
			Node *node = createNode(key, hash, value);
			if (node == NULL) {
				fprintf(stderr, "hashmapPut : createNode failed.\n");
				return NULL;
			}
			*ptr = node;
			map->count++;
			return NULL;
		}

		if (equalsKey(cur->key, cur->hash, key, hash, map->equalsFunction) == 1) {
			void *oldValue = cur->value;
			cur->value = value;
			return oldValue;
		}
		ptr = &(cur->next);
	}

}

void *hashmapGet(const Hashmap *map, void *key) {
	if (map == NULL || key == NULL) {
		fprintf(stderr, "hashmapGet : argument is NULL.\n");
		return NULL;
	}

	int hash = hashKey(map, key);
	int index = calculateIndex(map->bucketSize, hash);

	for (Node *p = map->buckets[index]; p != NULL; p = p->next) {
		if (map->equalsFunction(p->key, key))
			return p->value;
	}
	return NULL;
}

void *hashmapRemove(Hashmap *map, void *key) {
	if (map == NULL || key == NULL) {
		fprintf(stderr, "hashmapRemove : argument is NULL.\n");
		return NULL;
	}

	int hash = hashKey(map, key);
	int index = calculateIndex(map->bucketSize, hash);

	Node **ptr = &(map->buckets[index]);
	while (1) {
		Node *cur = *ptr;
		if (cur == NULL)
			break;

		if (map->equalsFunction(cur->key, key)) {
			void *oldValue = cur->value;
			*ptr = cur->next;
			free(cur);
			--map->count;
			return oldValue;
		}
		ptr = &(cur->next);
	}
	return NULL;
}

void hashmapDisplay(const Hashmap *map, const char *(*displayFunc)(const void *)) {
	if (map == NULL || displayFunc == NULL) {
		return;
	}
	system("cls");

	size_t bucketSize = map->bucketSize;
	for (size_t i = 0; i < bucketSize; i++) {
		printf("bucket[%2lu]", i);
		for (Node *cur = map->buckets[i]; cur != NULL; cur = cur->next) {
			printf("->[%s]", displayFunc(cur->value));
		}
		printf("\n");
	}
	getchar();
}

int hashmapForEach(Hashmap *map, int (*userFunc)(void *, void *)) {
	if (map == NULL || userFunc == NULL) {
		fprintf(stderr, "hashmapForEach : argument is NULL.\n");
		return -1;
	}

	size_t bucketSize = map->bucketSize;
	for (size_t i = 0; i < bucketSize; i++) {
		for (Node *cur = map->buckets[i]; cur != NULL; cur = cur->next) {
			if (userFunc(cur->key, cur->value) == 0) {
				return 0;
			}
		}
	}
	return 0;
}