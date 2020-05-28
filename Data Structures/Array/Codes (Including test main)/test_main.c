#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "Array.h"

typedef struct {
	char name[32];
	int age;
} Person;

const char *toPerson(const void *data) {
	static char buf[32];
	const Person *person = (const Person *)data;
	sprintf(buf, "%s(%d)", person->name, person->age);
	return (const char *)buf;
}

int main() {
	Array *arr = arrayCreate();
	Person people[5] = {
		{"A", 11}, {"B", 22}, {"C", 33}, {"D", 44}, {"E", 55} };

	arrayDisplay(arr, toPerson);
	for (int i = 0; i < 4; i++) {
		arrayAdd(arr, people + i);
		arrayDisplay(arr, toPerson);
	}

	arrayInsert(arr, 0, people + 4);
	arrayDisplay(arr, toPerson);

	arrayDestroy(arr);
}