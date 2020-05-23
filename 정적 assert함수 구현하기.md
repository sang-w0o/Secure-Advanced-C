C언어 정적 ASSERT 함수 구현하기
======
<h2>정적 assert함수가 필요한 이유</h2>
<hr/>
<h4>기본 제공해주는 assert 함수의 문제점</h4>

* assert함수는 assert.h 헤더파일에서 제공한다.
* assert함수는 인수의 조건이 거짓이라면 abort()를 호출하여 프로세스를 종료 시킨다.   
  이는 항상 구동되어야하는 프로그램에 대하여 치명적일 수 있다.   
* 또한 assert함수는 컴파일 타임이 아닌, 런타임에 조건을 검사한다.   
  (예시 코드)
```C

//Header 구조체의 크기가 5여야만 된다고 가정하자.
#include <stdio.h>
#include <assert.h>

typedef struct {
    char cmd;  //패킷의 종류, 1Byte
    int len;  //패킷의 길이, 4Byte
} Header;

int main(){
    assert(sizeof(Header)== 5);
    printf("헤더 길이가 5입니다.");
    return 0;
}
```
* 위 코드는 실행 시 런타임에서 assert함수의 조건을   
  만족하지 못하므로 프로세스가 종료된다.
* 그렇다면 assert가 하는 기능을 런타임이 아닌 컴파일 타임에 수행할 수 없을까?
* 먼저 가장 기본적인 방법은 조건문을 추가하는 것이다.
```C
#include <stdio.h>
#include <assert.h>

typedef struct {
    char cmd;  //패킷의 종류, 1Byte
    int len;  //패킷의 길이, 4Byte
} Header;

int main(){

    if(sizeof(Header) != 5) {
        fprintf(stderr, "헤더 크기가 5가 아닙니다."\n);
        return -1;
    }
    assert(sizeof(Header)== 5);
    printf("헤더 길이가 5입니다.");
    return 0;
}
```
* 하지만, 위에 추가된 if문도 런타임에 검사를 하기 때문에   
  컴파일 타임에 조건 검사를 하는 것은 아니다.
* 컴파일 타임에 조건 검사를 위해 아래에서는 STATIC_ASSERT 함수를 구현하겠다.
```C
#include <stdio.h>
#include <assert.h>
#define __JOIN(x,y) x##y
//매크로의 ##연산자는 두 피연산자를 그대로 이어붙여준다.
#define JOIN(x,y) __JOIN(x,y)
#define STATIC_ASSERT(e) \
    typedef char JOIN(assertion_failed_at_line, __LINE__)[(e)? 1 : -1]

// STATIC_ASSERT 매크로 함수의 해석 :
// STATIC_ASSERT(e)의 인자인 e는 e가 true라면
// char형, 원소가 1개인 배열을 typedef로 선언하지만,
// e가 false라면 char형이고, 원소가 -1개인 배열을
// typedef로 선언하게 된다.
// 이때, 원소가 -1개인 배열은 존재할 수 없으므로
// 컴파일 타임에서 에러가 나게 된다.

// 위 STATIC_ASSERT 매크로 함수의 구현부에서 
// typedef를 사용하지 않으면 실제 char형 배열 arr를 만드므로
// 1Byte를 낭비하게 된다. 하지만 typedef를 사용하면
// 타입에 대한 선언에 불과하기 때문에 스택에 실제 메모리가
// 잡히지 않게 된다.

// typedef에서 배열명을 assertion_failed_at_line, __LINE__으로
// 한 이유 :
// 만약 typedef char arr[(e) ? 1 : -1] 로 하게 된다면
// arr라는 변수가 있다면 그 이유 때문에 컴파일이 되지 않는다.
// 따라서 typedef에 사용될 변수명을 유일하게 해줘야 하는데,
// STATIC_ASSERT를 호출한 라인 넘버를 배열명에 넣는다면
// 그 배열명은 전체 코드 중 유일함을 보장할 수 있게 된다.

// __JOIN 매크로 함수를 굳이 만든 이유 :
// 만약 __JOIN 매크로 함수가 없고
// #define JOIN(x, y) x##y 만 있다면
// STATIC_ASSERT에서 배열명을 붙일 때
// __LINE__에 해당 줄 번호가 들어가지 않고, 문자열 __LINE__
// 이 그대로 들어가게 되기 때문이다.

typedef struct {
    char cmd;
    int len;
}Header;

int main(){
    int arr;

    STATIC_ASSERT(sizeof(Header) == 5);
}
```
* 위와 같이 매크로 함수 STATIC_ASSERT를 작성하면   
  컴파일 타임에 에러가 나는 것을 볼 수 있다.
