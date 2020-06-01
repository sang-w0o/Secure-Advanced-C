#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DoublyLinkedList.h"

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

// test code 1 - Node's data using Stack memory.
int main() {

	// 스택 메모리를 사용하는 경우, FreeFunction함수가 필요 없다.
	List *list = listInitialize(NULL);

	Person people[5] = { {"A",1},{"B",2},{"C",3},{"D",4},{"E",5} };

	for (int i = 0; i < 5; i++) {
		listAddBack(list, people + i);
		listDisplay(list, toPerson);
	}

	listRemove(list, 3);
	listDisplay(list, toPerson);
	listReverseDisplay(list, toPerson);

	listFinalize(list);
	return 0;
}


// test code 2 - Node's data using Heap memory.
void freePerson(void *p) {
	Person *p1 = p;
	free(p1);
}


int main() {
	
	List *list = listInitialize(freePerson);

	// 동적 할당된 데이터를 사용하는 경우, 사용자 정의 해제 함수가 필요하다.
	// 이 예시 코드에서는 freePerson함수를 만들어 사용했다.

	//listAdd test code===========
	Person *p1 = malloc(sizeof(Person));
	p1->age = 1;
	strcpy(p1->name, "A");
	listAddBack(list, p1);
	listDisplay(list, toPerson);

	Person *p2 = malloc(sizeof(Person));
	p2->age = 2;
	strcpy(p2->name, "B");
	listAddBack(list, p2);
	listDisplay(list, toPerson);

	Person *p3 = malloc(sizeof(Person));
	p3->age = 3;
	strcpy(p3->name, "C");
	listAddBack(list, p3);
	listDisplay(list, toPerson);

	Person *p4 = malloc(sizeof(Person));
	p4->age = 4;
	strcpy(p4->name, "D");
	listAddBack(list, p4);
	listDisplay(list, toPerson);

	Person *p5 = malloc(sizeof(Person));
	p5->age = 5;
	strcpy(p5->name, "A");
	listAddBack(list, p5);
	listDisplay(list, toPerson);

	//listInsert testcode
	Person *person = malloc(sizeof(Person));
	person->age = 22;
	strcpy(person->name, "SECOND");
	listInsert(list, 1, person);
	listDisplay(list, toPerson);

	//listRemove testcode
	listRemove(list, 1);
	listDisplay(list, toPerson);

	listFinalize(list);
	
	return 0;
}
