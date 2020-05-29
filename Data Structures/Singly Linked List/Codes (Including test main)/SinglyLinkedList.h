#ifndef _SINGLYLINKEDLIST_H_
#define _SINGLYLINKEDLIST_H_
#include <stdio.h>
#include <stdlib.h>

typedef struct Node Node;
typedef struct List List;

void *listRemove(List *list, int index);
void *listGet(const List *list, int index);
int listInsert(List *list, int index, void *data);
void *listSet(List *list, int index, void *newData);
List *listInitialize();
int listFinalize(List *list);
int listAdd(List *list, void *data);
void listDisplay(const List *list, const char *(*displayFunc)(const void *));

#endif