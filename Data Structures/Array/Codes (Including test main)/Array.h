#ifndef _ARRAY_H_
#define _ARRAY_H_
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Array Array;  //For data-hiding.

#define INITIAL_SIZE	(4)  //user can define the initial size.
#define MAX_SIZE (4096)  //user can define the max size of the array.

Array *arrayCreate();
void arrayDestroy(Array *array);
int arrayAdd(Array *array, void *data);
void arrayDisplay(const Array *array, const char *(*display)(const void *));
void *arraySet(Array *array, int index, void *newData);
int arrayInsert(Array *array, int index, void *newData);
int arrayCount(const Array * array);
void *arrayGet(const Array *array, int index);
void *arrayRemove(Array *array, int index);
	
// �迭�� ����
// (1) ������ ����.
// (2) ���� ���� ����
// (3) Ư�� ���ҿ� ������ ������ �� �ִ�.

// ����
// (1) �޸� ���� �߻��� �� �ִ�.
// (2) �������� ����, ������ ������尡 ũ��.
#endif