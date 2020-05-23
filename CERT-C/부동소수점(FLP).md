부동소수점 관련 C 시큐어 코딩 규칙
======

1. 부동소수점 변수를 루프 카운터로 사용하지 마라.   
   (예시 코드)
```C
#include <stdio.h>
int main(){
   int cnt = 0;
   for( float f = 0.0f; f <= 1.0f; f += 0.1f) {
       printf("f = %.30f\n", f);
       ++cnt;
   }
   printf("%d\n", cnt);
}
```
* 위 코드는 루프가 11번 돌아야하지만 10번만 돌게 된다.
* 부동소수점은 연산을 위해 만들어진 자료형이 아니기 때문이다.
* 그렇다면 부동소수점을 사용하는 목적을 알아보자.   
  * 매우 큰 수의 표현
  * 매우 정밀한 수의 표현
<hr/>

2. 부동소수점 연산용 정수는 먼저 부동소수점으로 바꿔라.   
   (예시 코드)
```C
#include <stdio.h>
int main(){
    int a = 533;
    float f = a / 7;
    printf("%f\n", f);
    // 76.000000 출력
    return 0;
}
```
* 위 코드는 533이 7로 나누어 떨어지지 않음에도 불구하고   
  76.00000이 나온다. 그 이유는 다음과 같다.
  * "float f = a / 7;"의 a / 7의 값을 저장한 임시변수에는   
    값이 부동소수점까지 들어가지만, a가 int형이므로 결국   
    f에는 소수점이 들어가지 않게 된다.
  * 따라서 아래와 같이 코드를 바꿔야 한다.
```C
#include <stdio.h>
int main(){
    int a = 533;
    float f = a / 7.0f;
    printf("%f\n", f);
    return 0;
}
```
<hr/>

3. 부동소수점 연산시에는 오차가 발생하지 않는 정수형으로 변경하여   
   연산을 수행하라.   
   (예시 코드)
```C
#include <stdio.h>

float avg(const float *arr, int len){
    if(len == 0)
        return 0.0f;
    
    float total = 0.0f;
    for(int i = 0; i < len; i++) {
        total += arr[i];
        printf("total in loop %d : %f\n", i, total);
    }

    if(len != 0)
        return total / len;
    else
        return 0.0f;
}

int main(){
    float arr[10];
    for(int i = 0; i < 10; i++) {
        arr[i] = 10.1f;
        //arr의 모든 원소를 10.1f로 초기화
    }
    float result = avg(arr, 10);
}
```
* 위 코드는 누적 오차가 발생한다. 
* 따라서 아래에서는 부동소수점을 정수로 변경하여 계산하겠다.
```C
#include <stdio.h>

float avg(const float *arr, int len) {
    if(len == 0)
        return 0.0f;
    
    int total = 0;  //total을 정수형으로 선언
    
    for(int i = 0; i < len; i++) {
        total += arr[i];
        printf("total in loop %d : %f\n", i, total / 100.0);
    }

    if(len != 0)
        return (float)total / len;
    else
        return 0.0f;
}

int main(){
    float arr[10];
    for(int i = 0; i < 10; i++) {
        arr[i] = 1010;
    }
    
    float result = avg(arr, 10);
    printf("result : %f\n", result / 100.0);
}
```
* 위와 같이 작성하면 연산이 정확함을 알 수 있다.
<hr/>

4. 부동소수점 변환이 새로운 타입의 범위 안에 들어가는지 확인하라.   
   (예시 코드)
```C
#include <stdio.h>
int main(){
    float f = 10000000000.0F;  //100억
    int i;
    i = f;
    printf("%f\n", f);
    printf("%d\n", i);
}
```
* 위 코드의 출력 결과를 보면 f의 값은 잘 나오지만   
  i의 값은 -2147483648, 즉 signed int의 최소값이 나온다.
* 이 문제는 f의 값이 너무 커서 int의 범위를 벗어났기 때문이다.
* 이러한 경우를 해결하기 위해 아래와 같이 체크해줘야 한다.   
```C
#include <stdio.h>
#include <limits.h>

int main(){
    float f = 10000000000.0F;
    int i;

    if(f > (float)INT_MAX || f < (float)INT_MIN) {
        fprintf(stderr, "부동소수점 크가 너무 큽니다.");
        return -1;
    }

    i = f;
    printf("%f\n", f);
    printf("%d\n", i);
}
```
<hr/>

