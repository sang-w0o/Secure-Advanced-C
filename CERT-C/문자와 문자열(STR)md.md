문자와 문자열 관련 C 시큐어 코딩 규칙
======

1. 문자열을 위한 공간이 문자 데이터와 null 종료문자를 담기에 충분함을 보장하라.   
   (예시 코드 1)
```C
#include <stdio.h>
#include <string.h>

int main(){
    const char *src = "Hello, world";
    char dst[5];

    strcpy(dst, src);
    printf("%s\n", dst);
}
```
* 위 코드는 실행은 되지만, 제대로 동작하지 않는다.
* src는 문자열이 총 13Byte이지만, dst는 5Byte이다.
* 이렇게 대상체에 대상체의 버퍼보다 큰 것을 복사하려하면   
  버퍼 오버플로우가 발생한다.
* 위에서 사용한 strcpy 대신 strncpy를 사용해보자.   
  (예시 코드 2)
```C
#include <stdio.h>
#include <string.h>

int main(){
    const char *src = "Hello, world";
    char dst[5];

    //strncpy의 세번 째 인수 : 대상체 버퍼의 크기
    strncpy(dst, src, sizeof(dst));

    printf("%s\n", dst);
}
```
* 위 코드도 마찬가지로 실행은 되지만, 정상적으로 복사가 이뤄지지 않는다.
* strncpy, fgets 등은 버퍼 오버플로우를 회피하기 위한 함수이다. 이러한 함수들의 단점들:
  * 지정된 제한을 넘는 문자열은 단순히 잘라버린다.
  * 이는 데이터의 손실과 더불어 소프트웨어의 취약성이 될 수 있다.
  * 위와 같은 단점을 보완하기 위해서는 추가적인 검사가 필요하다.   
  
   (예시 코드 3)
```C
#include <stdio.h>
#include <string.h>

int main(){
    const char *src = "Hello, world";
    char dst[5];

    //src의 NULL 체크
    if(src == NULL){
        fprintf(stderr, "문자열이 없습니다.\n");
        return -1;
    }

    //dst의 크기보다 src의 크기가 더 큰지 검사
    else if(strlen(src) >= sizeof(dst)){
        fprintf(stderr, "문자열이 너무 깁니다.\n");
        return -1;
    }

    //모든 검사 통과 시
    strncpy(dst, src, sizeof(dst));
    printf("%s\n", dst);
    return 0;
}
```
<hr/>

2. 경계가 불분명한 소스로부터 고정된 길이의 배열에 데이터를 복사하지 마라.   
   (예시 코드)
```C
#include <stdio.h>

int main(){
    char buf[32];
    gets(buf);

    // gets()는 길이 범위를 넘어선 문자열을 입력받으면
    // 오류가 발생한다.
}
```
* 해결 방법 (1) : 임시 버퍼를 동적 할당하기
  * 임시 버퍼에서 오버플로우가 발생할 수 있으므로 좋지 않다.
* 해결 방법 (2) : fgets() 사용
  * fgets는 오버플로우가 발생하지 않도록 입력받을 길이를 지정할 수 있다.
```C
int main(){
    char buf[32];
    fgets(buf, sizeof(buf), stdin);

    //fgets의 인수 목록 : 버퍼, 버퍼크기, 입력스트림
}
```
* 해결 방법 (3) : scanf()에 입력받을 문자열 수 정해주기
```C
int main(){
    char buf[32];
    scanf("%15s", buf);
    //15개의 문자열만 받겠다고 지정해준다.
}
```
<hr/>

3. 기본 문자 집합에서는 문자들을 위해 char 자료형을 사용하라.   
   (예시 코드)
```C
#include <stdio.h>
#include <string.h>

int main(){
    char str1[] = "Hello, world";
    signed char str2[] = "Hello, world";
    unsigned char str3[] = "Hello, world";

    printf("%d\n", strlen(str1));
    printf("%d\n", strlen(str2));
    printf("%d\n", strlen(str3));
}
```
* 위 코드의 결과는 정상적으로 나오지만, char, signed char,   
  unsigned char는 서로 다른 타입이므로 호환이 가능하지 않다.
<hr/>

4. 문자열 상수를 가리키는 포인터는 const로 선언하라.   
   (예시 코드)
```C
#include <stdio.h>

int main(){
    char s1[] = "Hello, world";
    char *s2 = "Hello, world";

    s1[0] = ' ';  //정상적으로 동작
    printf("%s\n", s1);

    s2[0] = ' ';  //비정상적 동작
    printf("%s\n", s2);

    return 0;
}
```
* 위 코드의 s1은 Stack에 저장되고, RW(Read, Write) 영역에 저장된다.
* 하지만 s2는 RO(Read Only)영역에 저장되므로, 값을 바꿀 수 없다.
* 따라서 위 코드는 컴파일타임에는 오류가 없지만, 런타임에서   
  비정상적으로 동작, 종료되는 것이다.
* 위 오류를 컴파일타임에 발생하게 하기 위해선 s2를 const char*로 선언하면 된다.
<hr/>

5. 문자열 리터럴을 수정하려고 하지 마라.
* (4번)의 코드를 보면, s2는 R.O영역에 저장된다. 이러한 것을 문자열 리터럴이라 한다.
* C언어에서 R.O 영역의 것을 수정하려는 행위는 미정의 동작이다.
<hr/>

6. strtock()에서 파싱되는 문자열이 보존된다고 가정하지 말아라.   
   (예시 코드)
```C
#include <stdio.h>
#include <string.h>

int main(){
    char str[] = "Hello, world";
    
    // " , "를 구분자로 하여 토큰 생성
    char *token = strtok(str, ",");

    printf("%s\n", token);  // Hello 출력
    printf("%s\n", str);  // Hello 출력
}
```
* 위 코드의 결과 token을 출력하면 Hello가 출력되는 것은 당연하다.
* 하지만 str의 출력 결과는 왜 Hello일까?
  * strtok()는 delimiter를 찾으면, 그것을 ascii null로 치환한다.
  * 즉, 원본 문자열을 수정하는 것이다.
* 따라서 strtok()를 사용하려면 원본을 복사한 후, 복사본에 대해   
  strtok()를 호출해야 한다.
```C
#include <stdio.h>
#include <string.h>

int main(){
    char str[] = "Hello, world";
    char copy[32];
    strcpy(copy, str);

    char *token = strtok(copy, ",");
    printf("%s\n", str);  // Hello, world 출력
}
```
<hr/>

7. 문자열은 ascii null로 끝남을 보장하라.   
   (예시 코드)
```C
#include <stdio.h>
#include <string.h>
#define STR_SIZE (5)

int main(){
    const char str[] = "abcdefghijklmnopqrstuvwxyz";
    char dst[STR_SIZE];

    strncpy(dst, str, sizeof(dst));
    
    //이렇게 되면 dst에는 ascii null 이 없다.
    // 따라서 복사 완료 후 마지막에 ascii null을 지정해야 한다.

    dst[sizeof(dst) - 1] = '\0';

    printf("%s\n", dst);

    return 0;
}
```
<hr/>

8. null문자로 종료된 문자열이 부적절하게 잘리지 않게 하라.
<hr/>

9. 문자열 리터럴로 초기화된 문자 배열의 경계를 지정하지 마라.   
    (예시 코드)
```C
const char s[3] = "ABC";
```
* 문자열 리터럴 ABC의 크기가 4더라도, s의 크기가 3이므로 s에는   
  null문자가 저장되지 않는다.
* 이는 이후에 s를 null 종료 문자열처럼 사용한다면 취약성으로 발전할 수 있다.
* 따라서 다음과 같이 배열 경계를 지정하지 말아야 한다.
```C
const char s[] = "ABC";
```
