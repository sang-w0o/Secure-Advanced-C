#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "SinglyLinkedList.h"

typedef struct {
	char name[32];
	int age;
}Person;

const char *toPerson(const void *data) {
	static char buf[32];
	const Person *person = (const Person *)data;
	sprintf(buf, "%s(%d)", person->name, person->age);
	return (const char *)buf;
}

int main() {


	List *list = listInitialize();

	Person people[5] = {
		{"A", 11}, {"B", 22}, {"C", 33}, {"D", 44}, {"E", 55} };

	listDisplay(list, toPerson);

	for (int i = 0; i < 5; i++) {
		listAdd(list, people + i);
	}
	listDisplay(list, toPerson);

	Person *data;
	listRemove(list, 4, &data), listDisplay(list, toPerson);
	listRemove(list, 2, &data), listDisplay(list, toPerson);
	listRemove(list, 0, &data), listDisplay(list, toPerson);

	listFinalize(list);
	return 0;
}