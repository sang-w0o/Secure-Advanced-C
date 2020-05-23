정수 관련 시큐어 C 코딩 규칙
======

1. 구현 시 사용되는 데이터 모델을 이해해라.   
   (데이터 모델 : 표준 데이터 타입에 할당되는 크기)   
   (예시 코드)
```C
#include <stdio.h>
#include <stdint.h>
int main(){
    unsigned int a, b;
    uintmax_t c;

    a = 1000000000;  //10억
    b = 10;

    c = (uintmax_t)a * b;
    printf("%llu\n", c);
}
```
* 위 코드는 플랫폼마다 다른 값이 나온다.
* uintmax_t : stdint.h내의 타입으로,   
  해당 시스템에서 제공할 수 있는 부호없는 가장 큰 타입.
* (참고)
  * Windows 64Bit : long --> 4Byte
  * Linux 64Bit : long --> 8Byte
<hr/>

2. 타입이 다른 피연산자들에 대해서는 연산을 수행하지 말아라.
<hr/>

3. 문자열을 정수로 변환할 때는 strtol()이나 관련 함수를 사용하라.   
   (예시 코드 1)
```C
#include <stdio.h>
#include <stdlib.h>

int main(){
    char buf[32];
    printf("input number : ");

    fgets(buf, sizeof(buf), stdin);

    int num = atoi(buf);
    printf("%d\n", num);
}
```
* 위 코드에서 알 수 있는 atoi() 등 관련 함수들의 문제점
  * 에러 발생 시, 에러코드가 설정되어 있지 않다.
  * 표현할 수 없는 값이 들어올 경우 미정의 동작이 수행된다.   
  (예 : 131231231421123)
  * 문자열에 숫자가 아닌 다른 문자가 들어오면 0을 반환한다.   
  따라서 오류 처리가 불가하다.   

   (예시 코드 2)
```C
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(){
    char buf[32];
    printf("input number : ");

    fgets(buf, sizeof(buf), stdin);

    char *endPtr;
    errno = 0;
    //errno는 오류코드를 저장하는 전역변수이다.
    //errno.h에 정의되어 있으며, 사용하려면
    //자동초기화가 안되므로
    //처음에 0으로 초기화해야한다.

    long num = strtol(buf, &endPtr, 10);
    //strtol의 인자 목록
    //바꿀 문자열, char* 포인터, n진수의 n값
    //2번째 인수는 오류처리를 위해 사용된다.
    //endPtr은 입력된 문자열의 시작부터 끝까지
    //1Byte씩 파싱을 해간다.
    //단, 개행 또는 ASCII NULL을 만나면
    //파싱을 중단한다.

    //1. 값의 범위를 넘어설 경우의 오류 처리
    if(errno == ERANGE) {
        perror("strtol() error");
        return -1;
        //ERANGE : 값의 범위를 넘어설 경우의 errno 값
    }

    //2. 처음부터 숫자가 아닌 다른 문자가 입력될 경우
    //즉, 시작포인터와 buf의 시작주소가 같을 경우
    else if(endPtr == buf){
        fprintf(stderr, "숫자가 아닙니다.");
        return -1;
    }

    //3. 중간에 파싱을 실패한 경우
    //파싱을 성공했다면 endPtr은 개행 또는
    //ASCII NULL이어야 한다.
    else if(*endPtr != '\n' && *endPtr != '\0'){
        fprintf(stderr, "숫자가 아닌 값 포함");
        return -1;
    }

    //파싱을 정상적으로 성공한 경우
    printf("%ld\n", num);
    return 0;
}
```
<hr/>

4. 숫자 값에는 명시적으로 unsigned나 signed를 지정하라.
   (예시 코드)
```C
#include <stdio.h>

int main(){
    unsigned char c = 200;
    int i = 1000;
    printf("%d / %d = %d\n", i, c, i/c);
    // 1000/200 = 5 출력

    char c2 = 200;
    printf("%d / %d = %d\n", i, c2, i/c2);
    // 1000 / -56 = -17 출력

    signed char c3 = -1;
    printf("%u\n", c3);
    // -1은 2진수로 1111 1111 이 된다.
    // 하지만 c3는 signed이므로 모든 bit가
    // data bit가 되어 255가 된다.
    // 이때, c3의 선언은 1Byte인 char로 했지만,
    // 메모리에는 4Byte로 잡히기 때문에 
    // 출력 결과는 unsigned int의 최대값인
    // 42억이 나오게 된다.
}
```
<hr/>

5. % 연산자를 쓸 때 나머지가 양수라고 가정하지 마라.   
   (예시 코드)
```C
#include <stdio.h>
int main() {
	printf("%d / %d = %d\n", 4, 3, 4 / 3);  
    //몫 : 양수
	printf("%d / %d = %d\n", -4, 3, -4 / 3);  
    //몫 : 음수
	printf("%d / %d = %d\n", 4, -3, 4 / -3);  
    //몫 : 음수
	printf("%d / %d = %d\n", -4, -3, -4 / -3);  
    //몫 : 양수

	printf("%d %% %d = %d\n", 4, 3, 4 % 3);  
    //나머지 : 양수
	printf("%d %% %d = %d\n", -4, 3, -4 % 3);  
    //나머지 : 음수
	printf("%d %% %d = %d\n", 4, -3, 4 % -3);  
    //나머지 : 양수
	printf("%d %% %d = %d\n", -4, -3, -4 % -3);  
    //나머지 : 음수
}
```
<hr/>

6. 객체의 크기를 나타내는 정수값은 rsize_t나 size_t를 사용하라.   
   (예시 코드)
```C
#include <stdio.h>
#define QUEUE_MAX (10)
typedef struct {
    int buf[QUEUE_MAX];
    int front;
    //front : 데이터를 꺼내오기 위한 위치 정보
    int rear;
    //rear : 데이터를 저장하기 위한 위치 정보
}Queue;

void initQueue(Queue *q){
    q->front = 0;
    q->rear = 0;
}

void insert(Queue *q, int data){
    q->buf[q->rear] = data;
    q->rear = (q->rear + 1) % QUEUE_MAX;
    // buf가 가득 찼을 때 rear를 buf의 처음으로
    // 보내주기 위해 %연산을 수행한다.
}
```
* 위 코드의 문제점   
  * (5)번에서 봤듯이 피제수 (q->rear + 1)이 음수이면   
   % 연산의 결과가 음수가 된다.
  * 따라서 이 코드는 잘못된 메모리 참조로 인해 종료될 수 있다.
<hr/>

7. 표현식에서 signed, unsigned 표시가 없는 int 비트 필드의 타입을 가정하지 마라.   
   (예시 코드)
```C
#include <stdio.h>

struct BITS {
    int a : 8;
    //a에 대하여 8bit만 쓰겠다는 표현.
}

int main(){
    printf("%d\n", bits.a);
}
```
* 위 코드의 결과는 구현마다 다르게 동작한다.
   * unsigned int로 판단될 경우 : 255 출력
   * signed int로 판단될 경우 : -1 출력
* 위와 같은 문제를 해결하기 위해 (4)번의 중요성이 다시   
  한번 강조된다.
<hr/>

8. 비트 연산자(>>, <<, %, |, ~, ^)는 unsigned   
   피연산자에 대해서만 사용하라.
<hr/>

9. 이식성이 좋은 자료형을 쓰려면 stdint.h에 정의된   
    자료형을 사용하라.
<hr/>

10. 불분명한 소스에서 얻어지는 정수값은 제한을 강제하라.
* (예시) : 사용자로부터 값을 입력받는 경우
<hr/>

11. unsigned 정수 연산이 래핑되지 않도록 주의하라.   
    (예시 코드)
```C
#include <stdio.h>
#include <limits.h>
int main(){
    unsigned int uint1, uint2, sum = 0;
    uint1 = UINT_MAX;
    uint2 = 1;
    
    sum = uint1 + uint2;
    printf("%d\n", sum);  //0 출력
}
```
* C99 표준에 따르면 unsigned 정수에 대한 연산은   
  결코 정수 오버플로우가 발생하지 않는다고 한다.   
  그 이유는 결과값이 저장할 수 없는 타입(값의 범위를 넘어서면)   
  % 연산으로 값을 줄여 표현하기 때문이다.   
  따라서 위 코드의 결과가 0이 나오는 것이다.   
  이렇게 값을 줄여 표현하는 것을   
  Wrap Around(정수 래핑) 이라 한다.
* 정수 래핑을 방지하기 위해 코드를 수정해보자.
```C
#include <stdio.h>
#include <limits.h>
int main(){
    unsigned int uint1, uint2, sum = 0;
    uint1 = UINT_MAX;
    uint2 = 1;
    
    if(uint1 > UINT_MAX - uint2) {
        fprintf(stderr, "정수 래핑 발생");
        return -1;
    }

    sum = uint1 + uint2;
    printf("%d\n", sum);  //0 출력
}
```
<hr/>

12. signed 정수의 연산이 오버플로우되지 않도록 보장하라.   
    (예시 코드)
```C
#include <stdio.h>
#include <limits.h>

int main(){
    signed int s1, s2, result = 0;
    s1 = INT_MAX;
    s2 = 1;

    result = s1 + s2;
    printf("%d\n", result);  //음수 값 출력
}
```
* 정수 오버플로우 : C언어에서 부호 있는 정수에 대해   
  연산을 수행할 경우, 결과값이 표현 가능한 범위를   
  넘어서면 이를 정수 오버플로우라 한다.
* C언어에서 정수 오버플로우는 미정의 동작이므로   
  signed 정수에 대하여 산술 연산 시, 오버플로우가   
  방지하지 않음을 보장해야 한다.
* 아래 코드는 간단히 정수 오버플로우를 방지한 것이다.   
```C
#include <stdio.h>
#include <limits.h>

int main(){
    signed int s1, s2, result = 0;
    s1 = INT_MAX;
    s2 = 1;

    //signed int의 최대값인 INT_MAX 범위 넘은 경우 검사
    if (((s1 > 0) && (s2 > 0)) && (s1 > INT_MAX - s2)) {
        fprintf(stderr, "정수 오버플로우 발생\n");
        return -1;
    }

    //두 피연산자가 양수인데 덧셈 연산의 결과가 
    //음수이면 정수 오버플로우가 발생한 것이다.
    //따라서 이를 검사한다.
    if (((s1 > 0) && (s2 > 0)) && (s1 + s2 < 0)) {
        fprintf(stderr, "정수 오버플로우 발생\n");
        return -1;
    }

    //두 피연산자가 음수인데 덧셈 연산의 결과가
    //양수이면 정수 오버플로우가 발생한 것이다.
    //따라서 이를 검사한다.
    if(((s1 < 0) && (s2 < 0)) && (s1 + s2 > 0)) {
        fprintf(stderr, "정수 오버플로우 발생\n");
        return -1;
    }

    //오버플로우 검사를 통과했다면
    result = s1 + s2;
    printf("%d\n", resulte);
    return 0;
}
```
<hr/>