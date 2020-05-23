선언과 초기화 관련 C 시큐어 코딩 규칙
======

1. 변하지 않는 객체는 const로 포장해둬라.
<hr/>

2. 구조체 배열 등을 함수의 인자로 전달할 때에는 포인터로 전달하라.
<hr/>

3. 함수를 main아래에 정의할 경우, 꼭 상단에 선언을 해라.
<hr/> 

4. 함수 포인터 사용 시, 시그니처를 주의해서 알맞게 작성하라.   
(예시 코드)
```C
#include <stdio.h>
int foo(int, int, int);

int main(){
    int(*fp)(int, int) = foo;
    fp(1,2);
}

int foo(int a, int b, int c){
    printf("foo(%d, %d, %d)", a, b, c);
    return 0;
}
```
* 위 코드에서는 컴파일타임 및 런타임에서 에러가 발생하지 않지만, foo()호출 시 인수가 3개가 아닌 2개가 들어갔기 때문에 Garbage Value가 출력된다. 이러한 상황을 막기 위해 함수 시그니처를 알맞게 작성해야 한다.
<hr/>

5. 지역 변수는 선언과 동시에 초기화를 해주어라.   
    (지역 변수는 초기화하지 않으면 컴파일러에 따라 0으로 초기화되거나, Garbage Value가 들어갈 수 있다.)
<hr/>

6. 초기화 되지 않은 변수는 참조하지 마라.
<hr/>

7. 캐시될 수 없는 데이터에는 volatile 키워드를 사용하라.   
    (예시 코드 - 컴파일러 최적화 옵션 주고 테스트해야된다.)
```C
#include <Windows.h>
#include <stdio.h>

int flag = 1;

unsigned long __stdcall threadMain(void *p){
    for(int i = 0; i < 10; i++){
        Sleep(5000);
        flag = 0;
        printf("flag changed to 0!\n");
    }
}

int main(){
    CreateThread(0, 0, threadMain, 0, 0, 0);

    while(flag){
        printf("waiting for flag to change to 0..\n");
        Sleep(1000);
    }

    printf("[main thread terminated.]\n");
}
```
* 위 코드에서는 main함수에서 thread를 threadMain함수를 인자로 받아 생성한다.
* threadMain에서는 5초 sleep 후 flag변수를 0으로 바꾸어서   
  main의 while문을 5초후에 탈출해야한다.
* 하지만, flag는 0으로 바뀌지 않아 while문은 무한 루프에 빠지게 된다.
* 이 이유는 flag가 컴파일러 최적화에 의해 값이 캐싱되기 때문이다.
* 이럴 때 사용해주는 키워드가 volatile 이다.
* 위의 코드에 flag 선언을 "volatile int flag = 1;"로 바꾸면 캐싱이 되지 않아   
  while문을 5초 후에 탈출하는 것을 볼 수 있다.
<hr/>

8. 해당 심볼이 정의된 파일의 범위를 넘어서까지 사용되지 않을   
    객체(변수)나 함수는 반드시 static으로 선언하라.
* (C언어에서 접근제한 구현하기.md) 참고
<hr/>
