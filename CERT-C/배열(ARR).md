부동소수점 관련 C 시큐어 코딩 규칙
======

1. 배열의 크기를 얻을 때 포인터를 sizeof의 피연산자로 사용하지 마라.   
2. 배열에는 크기 정보가 없으므로 함수로 전달 시 크기 정보를 같이 전달하라.   
   (1번, 2번의 예시 코드)
```C
#include <stdio.h>

void printArray(const int *arr){
    for(int i = 0; i < sizeof(arr) / sizeof(*arr); i++){
        printf("%d  ", arr[i]);
    }
}

int main(){
    int arr[] = {1, 2, 3, 4, 5};
    printArray(arr);
}
```
* 위 코드의 출력결과는 1로, printArray의 루프가 1번만 돈다.
* 그 이유는 arr를 printArray의 인수로 넘길 때, 시작 주소만 보내기 때문이다.
* 따라서 sizeof(arr)는 4U가 되고, sizeof(*arr)도 4U가 된다.
* 그렇다면 위와 같은 경우에는 어떻게 할까?
  * 배열 자체를 넘기기 --> 복사의 오버헤드 발생 가능
  * 포인터(배열의 시작 주소)넘기기 --> 배열 길이 알 수 없음
* 배열의 길이도 인수로 배열의 시작 주소와 같이 보내줘야 한다.   
  (예시 코드)
```C
void printArray(const int *arr, int len);
```
<hr/>

3. 모든 소스 파일에서 일관된 배열 표기를 사용하라.   
   (예시 코드1)
```C
//libarr.c

#include <stdio.h>
extern void create();
extern void destroy();
int *arr;  //전역 배열을 int*로 선언

void create(){
    arr = calloc(10, sizeof(int));
}

void destroy(){
    free(arr);
}
```
<hr/>

```C
//main.c

#include <stdio.h>
int[] arr;

int main(){
    create();
    for(int i = 0; i < 10; i++){
        arr[i] = i + 1;
    }

    destroy();

    return 0;
}
```
* 위 코드를 실행해보면 동작은 잘 되지만, 프로세스가 비정상적으로 종료된다.
* 그 이유는 전역 배열 arr가 libarr.c에서는 int*로 선언되었고,   
  main.c에서는 int[]로 선언되었기 때문이다.
* 위와 같이 같은 심볼에 대하여 자료형이 다른 것은 C언어에서 미정의 동작으로 분류한다.
<hr/>

4. 배열의 인덱스가 유효한 범위에 있음을 보장하라.   
   (예시 코드 1)
```C
void printArray(const int *arr, int len){
    for(int i = 0; i < len; i++){
        printf("%d  ", arr[i]);
    }
}
```
* 위 코드의 문제점 : len(배열의 길이)에 대한 유효성 검사가 없다.
* 먼저 arr가 NULL인지 검사하고, len이 1 이상인지 검사해야 한다.   
  (예시 코드 2)   
```C
int printArray(const int *arr, int len){
    if(arr == NULL)
        return -1;
    if(len < 1)
        return -1;
    for(int i = 0; i < len; i++){
        printf("%d  ", arr[i]);
    }
    return 0;
}
```
* 위와 같이 if문으로 len이 1 미만인지 검사해도 좋지만, 원천적으로   
  음수가 들어오는 것을 막는 것도 하나의 방법이다.
* 즉, len을 size_t로 선언하는 것이다.   
  * 단, 만약 size_t len이 -1이 들어오면 UINT_MAX가 되므로 이에 대한   
  예외처리를 해줘야 한다.        

  (예시 코드 3)
```C
#define ARR_MAX (4096)
int printArray(const int *arr, size_t len){
    if(arr == NULL || len > ARR_MAX){
        return -1;
    }

    for(size_t i = 0; i < len; i++){
        printf("%d  ", arr[i]);
    }
    return 0;
}
```
<hr/>

5. 루프에서 반복자가 배열의 끝을 넘어 접근하지 않게 하라.
<hr/>

6. 같은 배열을 참조하고 있지 않다면 두 개의 포인터를 빼거나 비교하지 마라.
<hr/>

7. 배열이 아닌 객체에 대한 포인터에 정수를 더하거나 빼지 마라.
<hr/>

8. 반환값이 유효한 배열 원소를 참조하고 있지 않은 경우 포인터에 정수를 더하거나 빼지 마라.
<hr/>

9. 충분한 크기의 공간에서 복사가 진행됨을 보장하라.   
<hr/>
