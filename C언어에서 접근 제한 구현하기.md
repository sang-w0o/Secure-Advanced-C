C언어에서 정보 은닉 구현하기
======
Java, C++ 등 거의 모든 객체 지향 언어에서는 public, protected, private, default 등의 접근제한자를 제공한다.
하지만 C언어에는 이러한 접근제한자가 없기 때문에 정보 은닉을 하려면 사용자가 구현해야 한다.

<hr/>

간단하게 정수를 저장하는 Stack 자료구조를 예를 들어보겠다.

* Stack.h

```C
#ifndef _STACK_H_
#define _STACK_H_
struct Stack {
    int *arr;
    int top;
    int size;
};
typedef struct Stack Stack;

Stack *createStack(int size);
#endif
```

* Stack.c
```C
#include "Stack.h"
#include <stdlib.h>

Stack *createStack(int size) {
    Stack *p = calloc(1, sizeof(Stack));
    if(p == NULL)
        return NULL;
    p->arr = malloc(sizeof(int) * size);
    if(p->arr == NULL)
        return NULL;
    p->size = size;
    return p;
}
```

* main.c
```C
#include "Stack.h"
int main(){
    Stack *s = createStack(5);
}
```
__위 코드의 문제점__
* 위 코드는 아무런 문제 없이 Stack이라는 자료구조를 생성했다. 하지만 정말 문제가 없을까?
* 만약 사용자가 "s->top = 1;"와 같이 Stack 구조체의 멤버값을 임의로 변경할 수 있다면 다양한 문제가 발생할 수 있다.
* 따라서 이렇게 사용자가 잘못된 방법으로 접근하는 것을 막기위해 정보 은닉을 구현할 필요성이 있다.

<hr/>

* stack.h
```C
#ifndef _STACK_H_
#define _STACK_H_

struct Stack;  //Stack이라는 구조체를 선언만 한다.
typedef struct Stack *Stack;  //포인터 변수 Stack 만들기
Stack createStack(int size);
#endif
```

* Stack.c
```C
#include "Stack.h"
#include <stdlib.h>
struct Stack{
    int *arr;
    int top;
    int size;
};

Stack createStack(int size){
    Stack p = calloc(1, sizeof(struct Stack));
    if(p == NULL)
        return NULL;
    p->arr = malloc(sizeof(int) * size);
    if(p->arr == NULL)
        return NULL;
    p->size = size;
    return p;
}
```

* main.c
```C
#include "Stack.h"
int main(){
    Stack s = createStack(5);
    s->top = 1;  //컴파일 에러 발생
}
```

* 위의 main.c를 보면, 사용자가 구조체의 멤버로 잘못된 접근을 할 수 없다.
* 그 이유는 헤더파일에서 구조체를 정의하지 않고, 선언만 했기 때문이다. 즉, 불완전한 타입이 되어서 컴파일 타임에 에러가 발생하는 것이다.
