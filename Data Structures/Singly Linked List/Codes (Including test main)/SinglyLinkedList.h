#ifndef _SINGLYLINKEDLIST_H_
#define _SINGLYLINKEDLIST_H_
#include <stdio.h>
#include <stdlib.h>

typedef struct Node Node;
typedef struct List List;

int listRemove(List *list, int index, void *outData);
int listGet(const List *list, int index, void *outData);
int listInsert(List *list, int index, void *data);
int listSet(List *list, int index, void *newData, void *oldData);
List *listInitialize();
int listFinalize(List *list);
int listAdd(List *list, void *data);
void listDisplay(const List *list, const char *(*displayFunc)(const void *));

#endif