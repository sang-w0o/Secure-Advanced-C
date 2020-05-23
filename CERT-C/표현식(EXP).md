표현식 관련 C 시큐어 코딩 규칙
======

1. 연산자 우선순위를 나타낼때는 괄호를 사용해라.
<hr/>

2. 포인터로 가리키는 타입의 크기를 결정하기 위해   
   포인터의 크기를 사용하지 마라.
<hr/>

3. 논리 연산자 AND와 OR의 단축 평가 방식을 주의하라.   
   (예시 코드)
```C
#include <stdio.h>
#define SIZE_MAX (5)
int main(){
    int arr[SIZE_MAX] {0,1,2,3,0};
    int i = 0;
    int cnt = 0;

    while((arr[i] == 0) && (++i < SIZE_MAX))
        ++cnt;
    
    printf("%d\n", cnt);
    return 0;
}
```
* 위 코드를 보면 cnt의 값은 2가 나와야 하지만 1이 나온다.
* 논리 연산자 AND는 좌항이 거짓이면 우항을 검사하지 않는다.
* 논리 연산자 OR는 좌항이 참이면 우항을 검사하지 않는다.
* 위와 같은 것을 "논리 연산의 단축 평가"라 한다.
* while문 대신 아래 코드와 같이 하면 논리 연산의 단축 평가를 해결할 수 있다.
```C
//for문을 제외한 나머지 코드 동일
for(i = 0; i < SIZE_MAX; i++){
    if(arr[i] == 0)
        ++cnt;
}
```
<hr/>

4. 구조체의 크기를 각 멤버의 크기의 합이라고 가정하지 마라.   
   (예시 코드)
```C
struct {
    char buf[10];
    int a;
    int b;
} TEMP;

int main(void){
    printf("%d\n", sizeof(TEMP));
}
```
* 위 코드를 보면 TEMP구조체의 멤버는 
   * char buf[10]; --> 10Byte
   * int a; --> 4Byte
   * int b; --> 4Byte
* 위와 같이 되므로 TEMP의 크기가 18Byte라고 생각하겠지만, 결과는 20Byte가 나온다. (VS 기준)
* 이처럼 구조체는 Padding Bit가 들어가는데, Padding Pattern은 컴파일러별로 다르므로 꼭 sizeof연산자를 이용해서 구조체의 크기를 알아내야 한다.
<hr/>

5. const를 캐스트로 없애지 마라.   
   (예시 코드)
```C
int main(){
    const double PI = 3.14;
    double *p = &PI;
    *p = 0;
    printf("%f\n", PI);
}
```
* 위 코드는 일반 포인터 p로 상수 PI를 가리켜 상수인 PI의    값을 0.0으로 바꿨다.
<hr/>

6. 시퀀스 포인트에서 동일한 객체를 여러번 참조하지 말아라.   
    (예시 코드 1)
```C
#define SQR(x) ((x)*(x))
int main(){
    int n = 2;
    int res = SQR(++n);
    printf("%d\n", res);
}
```

* 위 코드에서 출력되는 res의 값은 12일거 같지만, 16이다.
* 문제를 알기 전에, 시퀀스 포인트의 개념을 알고 가자.
* 시퀀스 포인트(Sequence Point) : 모든 식을 평가(계산) 하고 그 값이 메모리에 저장되는 시점
* 시퀀스 포인트의 예시 : 함수 호출 연산자(), 쉼표, 세미콜론, for(), while() 등
* C표준에서는 이러한 시퀀스 포인트에서 동일한 객체의 값을 여러 번 수정하는 것을 미정의 동작으로 정의한다.
* 위 코드의 int res = SQR(++n);은 전처리기가 수행되면   
  int res = ((++n)*(++n));이 되고, 여기서의 시퀀스 포인트는 ; 이 된다.   
  이는 시퀀스 포인트를 만나기 전에 n을 여러번 참조하고 있기 때문에, 미정의 동작이다. 따라서 이 값은 컴파일러별로 값이 다르게 나올 수 있다.
* 다음은 시퀀스 포인트 관련 미정의 동작 예시들이다.   
   * ++i + ++i;
   * i = ++i + 1;
   * arr[i++] = i;
   * func(n++, n);
        
* (예시 코드 2 - factorial 구하는 함수)
```C
int fact(int n){
    if(n <= 1)
        return 1;
    return n * fact(--n);
}
int main(){
    printf("%d\n", fact(5));
}
```
* 위 예시 코드 2 에서의 fact함수의 return문에서는 동일한 객체 n을 시퀀스 포인트 이전에 여러번 참조하고 있다.   
따라서 이는 미정의 동작이다.
* (예시 코드 3 - assert 함수)
```C
int main(){
    int n = 0;
    assert(++n > 0);
    return 0;
}
```
* 위 코드의 assert()함수는 assert.h 파일을 보면 매크로 함수임을 알 수 있다.
* 매크로 함수에서 단항 연산자 사용에 대한 것은   
  (전처리기(PRE).md)를 참조하라.
<hr/>

7. 함수의 반환값을 시퀀스 포인트에서 접근하거나 수정하지 말아라.   
   (예시 코드 1 - 임시 객체 개념)
```C
#include <stdio.h>
int main(){
    int a = 10;
    int b = 20;
    int c;
    int c = a + b;
}
```
* 지역 객체 : 이름이 있으며, 함수가 종료되면 파괴되는 객체
* 임시 객체 : 이름이 없으며, 문장의 끝을 만나면 파괴되는 객체
* 위 코드에서는 int c = a + b;에서 임시 객체가 사용되었다.   
  * 그 코드를 pseudo-code로 바꾸면 다음과 같다.   
  * int tmp = a + b; int c = tmp; delete tmp;  
   
   (예시 코드 2)   
```C
#include <stdio.h>
#include <string.h>

typedef struct {
    char buf[32];
    int len;
}String;

String makeString(const char* str){
    String s;
    strcpy(s.buf, str);
    s.len = strlen(str);
    return s;
}

int main(){
    printf("%s\n", makeString("HI").buf);
}
``` 
* 위 코드의 문제점 : makeString("HI").buf 부분
  * makeString()에서 임시 객체를 사용한다.
  * 이 임시 객체에 대해서 buf를 참조한 것이 문제이다.
  * 왜나햐면 임시 객체가 언제 사라질 지 알 수 없기 때문이다.
* 따라서 다음과 같이 수정하여 사용해야 한다.
```C
int main(){
    String s = makeString("HI");
    printf("%s\n", s.buf);
}
```
* 위와 같이 함수의 반환값을 바로 참조하지 말고,   
  지역 객체(변수)에 저장하여 사용해야 한다.
<hr/>

8. volatile 키워드를 캐스팅하여 없애지 말아라.
<hr/>

9. 호환되지 않는 타입들에는 연산자를 적용하지 말아라.
<hr/>

10. 함수에 의해 반환되는 값을 무시하지 마라.
* 예시1) printf : 화면에 출력된 문자의 개수 반환
* 예시2) scanf : 잘못된 값이 들어오면 0, 정상적인 값을   
  입력받으면 입력받는 값들의 개수 반환 