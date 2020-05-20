swap 함수 구현하기
=================
이 문서는 swap함수를 단계별로 단점을 극복해가며 작성한 것이다.

* 모두가 아는 swap 함수
<pre><code>

void swap(void *a, void *b);

int main() {
	int a = 10, b = 20;
	swap(&a, &b);
	printf("a = %d, b = %d", a, b);
	return 0;
}

void swap(void *a, void *b) {
        int t = *(int *)a;
	*(int *)a = *(int *)b;
	*(int *)b = t;
}
</code></pre>

__단점__

*  위 swap함수는 int형만 swap이 가능하다.   
따라서 다음에는 모든 타입에 대하여 swap이 가능하도록 해보자.
<hr/>
<pre><code>
enum Type {  INT, DOUBLE };  //Type Code라 한다.
void swap(void *a, void *b, enum Type t) {
	switch (t) {
	case INT: {
		int tmp = *(int *)a;
		*(int *)a = *(int *)b;
		*(int *)b = tmp;
		break;
	}
	case DOUBLE: {
		double tmp = *(double *)a;
		*(double *)a = *(double *)b;
		*(double *)b = tmp;
		break;
	}
	}
}

int main() {
   int a = 10, b = 20;
   swap(&a, &b, INT);
	
   double c = 3.14, d = 5.14;
   swap(&c, &d, DOUBLE);
}
</code></pre>
C언어에는 Java의 Object처럼 모든 타입을 담을 수 있을 수 없기에   
위와 같이 열거형으로 타입을 주었다.

__단점들__

* (1) Type Code를 사용하면 항상 switch문을 이용하게 된다.
* (2) 새로운 Type Code를 부여하면, case문을 그에 맞게 추가해야하는 번거로움이 있다.
<hr/>
이번에는 Type Code 대신 dataSize:size_t를 swap의   
인수로 받아서 동적 할당을 통해 구현해보았다.
<pre><code>
void swap(void *a, void *b, size_t dataSize){
    void *t = malloc(dataSize);
    memcpy(t, a, dataSize);
    memcpy(a, b, dataSize);
    memcpy(b, t, dataSize);
    free(t);
}

int main() {
	int a = 10, b = 20;
	swap(&a, &b, sizeof(int));
	printf("a = %d, b = %d\n", a, b);
	double c = 3.14, d = 5.14;
	swap(&c, &d, sizeof(double));
	printf("c = %f, d = %f\n", c, d);
}
</code></pre>
위와 같이 세번 째 인수로 바꾸려는 자료형의 크기를 전달하면,
swap에서 void* t 를 해당 크기에 맞게 동적 할당하여
swap을 구현할 수 있었다.
<br/>

__단점들__

* (1) malloc()의 성공이 보장되지 않는다.
* (2) 함수 호출의 오버헤드가 발생할 수 있다.
* (3) OS측면에서 malloc()의 호출은 상당히 부담이 되는 작업이다.
<br/>
따라서 다음에는 동적 할당을 하지 않고 하는 방법을 생각해보자.
<hr/>
<pre><code>
#define SWAP(x, y, t) \
    (t) = (x);
    (x) = (y);
    (y) = (t);

int main() {
	int a = 10, b = 20, t1;
	SWAP(a, b, t1);
	printf("a = %d, b = %d\n", a, b);
	double c = 3.14, d = 5.14, t2;
   	SWAP(c, d, t2);
   	printf("c = %f, d = %f\n", c, d);
}
</code></pre>
이번에는 SWAP을 매크로 함수로 구현해보았다.
SWAP의 세번 째 인수인 t는 사용자로부터 받는 인수이다.

__단점__

* SWAP의 호출 회수만큼 해당 자료형의 메모리 변수를 선언해야 하므로 스택 메모리를 많이 차지하게 된다. 

<hr/>
이번에는 위의 스택 메모리를 절약하기 위해 매크로 함수를 조금 바꿔보았다.
<pre><code>
#define SWAP(x, y, T) {\
	T t = (x);\
	(x) = (y);\
	(y) = t; }

int main() {
	int a = 10, b = 20;
   	SWAP(a, b, int);
   	printf("a = %d, b = %d\n", a, b);
   	double c = 3.14, d = 5.14;
   	SWAP(c, d, double);
   	printf("c = %f, d = %f\n", c, d);
}
</pre></code>
위 코드에서는 SWAP의 세번 째 인자T에 swap할 자료형을 받게 했다.   
그리고 { }블록을 두어 t를 그 안에서 선언하므로써, 불필요한 스택 메모리의 사용을 없앴다.

__단점__

* Dangling-Else가 발생할 수 있다.
* <pre><code>
  int a = 10, b = 20;
  if(a > b){
    SWAP(a, b, int);
  }
  else{
      ;
  }

  //위와 같이 if문 안에 SWAP함수를 두면,
  //전처리기에 의해 변환된 if절의 마지막이
  // }; 로 끝나게 되는 문제점이 발생한다.
  </code></pre>

<hr/>
위 코드에서의 Dangling-else가 발생하는 현상을   
막기 위해서는 SWAP함수의 정의부를 do-while 문으로   
감싸주는 방법이 있다.
<pre><code>
#define SWAP(x, y, T) do{ \
    T t = (x);\
    (x) = (y);
    (y) = t; } while(0)
</code></pre>

__단점__

* 위 코드에서의 단점은 딱히 보이지 않았다. 
* 하지만 꼭 매크로 함수를 써야할까? 그리고, 일반 함수를   사용한다면 동적할당을 하지 않고 구현할 수는 없을까?
<hr/>

<pre style="width:500px"><code>
void swap(void *a, void *b, size_t dataSize) {
    char *pA = (char *)a;
    char *pB = (char *)b;
    
   for(size_t i = 0; i < dataSize; i++, pA++, pB++) {
        char t = *pA;
        *pA = *pB;
        *pB = t;
   }
}

int main(){
  	int a = 10, b = 20;
   	swap(&a, &b, sizeof(int));
   	printf("a = %d, b = %d\n", a, b);
   	double c = 3.14, d = 5.14, t2;
   	SWAP(c, d, t2);
   	printf("c = %f, d = %f\n", c, d);
}
</code></pre>

위 swap함수에서는 바꿀 것을 1Byte 단위로 꺼내와서   복사한다. 그러기 위해 char* 와 char을 이용했다.

__결과__

* 위 코드는 일반 함수를 이용해 완벽하게(?) 구현해낸 swap 함수이다.

<br/>
<h1>생각해보기</h1>
다음은 리눅스 v2.6.39.4에 있는 sort.c의 swap함수이다.
<pre><code>
static void generic_swap(void *a, void *b, int size) {
    char t;
    do {
        t = *(char *)a;
        *(char *)a++ = *(char *)b;
        *(char *)b++ = t;
    } while( --size > 0);
}
</code></pre>

__의문점__

* 위 코드는 GCC 컴파일러에서는 문제없이 작동하지만, Visual Studio C Compiler에서는 컴파일 타임에 오류가 발생한다. 이유가 뭘까..?
