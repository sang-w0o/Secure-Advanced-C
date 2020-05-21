C언어에서 가변 인자 함수 만들기
======
이 문서는 C언어에서 가변 인자 함수를 만드는 법과, 과연 센티널이 꼭 필요할지에 대해 생각한 것을 작성한 것이다.

* 먼저, 인수를 받고 그 인수들의 평균값을 구해주는 함수를 생각하자.

```C
int average(int a, int b, int c){
    return (a + b + c) / 3;
}
```

__위 함수의 단점__
* 인수가 4개, 5개 일 때마다 그 인수에 맞는 새로운 함수를 만들어야 한다는 단점이 있다.
* 따라서 다음에는 가변인자를 활용하여 함수를 구현해 보겠다.

<hr/>

```C
#include <stdio.h>
#include <stdarg.h>  //가변인자를 위해 참조
#define VA_END (-1)

//VA_END는 가변인자의 끝임을 알려주기 위해 사용한다.
//이와 같은 기호를 '센티널'이라 한다.

int average(int a, ...) {
    //1. 가변인자 리스트 선언
    va_list args;

    //2. 가변인자 리스트 초기화
    va_start(args, a);
    //args : 가변인자 리스트, a : 첫 번째 매개변수

    int cnt = 0;  //인수의 개수를 저장하는 변수
    int sum = 0;  //인수들의 총합을 저장하는 변수
    int i = a;  //가변인자를 임시로 저장하는 변수

    while(i != VA_END) {
        sum += i;
        ++cnt;
        i = va_arg(args, int);
        //va_arg() : 다음 인자를 읽어오는 함수
    }

    va_end(args);  //가변인자 리스트 Clean-Up

    //return시에는 average()가 호출될 때 VA_END 하나만 
    //인수로 들어올 수 있으므로 이를 검사한다.
    return cnt ? sum / cnt : 0;
}

int main(){
    printf("%d\n", average(10, 20, 30, VA_END));
}
```

__위 코드의 문제점__
* 함수 호출 시 VA_END를 마지막에 써주지 않으면, 컴파일타임 및 런타임에도 에러가 나지 않지만 잘못된 연산 결과가 나오는 버그가 있다.
* 다음 코드에서는 꼭 센티널을 사용해야하는지를 생각해보겠다.

<hr/>

위 코드에서 센티널은 가변인자의 끝임을 알려주기 위해 사용했다. C언어에서 가변인자를 사용하려면, 첫 번째 인수는 고정으로 필요한데, 이번에는 그러한 첫 번째 인수를 활용해서 센티널을 사용하지 않도록 해보자.

```C
#include <stdio.h>
#include <stdarg.h>

int average(int count, ...){
    if(count == 0)
        return 0;
    else if (count < 0)
        return -1;
    va_list args;
    va_start(args, count);
    int sum = 0;
    for(int i = 0; i < count; i++) {
        sum += va_arg(args, int);
    }
    va_end(args);
    return sum / count;
}

int main(){
    printf("%d\n", average(3, 20, 30, 40));
}
```
* 위 함수에서는 첫 번째 고정 인수인 count:int를 가변인자에 들어올 인수들의 개수로 활용했다.
* 위와 같은 방법으로 센티널을 사용하지 않고도 가변인자를 갖는 함수를 구현할 수 있었다.
