입력과 출력 관련 C 시큐어 코딩 규칙
======

1. 포맷 문자열을 사용할 때 주의하라.   
   (예시 코드)
```C
#include <stdio.h>

int main(){
    const char *msg = "This is message";
    int err_code = 3;

    printf("Message : %d(%s)\n", msg, err_code);
}
```
* 위 코드는 정상적으로 수행되지 않는다.
* 입출력 시 포맷 문자열에 알맞은 변수를 지정해야 한다.
<hr/>

2. 파일 관련 처리는 운영 체제가 제공하는 API를 사용하라.   
   (예시 코드)
```C
#include <stdio.h>

int main(){
    const char *fname = "C:\a.txt";  //이 파일이 존재한다고 가정하자.

    FILE *fp = fopen(fname, "wt");

    return 0;
}
```
* 위 코드의 문제점은 fopen()함수에 존재한다.
* fopen()함수는 파일을 열 때, 존재하는 파일에 대하여 읽기 전용으로   
  열었는지, 아니면 새로운 파일을 생성하여 연 것인지 알 수 없다.
* 따라서 Linux의 경우 open(), Windows의 경우 CreateFile() 등   
  운영체제가 제공하는 API를 사용하여 파일 처리를 해야한다.
<hr/>

3. rename() 함수를 사용 시 주의하라.
* rename()은 기존 파일을 새로운 이름으로 변경하는 함수인데, 만약 변경할 새로운 이름이   
이미 시스템에 존재할 경우 OS별로 처리법이 다르다.
  * Windows : rename()함수 호출 실패 처리
  * Linux : 이미 존재하는 파일 삭제
<hr/>

4. 같은 파일을 동시에 여러 번 열지 말아라.   
   (예시 코드)
```C
#include <stdio.h>
#define LOG_FILE "log.txt"

void foo(){
    File *log = fopen(LOG_FILE, "a");
    //...
}

int main(){
    FILE *log = fopen(LOG_FILE, "a");
    foo();
}
```
* 하나의 파일을 동시에 열 경우, 이에 대한 구현은   
  컴파일러마다 다르게 정의되어 있다.
<hr/>

5. 문자 I/O의 반환 값을 사용할 때는 int를 사용하라.   
   (예시 코드)
```C
#include <stdio.h>

int main(){

    //파일의 크기를 구하고 싶다.

    FILE *fp = fopen("C:\name.txt", "rb");
    if(fp == NULL){
        fprintf(stderr, "fopen() failed.\n");
        return -1;
    }

    size_t fSize = 0;
    int ch = fgetc(fp);

    while(ch != EOF){
        ++fSize;
        ch = fgetc(fp);
    }

    printf("File size : %d\n", fSize);
    return 0;
}
```
* fgetc는 파일 데이터를 1Byte씩 읽어온다.
* 그리고 파일의 끝을 만나면 EOF (-1)을 반환한다.
* fgetc는 먼저 파일로부터 읽어온 1Byte를 unsigned char형으로 변환한다.
   * 0xFF를 읽어오면 이를 unsigned char로 변환한다.
* 그 후 이를 4Byte의 signed int로 확장한다.
  * 0x000000FF --> 255 반환
* 만약 fgetc(fp)를 char형으로 받으면 0xFF를 만났을 때 -1를 반환하는데,   
  이를 EOF와 구분할 방법이 없어진다.
<hr/>

6. fgets()를 사용할 때 개행 문자가 읽힌다고 가정하지 마라.   
   (예시 코드)
```C
#include <stdio.h>
#include <string.h>

int main(){
    char buf[32];
    for(int i = 0; i < 32; i++)
        buf[i] = 'X';

    if(fgets(buf, sizeof(buf), stdin)){
        for(int i = 0; i < 32; i++){
            printf("%d ", buf[i]);
        }
        printf("\n");
    }

    else{
        printf("fgets() error.\n");
        return -1;
    }

    return 0;
}
```
* fgets()는 보통 입력 스트림으로 부터 개행 문자로   
  종료된 문자열을 읽기 위해 사용한다.
* fgets()는 실패할 경우 NULL을 반환한다.
* fgets()로 버퍼보다 작은 값을 읽는 경우
  * 개행문자까지 읽어오며, 끝은 ASCII NULL로 설정한다.
* fgets()로 버퍼의 크기와 같거나 더 큰 값을 읽는 경우
  * 개행은 읽지 않으며 마지막 문자만 ASCII NULL로 설정한다.
* 만약 ascii null 한문자와 개행문자만 fgets()가 읽는다면 이에 따른 처리를 해야한다.   
  ```C
  char buf[32];
  if(fgets(buf, sizeof(buf), stdin)){

      //ABCD가 입력되면 buf는 다음과 같다.
      // 0  1  2  3  4  5
      //[A][B][C][D][10][0]

      //buf가 null이 아닌 문자로 시작하는 경우
      if(*buf){
          //buf에서 개행을 지워준다.
          buf[strlen(buf) - 1] = '\0';
          printf("%s", buf);
      }
  }
  else{
      printf("fgets() error.\n");
      return -1;
  }
  return 0;
  ```

```C
#include <stdio.h>
#include <string.h>

int main(){
    char buf[5];

    if(fgets(buf, sizeof(buf), stdin) != NULL) {
        buf[strlen(buf) - 1] = '\0';

        //값 찍어보기
        for(int i = 0; i < 5; i++){
            printf("[%d]", buf[i]);
        }
        printf("\n");
        // ABC 입력 시
        // [65] [66] [67] [0] [0] 출력

        // ABCDE 입력 시
        // [65] [66] [67] [0] [0] 출력
        // --> D의 데이터가 유실된다!
    }
}
```
* 위에서 fgets로 읽어들인 버퍼에서 개행문자를 무조건 지우면   
  데이터가 유실될 수도 있음을 알 수 있다.
* 따라서 추가적인 조치가 필요하다.
* [strchr() : 해당 문자열에 특정 문자가 있는지 확인하는 함수]를 이용해보자
```C
#include <stdio.h>
#include <string.h>

int main(){
    char buf[5];

    fgets(buf, sizeof(buf), stdin);

    //buf에 개행이 있는지 검사
    char *p = strchar(buf, '\n');
    
    //개행이 있는지 검사
    //없다면 p는 null이므로 if문에 안들어간다.
    if(*p){
        *p = '\0';
    }

    for(int i = 0; i < 5; i++){
        printf("[%d] ", buf[i]);
    }
    printf("\n");

    return 0;
}
```
<hr/>

7. fgets() 실패 시 버퍼를 NULL 문자열로 리셋하라.
* fgets함수 실패 시, 인자로 전달된 버퍼는 알 수 없는   
  상태로 정의되므로 사용하면 안된다.   
  (예시 코드)
```C
#include <stdio.h>

int main(){
    
    char buf[5] = {0, };
    
    //fgets() 실패 시
    if(fgets(buf, sizeof(buf), stdin) == NULL) {
        fprintf(stderr, "fgets() error");
        *buf = '\0';
    }
}
```
<hr/>

8. 입출력 FILE 객체를 복사해 사용하지 마라.   
   (예시 코드)
```C
#include <stdio.h>

int main(){
    
    //stdout은 FILE* 형이다.
    FILE *fp = stdout;
    fputs("HELLO", fp);  //정상 수행

    FILE file = *stdout;
    fputs("HELLO", &file);
    // 비정상적 수행
}
```
* 위 코드의 결과를 보면 두번째 fputs() 호출 시 프로그램이 비정상 종료된다.
* 그 이유는 stdout의 복사본을 사용하는 것 이기 때문이다.
<hr/>
