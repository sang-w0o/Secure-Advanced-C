C언어의 함수 호출 규약
=====

<h2>#1. 기초 어셈블리어</h2>
<hr/>

* 먼저 nasm 컴파일러를 받고, main.c 파일을 만들자.
```C
// main.c
#include <stdio.h>

int asm_main(); 
//어셈블리어에서 함수명은 _asm_main이 된다.
//앞에 _가 붙는 것은 단순 문법이다.

int main(){
    int result = asm_main();
    printf("%d\n", result);

    return 0;
}
```

* 위 파일을 작성한 후, main.asm 파일을 추가하자.
* main.asm파일의 속성페이지의 "일반"에서 항목 형식을   
  "사용자 지정 빌드 도구"로 선택한다.
* "사용자 지정 빌드 도구"에서 명령줄에 다음과 같이 입력한다.
  * **nasm -f win32 -o first.obj first.asm**
* "출력" 부분에는 **first.obj**를 입력한다.

* (예시 코드 1)
```nasm
; 어셈블리어의 주석은 세미콜론(;) 이다.
segment .data           ;전역 변수를 선언하는 공간

segment .text           ;함수의 정보를 선언하는 공간
    global _asm_main    ;다른 파일(모듈)에서도 호출할 수 있도록 _asm_main함수를 전역으로 선언함.

_asm_main:              ; 함수 정의
    mov eax, 123456789  ; eax = 123456789
    ret                 ; 함수가 호출된 곳으로 복귀
```

* (예시 코드 2)
```nasm
segment .data
    num1 DD 100         ; int num1 = 100
        ; DD : Define DWORD(signed 4Byte 정수 타입)
    
    num2 DW 200         ; short num2 = 200;
        ; DW : Define WORD(signed 2Byte 정수 타입)
    
    str1 DB "HI", 0     ; "HI"를 str1에 저장
                        ; 0은 마지막을 null종료문자로 해준다.
    
segment .text
    global _asm_main

_asm_main:
    ; 어셈블리어에서 선언된 모든 심볼은 주소로 해석된다.
    mov dword[num1], 200    ; *((int*)num1) = 200
    mov eax, dword[num1]    ; eax = *((int*)num1)
    ret
```

* (예시 코드 3)
```nasm
segment .data

segment .text
    global _asm_main

_asm_main:
    jmp foo             ;foo 함수로 이동하라.
    jmp foo

next:                   ;next도 심볼이므로 주소로 해석된다.
    ret                 

foo:                    ;foo 함수 정의
    mov eax, 99999
    jmp next            ;next의 주소로 이동
```
* 위 코드는 foo함수를 2번 호출하려 했지만, 실제로는 1번만 호출된다.
* 그 이유는 복귀 주소가 next로 고정되어 있기 때문이다.

* (예시 코드 4)
```nasm
segment .data

segment .text
    global _asm_main

_asm_main:
    mov ebx, next1      ;함수 호출 전 복귀주소를 레지스터레 미리 저장
    jmp foo

next1:
    mov ebx, next2
    jmp foo

next2:
    ret

foo:
    mov eax, 9999
    jmp ebx
```
* 위 코드는 정상적으로 foo함수를 2번 호출하고 있다.
* 하지만 레지스터의 개수는 극히 한정적이므로, 복귀주소를   
  저장하는데에는 레지스터 보다 스택 메로리를 사용하는 것이 좋다.

* (예시 코드 5)
```nasm
segment .data

segment .text
    global _asm_main

_asm_main:
    call foo
    call foo
    ret

foo:
    mov eax, 12341234
    ret
```
* 위 코드는 복귀 주소를 스택 메모리에 저장하여   
  정상적으로 foo함수를 2번 호출했다.
<hr/>

<h2>#2. 함수 인자의 전달 방식</h2>
<hr/>

* 함수에 인자가 있다면, 이 인자를 함수에서 읽어와야 한다.
* 이 때, 인자는 레지스터에 저장할 수 있고, 스택 메모리에 저장할 수 있다.   
  아래 예시 코드에서는 10, 20이 인자로 들어왔다고 가정하자.

* (1) 레지스터에 인자를 저장하는 방법
```nasm
segment .data

segment .text
    global _asm_main

_asm_main:
    mov edx, 20             ; edx = 20
    mov ecx, 10             ; ecx = 10
    call add
    ret

add:
    mov eax, ecx            ; eax = ecx
    add eax, edx            ; eax += edx
    ret
```

* (2) 스택 메모리에 인자를 저장하는 방법
* 스택 메모리는 32bit 기준 0xFFFFFF부터 0x000000쪽으로,   
  즉 높은 번지에서 낮은 번지로 값을 저장해간다.
* 이 때, 스택에는 ESP가 존재하는데, ESP는 현재 스택에 저장된    
  데이터의 다음 칸(1단계 낮은 주소)를 가리키는 포인터이다.
```nasm
segment .data

segment .text
    global _asm_main

_asm_main:
    push 20         ; 스택에 20 저장
    push 10
    call add
    ret

add:
    mov eax, dword[esp+4]
    add eax, dword[esp+8]
    ret
```
* 위 코드는 비정상적으로 동작한다.
* 그 이유는 스택에 저장한 20, 10이라는 데이터를 해제하지 않았기 때문이다.
* 이 때 스택에 저장한 데이터를 해제하는 곳에 따라   
  함수 호출 방식이 갈려진다.
* cdecl방식 : 함수를 호출한 곳에서 인자들을 파기   
  (C언어 표준)
* stdcall방식 : 호출된 함수에서 인자들을 파기

* 다음은 cdecl 방식이다.
```nasm
segment .data

segment .text
    global _asm_main

_asm_main:
    push 20
    push 10
    call add

    add esp, 8          ;스택의 인자 데이터 파기
    ret

add:
    mov eax, dword[esp+4]
    add eax, dword[esp+8]
    ret
```

* 다음은 stdcall 방식이다.
```nasm
.segment data

.segment .text
    global _asm_main

_asm_main:
    push 20
    push 10
    call add
    ret

add:
    mov eax, dword[esp+4]
    add eax, dword[esp+8]
    ret 8               ;스택의 인자 데이터 파기
```
<hr/>
<h2>정리</h2>
<hr/>
<h3>함수 호출 시, 인자 전달에 사용된 스택 메모리를 해제하는 방식</h3>

* __cdecl : 호출자가 해제한다. (C 표준)
  * C DECLaration의 약자
  * 가변 인자를 구현할 수 있다.   
  (함수를 호출한 쪽에서 전달받은 인자의 개수를 알 수 있기 때문)
  * 함수 호출 종료 시 마다 스택을 해제하는 코드가 추가되므로   
  프로그램의 크기가 커질 수 있다.
* __stdcall : 피호출자가 해제하는 방식 (Win32 API 등)
  * 스택을 해제하는 코드가 함수내에 있으므로 프로그램의   
  크기가 상대적으로 작다는 장점이 있다.
  * 가변 인자 함수를 구현할 수 없다는 단점이 있다.
<hr/>
<h2>#3. 함수 호출 규약</h2>
<hr/>
(1) __cdecl 

* 인자 전달 : 오른쪽부터
* 스택 정리 : 호출한 곳
* 작명 규칙 : _함수명

(2) __fastcall

* 인자 전달 : 오른쪽부터
* 스택 정리 : 피호출 함수
* 작명 규칙 : @함수명@인자크기

(3) __stdcall

* 인자 전달 : ECX와 EDX에 저장 후, 나머지는 오른쪽부터
* 스택 정리 : 피호출 함수
* 작명 규칙 : _함수명@인자크기

(4) thiscall

* 인자전달 : 오른쪽부터 (객체 주소는 ECX에 저장)
* 스택 정리 : 피호출 함수
* 작명 규칙 : C++의 규칙을 따른다.

(5) naked

* 인자 전달 : 오른쪽부터
* 스택 정리 : 피호출 함수
* 작명 규칙 : 없음
