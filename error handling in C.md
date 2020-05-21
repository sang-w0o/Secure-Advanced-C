C언어에서 오류 처리하기
======
이 문서는 C언어에서 발생할 수 있는 기본적인 오류인 0으로 나누게 될 때 발생할 오류를 단계적으로 해결해가며 작성한 것이다.

* 가장 흔한 제수가 0일 때의 처리 방법

```C
int div(int a, int b){
    if(b == 0)
        return -1;
    return a / b;
}

int main(){
    int ret = div(4, 0);
    if(ret == -1){
        fprintf(stderr, "divide by zero\n");
        return -1;
    }
    printf("%d\n", ret);
    return 0;
}
```

__이 함수의 문제점__

* 관례적으로 C언어에서는 함수의 리턴값으로 종료 코드를 반환한다.
   * 종료 코드 0 : 함수의 정상적인 호출 및 수행
   * 종료 코드가 0이 아닐 때 : 함수 호출 실패 및 오류 발생

* 위 코드의 문제점은 리턴값이 연산의 결과인지, 종료 코드인지 구별할 수 없다는 것이다.
   * 제수가 0이 아닌데 연산 결과가 -1이 나올 경우를 생각하면 된다. 

<hr/>
이번에는 위 함수의 문제를 극복하기 위해, 결과값과 오류 코드를 분리해보자.

```C
int div(int a, int b, int *out) {
    
    //실패 시
    if(b == 0 || out == NULL)
        return -1;
    
    //성공 시
    *out = a / b;
    return 0;
}
```

__위 코드의 문제점__

* 위 코드는 또다시 사용자로 하여금 반환값을 조사하라고 알려주지 않는다.
* 즉, 함수가 오류코드를 반환하는지, 연산결과를 반환하는지 알 수 없다는 단점이 아직 완전히 극복되지 않았다.

<hr/>
관례적으로 오류 코드를 반환하는 경우에 개발자들은 error_t라는 사용자 정의 타입을 이용한다.

```C
typedef int error_t;

error_t div(int a, int b, int *out){
    if(b == 0 || out == NULL)
        return -1;
    *out = a / b;
    return 0;
}

int main(){
    int ret;

    //이제 사용자도 div의 리턴값이 오류코드인지 알 수 있다.
    if(div(4, 0, &ret) == -1) {
        fprintf(sterr, "divide by zero\n");
        return -1;
    }
    printf("%d\n", ret);
    return 0;
}
```

* 위와 같이 에러코드를 반환하는 함수는 반환타입을 typedef로 인코딩하여 사용자에게 알려주는 것이 좋다.
