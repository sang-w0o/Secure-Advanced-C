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
	
// 배열의 장점
// (1) 구현이 쉽다.
// (2) 임의 접근 가능
// (3) 특정 원소에 빠르게 접근할 수 있다.

// 단점
// (1) 메모리 낭비가 발생할 수 있다.
// (2) 데이터의 삽입, 삭제에 오버헤드가 크다.
#endif