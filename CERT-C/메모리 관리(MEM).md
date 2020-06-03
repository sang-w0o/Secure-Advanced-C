메모리 관련 C 시큐어 코딩 규칙
======

1. 동일한 추상화 레벨의 같은 모듈 내에서 메모리를 할당하고 해제하라.   
   (예시 코드)
```C
#include <stdio.h>
#include <stdlib.h>
#define MIN_SIZE (32)

int verify_list(char *list, size_t size){
    if(size < MIN_SIZE){
        free(list);
        return -1;
    }
    return 0;
}

void process_list(size_t size){
    char *list = malloc(size);
    if(list == NULL)
        return;
    
    if(verify_list(list, size) < 0){
        free(list);
        return;
    }
    
    free(list);
}
```
* 위 코드의 문제점 : list를 두 개의 함수에서 해제하는 경우가 생긴다.
* 해제한 메모리를 또 해제하는 행위는 미정의 동작이다.
* 이러한 일을 방지하기 위해 1번 규칙을 지켜야 한다.
<hr/>

2. free() 후 즉시 포인터에 새로운 값을 할당하라.   
   (예시 코드 1)
```C
#include <stdio.h>
#include <stdlib.h>
#define ARR_SIZE (10)

int main(){
    int *p = malloc(sizeof(int) * ARR_SIZE);

    if(p == NULL){
        fprintf(stderr, "malloc() failed\n");
        return -1;
    }

    free(p);

    //...

    free(p);
}
```
* 위 코드와 같이 한번 해제한 메모리를 또 해제하는 것은 미정의 동작이다.
* 하지만 NULL(0x00000000)을 해제하는 것은 안전하므로 이를 활용해보자. 
* 해제된 포인터가 자동으로 NULL을 가리키게 하는 함수 Free의 구현     
  (예시 코드 2)
```C
#include <stdio.h>
#include <stdlib.h>
#define ARR_SIZE (10)

void Free(void **ptr){
    if(ptr == NULL)
        return;
    if(*ptr != NULL){
        free(*ptr);
        *ptr = NULL;
    }
}

int main(){
    int *p = malloc(sizeof(int) * ARR_SIZE);
	if (p == NULL) {
		fprintf(stderr, "malloc failed.\n");
		return -1;
	}
	Free(&p);
	printf("%p\n", p);  //00000000 출력
}
```
* 위 코드의 단점 : 다중 포인터를 이용하여 코드가 복잡하고,   
  함수의 기능이 단순하여 함수의 오버헤드가 발생할 수 있다.
* 따라서 다음에는 매크로 함수로 구현해보겠다.   
  (예시 코드 3)
```C
#include <stdio.h>
#include <stdlib.h>

#define SAFE_FREE(p) do {  \
    if((p)) {              \
        free((p));         \
        (p) = NULL;        \
    }                      \
} while(0)

int main(){
    int *p = malloc(sizeof(int) * 10);
    if(p == NULL){
        fprintf(stderr, "malloc() failed.\n");
        return -1;
    }

    SAFE_FREE(p);
    printf("%p\n", p);
}
```
* SAFE_FREE의 해석 : if문은 p가 0이 아니면 다 true로 보기 때문에   
  간단하게 p가 NULL인지 아닌지를 검사할 수 있다.
* SAFE_FREE의 단점 : aliasing 된 포인터에 대해서는 동작하지 않는다.
* 단점의 예시
  ```C
  int *p = malloc(...);
  int *q = p;
  SAFE_FREE(p);
  *q = 10;  // 비정상적 종료
  SAFE_FREE(q);  //비정상적 종료
  ```
<hr/>

3. 해제된 메모리에 접근하지 마라.
<hr/>

4. 동적으로 할당된 메모리는 한 번만 해제하라.
<hr/>

5. 동적으로 할당된 메모리만 해제하라.   
   (예시 코드)
```C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    const char *name = "sangwoo";

    char *p = realloc(name, strlen(name) * 2);

    strcpy(p, name);
}
```
* 위 코드는 비정상적으로 동작, 종료된다.
* 그 이유는 realloc함수로 name을 먼저 해제하고, 다시 메모리를 할당했기 때문이다.
* C언어에서 동적으로 할당되지 않은 메모리의 해제는 미정의 동작이다.
<hr/>

6. 메모리 할당 함수의 반환 값을 즉시 할당된 타입의 포인터로 변환해라.   
   (예시 코드)
```C
#include <stdio.h>
#include <stdlib.h>

typedef struct{
    char name[32];
    int age;
}Person;

typedef struct{
    char name[32];
    int age;
    int id;
}Student;

int main(){
    Student *p = malloc(sizeof(Person));
    p->age = 20;
    getchar();
    p->id = 20189999;
}
```
* 위와 같이 해도 프로그램은 정상적으로 수행된다.
* malloc은 void* 형을 반환하는데, 위와 같이 p를 만들면 malloc의 리턴값인 void* 가   
  좌항의 Student*로 형변환되어 p로 들어간다.
* 따라서 malloc의 결과값은 인자로 전달된 타입으로 캐스팅해야 한다.
```C
Student *p = (Person *)malloc(sizeof(Person));
```

* 위 과정을 즉각 수행해주는 매크로 함수를 만들면 다음과 같다.   
  ```C
  #define MALLOC(T) (T *)malloc(sizeof(T))
  ```
<hr/>

7. 큰 스택 할당을 피하라.   
* C99에서 배열의 크기에 상수가 올 수 있게 되었다. 즉, 다음과 같은 코드가 가능하다.   
  ```C
  void makeArray(size_t buf_size){
      int arr[buf_size];
  }
  ```
<hr/>

8. 크기가 0인 할당을 수행하지 마라.
<hr/>

9. Flexible Array Member에 정확한 문법을 사용하라.   
    (예시 코드 1)
```C
//이 코드는 Flexible Array Member의 등장 배경을 알기 위한 코드이다.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct{
    int age;
    char name[32];
}Person;

int main(){
    // 사용자로부터 이름과 나이를 입력받았다고 가정하자.
    char name[32] = "exname";
    int age = 23;

    Person *p = (Person*)malloc(sizeof(Person));
    strcpy(p->name, name);
    p->age = age;

    printf("%s, %d\n", p->name, p->age);

    return 0;
}
```
* 위 코드의 문제점 : Person구조체 내의 name이 낭비되는 공간이 많아질 수 있다.
* 이를 해결하기 위해 name을 동적 할당해보자.   
  (예시 코드 2)
```C
typedef struct {
    int age;
    char *name;
}Person;

int main(){
    //사용자로부터 이름과 나이를 입력받았다고 가정하자.
    char name[32] = "exname";
    int age = 23;

    Person *p = (Person *)malloc(sizeof(Person));
    p->name = malloc(strlen(name) + 1);
    strcpy(p->name, name);
    p->age = age;

    printf("%s, %d\n", p->name, p->age);

    free(p->name);
    free(p);

    return 0;
}
```
* 위 코드의 단점 :
  * (1) : 동적 할당을 2번 수행한다. malloc은 이전에 얘기했듯이   
  OS차원에서 매우 부담이 되는 작업이다.
  * (2) : free(p->name); 을 수행하지 않고, free(p)만 하면   
  name이 해제되지 않아 메모리 누수가 발생할 가능성이 있다.
* Flexible Array Member의 등장   
  (예시 코드 3)
```C
typedef struct {
    int age;
    char name[];
}Person;

int main(){
    char name[32] = "exname";
    int age = 23;

    Person *p = (Person*)malloc(sizeof(Person) + strlen(name) + 1);

    // p의 메모리 할당 시 name의 길이만큼 더 잡아준다.
    // 이는 p는 시작 주소만 알지, 가리키는 것이 얼마나 큰지는
    // 모른다는 맹점을 이용한 것이다.
    // 마지막에 +1 한 것은 ascii null을 저장할 공간을 위함이다.

    strcpy(p->name, name);
    p->age = age;

    printf("%s, %d\n", p->name, p->age);
    
    free(p);

    return 0;
}
```
* Flexible Array Member은 C99표준에 등장한 문법으로,   
  위의 경우 Person구조체 안의 name멤버이다.
* Flexible Array Member는 배열의 길이 정보가 없는 구조체의 멤버   
  필드로, 반드시 구조체의 마지막 멤버에 위치해야한다.
* 즉, 하나의 구조체에 반드시 1개만 존재해야 한다.
<hr/>

10. 같은 메모리에 대한 복사를 수행할 때는 memcpy를 사용하지 말고, memmove함수를 사용하라.
