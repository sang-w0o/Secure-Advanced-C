#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "BinarySeacrhTree.h"

typedef struct Person {
	char name[32];
	int age;
}Person;

size_t sizeOfPerson() {
	return sizeof(Person);
}

const char *toPerson(const void *data) {
	static char buf[32];
	const Person *person = (const Person *)data;
	sprintf(buf, "%s(%d)", person->name, person->age);
	return (const char *)buf;
}

int comparePerson(void *data1, void *data2) {
	const Person *p1 = data1;
	const Person *p2 = data2;
	return (p1->age - p2->age);
}

int main() {

	// This test code uses Person's age as the Node's key.

	BST *bst = bstCreate(toPerson, comparePerson, sizeOfPerson);

	Person people[8] = { {"FOUR", 40}, {"TWO", 20}, {"ONE", 10}, {"THREE", 30}, {"SIX",60}, {"FIVE", 50}, {"SEVEN", 70}, {"EIGHT", 80} };
	for (int i = 0; i < 8; i++) {
		bstInsert(bst, people + i);
	}

	printf("\n\n");

	printf("========ordering test========\n\n");
	preorder(bst);
	inorder(bst);
	postorder(bst);

	printf("========bstGet() test========\n\n");
	for (int i = 0; i < 8; i++) {
		void *data = bstGet(bst, people + i);
		if (data != NULL)
			printf("%s\n", toPerson(data));
	}

	printf("========bstRemove() test========\n\n");

	for (int i = 0; i < 8; i++) {
		int num;
		printf("input age to remove : ");
		scanf("%d", &num);

		Person data = { "TMP", num };

		Person *p = bstRemove(bst, &data);
		if (p == NULL) {
			printf("bstRemove failed.\n");
		}
		else {
			printf("Removed data : %s\n", toPerson(p));
		}

		inorder(bst);
	}
	return 0;
}