#include <stdio.h>
#include <stdlib.h>
#include "BinarySeacrhTree.h"

typedef struct Node {
	void *data;
	struct Node *left;
	struct Node *right;
}Node;

typedef struct BST {
	Node *root;
	DisplayFunction displayFunction;
	CompareFunction compareFunction;
	SizeFunction sizeFunction;
}BST;

BST *bstCreate(DisplayFunction displayFunction, CompareFunction compareFunction, SizeFunction sizeFunction) {

	if (displayFunction == NULL || compareFunction == NULL) {
		fprintf(stderr, "bstCreate : argument is NULL.\n");
		return NULL;
	}

	BST *bst = calloc(1, sizeof(BST));
	if (bst == NULL) {
		fprintf(stderr, "bstCreate : calloc failed.\n");
		return NULL;
	}
	bst->displayFunction = displayFunction;
	bst->compareFunction = compareFunction;
	bst->sizeFunction = sizeFunction;
	return bst;
}

int bstInsert(BST *bst, void *data) {
	Node *node = calloc(1, sizeof(Node));
	if (node == NULL) {
		fprintf(stderr, "bstInsert: malloc failed.\n");
		return -1;
	}
	node->data = data;

	if (bst->root == NULL) {
		bst->root = node;
		return 0;
	}

	Node *parent = NULL;
	Node *cur = bst->root;
	while (cur != NULL) {
		parent = cur;
		if (bst->compareFunction(data, cur->data) < 0) {
			cur = cur->left;
		}
		else if (bst->compareFunction(data, cur->data) > 0) {
			cur = cur->right;
		}
		else {
			free(node);
			return -1;
		}
	}

	if (bst->compareFunction(data, parent->data) < 0) {
		parent->left = node;
	}
	else {
		parent->right = node;
	}
	return 0;
}

static void _preorder(const BST *bst, Node *node) {

	if (node == NULL)
		return;

	printf("%s ", bst->displayFunction(node->data));
	_preorder(bst, node->left);
	_preorder(bst, node->right);
}

void preorder(const BST *bst) {
	printf("preorder : ");
	_preorder(bst, bst->root);
	printf("\n");
}

static void _inorder(const BST *bst, Node *node) {
	if (node == NULL)
		return;

	_inorder(bst, node->left);
	printf("%s ", bst->displayFunction(node->data));
	_inorder(bst, node->right);
}

void inorder(const BST *bst) {
	printf("inorder : ");
	_inorder(bst, bst->root);
	printf("\n");
}

static void _postorder(const BST *bst, Node *node) {
	if (node == NULL)
		return;

	_postorder(bst, node->left);
	_postorder(bst, node->right);
	printf("%s ", bst->displayFunction(node->data));
}

void postorder(const BST *bst) {
	printf("postorder : ");
	_postorder(bst, bst->root);
	printf("\n");
}

void *bstGet(BST *bst, void *key) {
	if (bst->root == NULL)
		return NULL;

	Node *cur = bst->root;
	while (cur != NULL) {
		if (bst->compareFunction(key, cur->data) < 0)
			cur = cur->left;
		else if (bst->compareFunction(key, cur->data) > 0)
			cur = cur->right;
		else
			return cur->data;
	}
	return NULL;
}

static void generic_swap(void *a, void *b, size_t dataSize) {
	char *pA = (char *)a;
	char *pB = (char *)b;
	for (size_t i = 0; i < dataSize; i++, pA++, pB++) {
		char t = *pA;
		*pA = *pB;
		*pB = t;
	}
}

void *bstRemove(BST *bst, void *key) {

	if (bst == NULL || key == NULL) {
		fprintf(stderr, "bstRemove : argument is NULL.\n");
		return NULL;
	}

	if (bst->root == NULL) {
		fprintf(stderr, "bstRemove : root is NULL.\n");
		return NULL;
	}

	Node *parent = NULL;
	Node *target = bst->root;
	while (target != NULL && bst->compareFunction(target->data, key) != 0) {
		parent = target;
		if (bst->compareFunction(key, target->data) < 0)
			target = target->left;
		else if (bst->compareFunction(key, target->data) > 0)
			target = target->right;
	}

	if (target == NULL) {
		fprintf(stderr, "bstRemove : Node doesn't exist.");
		return NULL;
	}

	if ((target->left == NULL) && (target->right == NULL)) {
		if (parent != NULL) {
			if (target == parent->left) {
				parent->left = NULL;
			}
			else
				parent->right = NULL;
		}
		else {
			bst->root = NULL;
		}
		void *outData = target->data;
		free(target);
		return outData;
	}

	else if ((target->left == NULL) || (target->right == NULL)) {
		Node *child;
		if (target->left == NULL)
			child = target->right;
		else
			child = target->left;
		if (parent != NULL) {
			if (target == parent->left)
				parent->left = child;
			else
				parent->right = child;
		}
		else {
			bst->root = child;
		}
	}
	else {
		Node *cParent = target;
		Node *candidate = target->right;

		while (candidate->left != NULL) {
			cParent = candidate;
			candidate = candidate->left;
		}

		if (cParent->left == candidate)
			cParent->left = candidate->right;
		else
			cParent->right = candidate->right;

		generic_swap(candidate->data, target->data, bst->sizeFunction());
		target = candidate;
	}

	void *out = target->data;
	free(target);
	return out;
}