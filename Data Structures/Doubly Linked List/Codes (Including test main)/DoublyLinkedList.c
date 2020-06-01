#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "DoublyLinkedList.h"


typedef struct Node {
	void *data;
	struct Node *next;
	struct Node *prev;
}Node;

typedef struct List {
	Node *head;
	Node *tail;
	int count;
	FreeFunction *freeFunction;
}List;

static int listCount(const List *list) {
	if (list == NULL) {
		fprintf(stderr, "listCount:list is NULL.\n");
		return -1;
	}
	return list->count;
}

static Node *getNodeAtIndex(const List *list, int index) {
	if (list == NULL) {
		fprintf(stderr, "getNodeAtIndex:list is NULL.\n");
		return NULL;
	}

	Node *node;
	if (index <= (list->count) >> 1) {
		node = list->head->next;
		for (int i = 0; i < index; i++) {
			node = node->next;
		}
	}

	else {
		node = list->tail->prev;
		for (int i = list->count - 1; i > index; i--) {
			node = node->prev;
		}
	}
	return node;
}
List *listInitialize(FreeFunction freeFunction) {

	List *list = calloc(1, sizeof(List));
	if (list == NULL) {
		fprintf(stderr, "listInitialize:malloc failed.\n");
		return NULL;
	}

	Node *head = malloc(sizeof(Node));
	if (head == NULL) {
		fprintf(stderr, "listInitialize:malloc failed.\n");
		return NULL;
	}

	Node *tail = malloc(sizeof(Node));
	if (tail == NULL) {
		fprintf(stderr, "listInitialize:malloc failed.\n");
		free(head);
		return NULL;
	}

	list->head = head;
	list->tail = tail;
	list->head->next = list->tail;
	list->head->prev = list->head;
	list->tail->next = list->tail;
	list->tail->prev = list->head;
	list->freeFunction = freeFunction;

	return list;
}

int listFinalize(List *list) {
	if (list == NULL) {
		fprintf(stderr, "listFinalize:list is NULL.\n");
		return -1;
	}

	if (list->head == NULL || list->tail == NULL) {
		fprintf(stderr, "listFinalize:head or tail is NULL.\n");
		return -1;
	}

	while (list->head->next != list->tail) {
		Node *target = list->head->next;
		list->head->next = target->next;
		if (list->freeFunction) {
			list->freeFunction(target->data);
		}
		free(target);
	}
	free(list->head);
	free(list->tail);
	free(list);
	return 0;
}

void listDisplay(const List *list, const char *(*displayFunc)(const void *)) {
	if (list == NULL) {
		fprintf(stderr, "listDisplay:list is NULL.\n");
		return;
	}

	system("cls");
	printf("[head]");
	for (Node *node = list->head->next; node != list->tail; node = node->next)
		printf("<->[%s]", displayFunc(node->data));
	printf("<->[tail]");
	getchar();
}

void listReverseDisplay(const List *list, const char *(*displayFunc)(const void *)) {
	if (list == NULL) {
		fprintf(stderr, "listDisplay:list is NULL.\n");
		return;
	}

	system("cls");
	printf("[tail]");
	for (Node *node = list->tail->prev; node != list->head; node = node->prev)
		printf("<->[%s]", displayFunc(node->data));
	printf("<->[head]");
	getchar();
}

int listAddBack(List *list, void *data) {
	if (list == NULL) {
		fprintf(stderr, "listAdd:list is NULL.\n");
		return -1;
	}

	Node *node = malloc(sizeof(Node));
	if (node == NULL) {
		fprintf(stderr, "listAdd:malloc failed.\n");
		return -1;
	}
	node->data = data;
	node->next = list->tail;
	node->prev = list->tail->prev;

	list->tail->prev->next = node;
	list->tail->prev = node;
	++(list->count);

	return 0;
}

int listAddFront(List *list, void *data) {
	if (list == NULL) {
		fprintf(stderr, "listAddFront:list is NULL.\n");
		return -1;
	}

	Node *node = malloc(sizeof(Node));
	if (node == NULL) {
		fprintf(stderr, "listAddFront:malloc failed.\n");
		return -1;
	}

	node->data = data;
	node->prev = list->head;
	node->next = list->head->next;
	list->head->next->prev = node;
	list->head->next = node;
	++(list->count);
	return 0;
}

void *listRemove(List *list, int index) {
	if (list == NULL) {
		fprintf(stderr, "listRemove:list is NULL.\n");
		return NULL;
	}

	if (list->count == 0) {
		fprintf(stderr, "listRemove:list is empty.\n");
		return NULL;
	}

	if (index < 0 || index >= list->count) {
		fprintf(stderr, "listRemove:index is out of bound.\n");
		return NULL;
	}

	Node *delNode = getNodeAtIndex(list, index);

	delNode->prev->next = delNode->next;
	delNode->next->prev = delNode->prev;
	void *outData = delNode->data;
	free(delNode);
	--(list->count);
	return outData;
}

void *listRemoveHead(List *list) {
	if (list == NULL) {
		fprintf(stderr, "listRemoveHead:list is NULL.\n");
		return NULL;
	}

	if (list->count == 0) {
		fprintf(stderr, "listRemoveHead:list is empty.\n");
		return NULL;
	}

	Node *delNode = list->head->next;
	list->head->next = delNode->next;
	delNode->next->prev = list->head->next;
	void *outData = delNode->data;
	free(delNode);
	--(list->count);
	return outData;
}

void *listRemoveTail(List *list) {
	if (list == NULL) {
		fprintf(stderr, "listRemoveTail:list is NULL.\n");
		return NULL;
	}

	if (list->count == 0) {
		fprintf(stderr, "listRemoveTail:list is empty.\n");
		return NULL;
	}

	Node *delNode = list->tail->prev;
	list->tail->prev = delNode->prev;
	delNode->prev->next = list->tail;
	void *outData = delNode->data;
	free(delNode);
	--(list->count);
	return outData;
}

void *listGet(const List *list, int index) {
	if (list == NULL) {
		fprintf(stderr, "listGet:list is NULL.\n");
		return NULL;
	}

	if (list->count == 0) {
		fprintf(stderr, "listGet:list is empty.\n");
		return NULL;
	}

	if (index < 0 || index >= list->count) {
		fprintf(stderr, "listGet:index is out of bound.\n");
		return NULL;
	}

	Node *node = getNodeAtIndex(list, index);

	void *outData = node->data;
	return outData;
}

void *listSet(List *list, int index, void *newData) {
	if (list == NULL) {
		fprintf(stderr, "listSet:list is NULL.\n");
		return NULL;
	}

	if (list->count == 0) {
		fprintf(stderr, "listSet:list is empty.\n");
		return NULL;
	}

	if (index < 0 || index >= list->count) {
		fprintf(stderr, "listSet:index is out of bound.\n");
		return NULL;
	}

	Node *node = getNodeAtIndex(list, index);
	void *oldData = node->data;
	node->data = newData;
	return oldData;
}

int listInsert(List *list, int index, void *data) {
	if (list == NULL) {
		fprintf(stderr, "listInsert:list is NULL.\n");
		return -1;
	}

	if (list->count == 0) {
		fprintf(stderr, "listInsert:list is empty.\n");
		return -1;
	}

	if (index < 0 || index >= list->count) {
		fprintf(stderr, "listInsert:index is out of bound.\n");
		return -1;
	}


	Node *node = malloc(sizeof(Node));
	if (node == NULL) {
		fprintf(stderr, "listInsert:malloc failed.\n");
		return -1;
	}

	node->data = data;

	Node *prev = getNodeAtIndex(list, index - 1);
	node->next = prev->next;
	node->prev = prev;
	prev->next->prev = node;
	prev->next = node;

	++(list->count);

	return 0;
}