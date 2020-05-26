에러 처리 관련 C 시큐어 코딩 규칙
======

1. 표준 라이브러리에서 제공하는 에러 출력 함수를 사용하라.   
   (예시 코드 1)
```C
#include <stdio.h>

int main(){

    FILE *fp = fopen("filename", "r");
    char buf[4096];

    fgets(buf, sizeof(buf), fp);

    fputs(buf, stdout);

    fclose(fp);
    return 0;
}
```
* 위 코드는 작동하지 않는데, 사용자는 어디에서 에러가 발생했는지 알 수 없다.
* 따라서 오류 처리가 필요한데, fopen()이 실패 시 NULL을 반환하는 것을 이용해보자.   
  (예시 코드 2)
```C
#include <stdio.h>

int main(){

    FILE *fp = fopen("filename", "r");

    //fopen() 실패 시
    if(fp == NULL){
        fprintf(stderr, "fopen() error\n");
    }

    //이하 코드 (예시 코드 1)과 동일
}
```
* 위 코드는 fopen함수 실행 실패 시 실패되었다는 사실만 알 수 있지,   
  왜 오류가 발생했는지는 알 수 없다.
* 이런 경우를 위해 OS에서는 errno라는 전역 변수를 제공한다.
* errno는 정수 값으로, 어떤 함수가 왜 실패했는지를 알려주는 용도로 사용된다.
* errno는 errno.h 파일 안에 정의되어 있다.   
  (예시 코드 3)
```C
#include <stdio.h>
#include <errno.h>

int main(){
    FILE *fp = fopen("filename", "r");

	//fopen() 실패 시
	if (fp == NULL) {
		fprintf(stderr, "fopen() error, errno : %d",   errno);
		return -1;
	}

	char buf[4096];

	fgets(buf, sizeof(buf), fp);

	fputs(buf, stdout);

	fclose(fp);

	return 0;
}
```
* 위 코드는 fopen 실패 원인을 errno로 알 수 있지만, 사용자는 errno값만   
  알 수 있지, 그 errno값이 나온 원인은 알 수 없다.
* 이 때, 표준 라이브러리는 errno의 원인을 문자열로 반환해주는 strerror 함수를 제공한다.
* strerror함수는 string.h에 정의되어 있다.   
  (예시 코드 4)
```C
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main() {

	FILE *fp = fopen("filename", "r");

	//fopen() 실패 시
	if (fp == NULL) {
		fprintf(stderr, "fopen() error(%d) : %s", errno, strerror(errno));
		return -1;
	}

	char buf[4096];

	fgets(buf, sizeof(buf), fp);

	fputs(buf, stdout);

	fclose(fp);

	return 0;
}
```
* 위 코드는 이제 어떠한 이유로 fopen이 실패했는지 알 수 있다.
* 표준 라이브러리는 위와 같이 에러의 원인을 출력해주는 함수를 제공하는데, 바로 perror 함수이다.
* perror함수는 stdio.h내에 정의되어 있다.   
  (예시 코드 5)
```C
#include <errno.h>
#include <string.h>
#include <stdio.h>

int main() {

	FILE *fp = fopen("filename", "r");

	//fopen() 실패 시
	if (fp == NULL) {
		perror("fopen");
		return -1;
	}

	char buf[4096];

	fgets(buf, sizeof(buf), fp);

	fputs(buf, stdout);

	fclose(fp);

	return 0;
}
```
<hr/>

2. 에러 메시지 출력 시에는 stderr 스트림을 이용하라.
* 표준 C에는 다양한 스트림이 있지만, 그 중 대표적인 것들로는 stdin, stdout, stderr가 있다.   
* 먼저 stdin, stdout은 스트림 중간에 "버퍼"를 가진다.
* 하지만 stderr스트림은 버퍼가 없으므로, 오류로 인해 프로그램이 종료되더라도 출력이 되고 종료된다.

  (추가 정보)
* 버퍼의 용도 : 서로 다른 하드웨어 간의 처리 속도를 줄이기 위함.
* 버퍼의 플러시(정보 전달 후 버퍼를 비우는 행위) 방식
  * (1) full buffer : 버퍼가 가득차면 flush한다.
  * (2) line buffer : 개행문자를 만나면 flush한다.

* printf()는 stdin을 사용하며, 그 버퍼는 개행을 만나야 flush되는데,   
  stderr는 버퍼가 없으므로 바로바로 출력된다.
* 따라서 프로그램의 비정상적 종료를 대비하면 에러 메시지는   
  printf보다 fprintf(stderr)로 출력하는 것이 좋다.  
<hr/>

3. 함수가 호출된 시점에 프로세스를 종료하고 싶다면 exit함수를 사용해라.   
   (예시 코드)
```C
#include <stdio.h>
#include <stdlib.h>

int foo(){
    exit(0);
    printf("called foo()\n");
}

int main(){
    foo();
    printf("MESSAGE");
}
```
* 위 코드는 함수가 호출된 시점에 프로세스를 종료시킨다.
* 프로그램 종료 방법의 몇 가지를 알아보자.
  * (1) 메인 함수에서 정수 값 리턴
  * (2) exit()함수 호출
  * (3) abort()함수 호출 --> 비정상적 종료
<hr/>

4. 일관되고 이해할 수 있는 에러 처리 정책을 적용하고 구현하라.   
* 에러 코드를 반환하는 것도 좋은 방법이지만, 상황에 따라서는   
  특정 조건에 따라 프로그램을 종료시키는 것도 좋은 방법이다.
<hr/>

5. errno를 사용하는 라이브러리 함수를 호출하기 전에 errno 값을 0으로 설정하고,   
   함수가 에러를 의미하는 값을 반환했을 때는 errno값을 체크해라.
<hr/>

6. errno를 재정의하지 마라.
<hr/>

7. 애매한 errno값에 의존하지 마라.
<hr/>