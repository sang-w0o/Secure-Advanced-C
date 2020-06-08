비트맵 (Bitmap)
======


* 비트맵은 어떠한 시스템, 객체의 상태 정보를 저장하기 위해 사용되는 자료구조이다.

* 다음 코드를 보자.
```C
// 8개의 LED (0번부터 7번)에 대하여 상태 정보를 저장하는 코드
#include <stdio.h>

enum { LED_OFF, LED_ON };
int main() {
	
	int led[8] = { 0, };

	// 0번 led가 켜진 경우
	led[0] = LED_ON;

	// 3번 led가 켜진 경우
	led[3] = LED_ON;
	
	// LED 검사
	for (int i = 0; i < 8; i++) {
		if (led[i] == LED_ON)
			printf("%d LED : ON\n", i);
		else
			printf("%d LED : OFF\n", i);
	}
	getchar(), system("cls");

	// 3번 led가 꺼진 경우
	led[3] = LED_OFF;

	// LED 검사
	for (int i = 0; i < 8; i++) {
		if (led[i] == LED_ON)
			printf("%d LED : ON\n", i);
		else
			printf("%d LED : OFF\n", i);
	}
	getchar(), system("cls");
}
```

* 위 코드는 메모리 낭비가 매우 심하다는 문제점이 있다.
* 8개의 LED이므로 이에 맞게 1Byte (8bit)를 사용하는 char형을 이용하여,   
  1bit가 1개의 LED의 상태 정보를 저장하도록 해보자.
<hr/>

<h2>비트맵의 등장 배경</h2>

```C
#include <stdio.h>
#include <Windows.h>

int main() {
	
	char led = 0;

	// 0번 led가 켜진 경우
	// 0000 0000
	// 0000 0001 (결과)
	//led |= 0x1; 또는 아래 코드
	led |= (1 << 0);

	// 3번 led가 켜진 경우
	// 0000 0001
	// 0000 1001 (결과)
	//led |= 0x8; 또는 아래 코드
	led |= (1 << 3);
	
	// LED 검사
	for (int i = 0; i < 8; i++) {
		if ((led & (1 << i)))
			printf("%d LED : ON\n", i);
		else
			printf("%d LED : OFF\n", i);
	}
	getchar(), system("cls");

	// 3번 led를 끄는 경우
	led &= ~(1 << 3);

	// LED 검사
	for (int i = 0; i < 8; i++) {
		if ((led & (1 << i)))
			printf("%d LED : ON\n", i);
		else
			printf("%d LED : OFF\n", i);
	}
	getchar(), system("cls");
}
```

* 위와 같이 하면 1Byte의 각 1bit에 1개의 LED의 상태 정보를 저장했다.
* 하지만 위 코드는 가독성이 매우 떨어진다.
* 따라서 연산을 매크로 함수로 추상화해보자.

<hr/>

<h2>#1. 비트 연산의 추상화</h2>

```C
#include <stdio.h>
#include <stdlib.h>

#define BIT_SET(set, index) ((set) |= (1 << index))

#define IS_BIT_SET(set, index) (((set) & (1 << index)))

#define BIT_OFF(set, index) ((set) &= ~(1 << index))

int main() {
	
	char led = 0;
	// 0, 3번 LED 켜기
	BIT_SET(led, 0);
	BIT_SET(led, 3);
	
	// LED 검사
	for (int i = 0; i < 8; i++) {
		if (IS_BIT_SET(led, i))
			printf("%d LED : ON\n", i);
		else
			printf("%d LED : OFF\n", i);
	}
	getchar(), system("cls");

	// 3번 led를 끄는 경우
	BIT_OFF(led, 3);

	// LED 검사
	for (int i = 0; i < 8; i++) {
		if (IS_BIT_SET(led, i))
			printf("%d LED : ON\n", i);
		else
			printf("%d LED : OFF\n", i);
	}
	getchar(), system("cls");
}
```

* 위 자료구조는 상태 정보를 8개만 저장할 수 있다는 단점이 있다.
* 만약 128개의 상태정보를 저장하고 싶다면 어떻게 할까?
* unsigned int led[4]로 led변수를 선언하면 된다.
* 하지만 선언만 바꾸면 코드가 작동하지 않는다.
  * 배열에 대한 |, &, ^ 등의 연산은 불가하다. (배열명이 상수이기 때문)
* 위에서 선언한 led의 100번째만 켜고 싶다고 해보자.
  * 우선 100번째 bit가 포함되어 있는 행을 계산해야한다.
    * leds[100 / 32] = leds[3]  --> 3번째 행
  * 3번째 행의 몇 번째 bit에 있는지를 계산해야 한다.
    * lets[100 % 32] = 4 --> 4번째 비트
* 다음에는 위 과정을 도입해보자.
<hr/>

<h2>#2. 배열에 대한 비트 연산 구현</h2>

```C
#include <stdio.h>
#include <stdlib.h>

#define BIT_SET(set, index) ((set)[(index)/32] |= (1 << ((index)%32)))

#define IS_BIT_SET(set, index) ((set)[(index)/32] & (1 << ((index)%32)))

#define BIT_OFF(set, index) ((set)[(index)/32] &= ~(1 << ((index)%32)))

int main() {
	
	unsigned int led[4] = { 0, };
	// 100번 LED 켜기
	BIT_SET(led, 100);
	
	// LED 검사
	for (int i = 0; i < 128; i++) {
		if (IS_BIT_SET(led, i))
			printf("%d LED : ON\n", i);
		else
			printf("%d LED : OFF\n", i);
	}
	getchar(), system("cls");
}
```

* 하지만 위 자료구조는 매크로 함수에서 총 bit수가 고정되어 계산하기 때문에 사용할 수 없다.
* 따라서 타입을 추상화해야 한다.

<hr/>

<h2>#3. 타입의 추상화</h2>

```C
#include <stdio.h>
#include <stdlib.h>

#define BIT_SET(set, index) ((set)[(index)/32] |= (1 << ((index)%32)))

#define BIT_IS_SET(set, index) ((set)[(index)/32] & (1 << ((index)%32)))

#define BIT_OFF(set, index) ((set)[(index)/32] &= ~(1 << ((index)%32)))

typedef unsigned int bitset[4];

#define BITSET_INITIALIZER {0, }

int main() {
	
	bitset led = BITSET_INITIALIZER;
	// 100번 LED 켜기
	BIT_SET(led, 100);
	
	// LED 검사
	for (int i = 0; i < 128; i++) {
		if (BIT_IS_SET(led, i))
			printf("%d LED : ON\n", i);
		else
			printf("%d LED : OFF\n", i);
	}
	getchar(), system("cls");

	// LED 검사
	for (int i = 0; i < 128; i++) {
		if (BIT_IS_SET(led, i))
			printf("%d LED : ON\n", i);
		else
			printf("%d LED : OFF\n", i);
	}
	getchar(), system("cls");
}
```

* 위 자료구조는 한번 초기화 하면 재사용이 불가하다는 단점이 있다.
* 이를 해결하기 위해 기존의 정적 초기화가 아닌 동적 초기화 기능을 제공해야 한다.

<hr/>

<h2>#4. 동적 초기화 함수의 제공</h2>

```C
#include <stdio.h>
#include <stdlib.h>

#define BIT_SET(set, index) ((set)[(index)/32] |= (1 << ((index)%32)))

#define BIT_IS_SET(set, index) ((set)[(index)/32] & (1 << ((index)%32)))

#define BIT_OFF(set, index) ((set)[(index)/32] &= ~(1 << ((index)%32)))

#define BIT_ZERO(set) \
	do {for(int i = 0; i < 4; i++) (set)[i] = 0;} while(0)

typedef unsigned int bitset[4];

#define BITSET_INITIALIZER {0, }

int main() {
	
	bitset led = BITSET_INITIALIZER;
	// 100번 LED 켜기
	BIT_SET(led, 100);
	
	// LED 검사
	for (int i = 0; i < 128; i++) {
		if (BIT_IS_SET(led, i))
			printf("%d LED : ON\n", i);
		else
			printf("%d LED : OFF\n", i);
	}
	getchar(), system("cls");

	// LED 검사
	for (int i = 0; i < 128; i++) {
		if (BIT_IS_SET(led, i))
			printf("%d LED : ON\n", i);
		else
			printf("%d LED : OFF\n", i);
	}
	getchar(), system("cls");

	BIT_ZERO(led);
}
```

* 위 자료구조는 매크로 함수에서 나눗셈 연산을 사용하므로 성능 하락의 원인이 될 수 있는 요인이 있다.
* 따라서 shift연산과 비트 연산자를 이용해 이를 개선하도록 하자.

<hr/>

<h2>#5. 성능 하락 요인의 개선</h2>

```C
// 기타 코드 동일
#define BIT_SET(set, index) ((set)[(index)>>5] |= (1 << ((index)&31)))

#define BIT_IS_SET(set, index) ((set)[(index)>>5] & (1 << ((index)&31)))

#define BIT_OFF(set, index) ((set)[(index)>>5] &= ~(1 << ((index)&31)))
```

* 위 자료구조는 길이가 128bit로 고정되어 있다는 단점이 있다.
* 이를 해결하기 위해 사용자 정의형인 bitset를 일반화 해보자.

<hr/>

<h2>#5. 사용자가 원하는 만큼 bit를 할당하도록 하기</h2>

```C
#include <stdio.h>
#include <stdlib.h>

#define BIT_SET(set, index) ((set)[(index)>>5] |= (1 << ((index)&31)))

#define BIT_IS_SET(set, index) ((set)[(index)>>5] & (1 << ((index)&31)))

#define BIT_OFF(set, index) ((set)[(index)>>5] &= ~(1 << ((index)&31)))

#define BIT_ZERO(set) \
	do {for(int i = 0; i < 4; i++) (set)[i] = 0;} while(0)

//#define BITSET_DECLARE(set, bits) unsigned int set[(bits)/32 + 1]
// 위와 같이 하면, bits가 32일 때 set[2]가 되므로 32bit가 낭비된다.
// 따라서 요청한 크기(bits)를 나눌 때, 제수보다 하나 작은 값을 더하여 나누어 이를 해결한다.
#define DIV_ROUND_UP(n, d) (((n) + (d) -1) / (d))

#define BITSET_DECLARE(set, bits) unsigned int set[DIV_ROUND_UP(bits, 32)]

int main() {

	// 30 bit 할당
	BITSET_DECLARE(led, 30);

	BIT_ZERO(led);

	// 15번 LED 켜기
	BIT_SET(led, 15);

	// LED 검사
	for (int i = 0; i < 30; i++) {
		if (BIT_IS_SET(led, i))
			printf("%d LED : ON\n", i);
		else
			printf("%d LED : OFF\n", i);
	}
	getchar(), system("cls");

	BIT_ZERO(led);
}
```
* 위 자료구조는 32bit 이상을 할당받으면 제대로 동작하지 않는다는 단점이 있는데, 이는 해결할 수 있는 방법이 딱히 없다.
* 이렇게 비트 연산을 통해 효율적으로 상태를 저장하는 자료구조를 비트맵이라 한다.

* 이 repository에 모듈화를 하여 테스트 코드까지 업로드 해 놓았다.