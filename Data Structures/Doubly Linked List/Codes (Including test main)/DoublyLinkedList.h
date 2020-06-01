#ifndef _DOUBLYLINKEDLIST_H_
#define _DOUBLYLINKEDLIST_H_

typedef struct Node Node;
typedef struct List List;
typedef void(FreeFunction)(void *ptr);

List *listInitialize(FreeFunction freeFunction);
int listFinalize(List *list);
void listDisplay(const List *list, const char *(*displayFunc)(const void *));
void listReverseDisplay(const List *list, const char *(*displayFunc)(const void *));
int listAddBack(List *list, void *data);
int listAddFront(List *list, void *data);
void *listRemove(List *list, int index);
void *listRemoveHead(List *list);
void *listRemoveTail(List *list);
void *listGet(const List *list, int index);
void *listSet(List *list, int index, void *newData);
int listInsert(List *list, int index, void *data);

#endif