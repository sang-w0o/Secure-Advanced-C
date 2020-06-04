#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "HashMap.h"

// 동명이인이 없다는 가정하에 진행한다.
// key는 name, value는 Person이라 하자.
typedef struct Person {
	char name[32];
	int age;
}Person;

// key에 대한 hash값을 만들어내는 사용자 정의 함수
int myHash(void *key) {
	return strlen((const char *)key);
}

// 데이터를 비교하는 사용자 정의 함수
int myEquals(void *key1, void *key2) {
	if (key1 == NULL || key2 == NULL) {
		fprintf(stderr, "myEquals : argument is NULL");
		return 0;
	}
	return strcmp((const char *)key1, (const char *)key2) == 0;
}

const char *toPerson(const void *data) {
	static char buf[32];
	const Person *person = (const Person *)data;
	sprintf(buf, "%s(%d)", person->name, person->age);
	return (const char *)buf;
}

int increaseAge(void *key, void *value) {
	if (key == NULL || value == NULL) {
		return 0;
	}
	Person *p = value;
	++p->age;
	return 1;
}

int main() {

	Person people[4] = { {"A", 10}, {"BB", 20}, {"CCC", 30}, {"D", 40} };
	Hashmap *map = hashmapCreate(myHash, myEquals);
	hashmapDisplay(map, toPerson);

	for (int i = 0; i < 4; i++) {
		hashmapPut(map, people[i].name, &people[i]);
		hashmapDisplay(map, toPerson);
	}

	printf("\n\n===hashmapGet test====\n\n");
	for (int i = 0; i < 4; i++) {
		const Person *p = hashmapGet(map, people[i].name);
		if (p) {
			printf("key : %s, value : %d\n", people[i].name, p->age);
		}
	}

	printf("\n\n===hashmapRemove test===\n\n");
	printf("remove data whose key is A\n\n");
	Person *removeData = hashmapRemove(map, "A");
	printf("removed Data : %s(%d)\n", removeData->name, removeData->age);
	getchar();

	hashmapDisplay(map, toPerson);

	printf("increase every person's age by 1");
	getchar();

	hashmapForEach(map, increaseAge);
	hashmapDisplay(map, toPerson);

	hashmapDestroy(map);
	return 0;
}