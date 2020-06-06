#ifndef _BINARYSEARCHTREE_H_
#define _BINARYSEARCHTREE_H_
#include <stdio.h>
#include <stdlib.h>

typedef struct Node Node;
typedef struct BST BST;

typedef const char *(*DisplayFunction)(void *data);
typedef int (*CompareFunction)(void *data1, void *data2);
typedef size_t(*SizeFunction)();

BST *bstCreate(DisplayFunction, CompareFunction, SizeFunction);
int bstInsert(BST *bst, void *data);
void preorder(const BST *bst);
void inorder(const BST *bst);
void postorder(const BST *bst);
void *bstGet(BST *bst, void *key);
void *bstRemove(BST *bst, void *key);

#endif