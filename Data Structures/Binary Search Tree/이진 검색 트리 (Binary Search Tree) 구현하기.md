이진 검색 트리 (Binary Search Tree) 구현하기
======

<h2>Tree의 기본 용어</h2>

* Tree는 왼쪽, 오른쪽으로 나누어 탐색을 한다.
* Tree의 최소단위는 Node이다.
* Node들은 부모 - 자식 관계로 구성되어 있다.
  * 최상위 Node (__Root Node__)만이 부모 Node가 없다.
  * 자식이 없는 노드를 __Leaf Node__ (단말 노드) 라 한다.
  * Root Node와 Leaf Node사이의 노드를 __내부 노드__ 라 한다.
  * 공통 부모 노드를 가지는 자식 노드들은 __형제 노드__ 라 한다.
  * 자식 노드 기준으로 부모 노드의 형제를 __삼촌 노드__(?) 라 한다.
  * 자식 노드 기준으로 부모 노드의 부모 노드를 __조부모 노드__ 라 한다.
* 레벨 : __Root Node 가 level 0__  이며, 자식으로 내려갈 수록 레벨이 1씩 증가한다.
* Tree 검색의 핵심은 level(높이)를 줄이는 것이다.
* Binary Search Tree : 자식 노드의 개수가 최대 2개인 트리 구조
  * 장점 : 검색이 빠르다.
  * 대부분 부모 노드의 data를 기준으로 자식 노드의 data가 더 작으면   
    왼쪽 자식으로, 크거나 같으면 오른쪽 자식으로 삽입한다.
  * 조건 : 각 Node의 data는 서로 다르다.
<hr/>

<h2>#1. Node 설계 하기</h2>

```c
typedef struct Node{
  int data;
  struct Node *left;
  struct Node *right;
}Node;

Node *root;  // Root Node
```

<hr/>

<h2>#2. Node 삽입 함수 구현하기</h2>

```C
int bstInsert(int data){
  Node *node = calloc(1, sizeof(Node));
	if (node == NULL) {
		fprintf(stderr, "bstInsert: malloc failed.\n");
		return -1;
	}
	node->data = data;
	
	// tree가 비어있는 상태라면,
	// 즉 처음 생성되는 node라면
	if (root == NULL) {
		root = node;
		return 0;
	}

	// root가 존재한다면
	Node *parent = NULL;
	Node *cur = root;
	while (cur != NULL) {
		parent = cur;
		if (data < cur->data) {
			cur = cur->left;
		}
		else if (data > cur->data) {
			cur = cur->right;
		}
    // 부모 노드와 새로운 노드의 data가 같을 때
    // 삽입을 하지 않는다.
		else {
			free(node);
			return -1;
		}
	}

	if (data < parent->data)
		parent->left = node;
	
	else
		parent->right = node;
	
	return 0;
}

int main(){
  int arr[8] = { 4,2,1,3,6,5,7,8 };
  for(int i = 0; i < 8; i++) {
    bstInsert(arr[i]);
  }
}
/*       root-->   4
                  / \
                 2   6
                / \  / \
               1  3  5  7
                         \
                          8  */
```
<hr/>

<h2>#3. Tree의 순회 구현</h2>

* Tree를 순회하는 대표적인 3가지는 다음과 같다.
  * 전위 순회 : 노드 방문 --> 왼쪽 노드로 이동 --> 오른쪽 노드로 이동
  * 중위 순회 : 왼쪽 노드로 이동 --> 노드 방문 --> 오른쪽 노드로 이동
  * 후위 순회 : 왼쪽 노드로 이동 --> 오른쪽 노드로 이동 --> 노드 방문

* (1) 전위 순회 (preorder)의 구현
```C
static void _preorder(Node *node) {

	// 종료조건 : node가 NULL일 시
	if (node == NULL)
		return;

	// 노드 방문 --> 왼쪽 노드로 이동 --> 오른쪽 노드로 이동
	printf("%2d", node->data);
	_preorder(node->left);
	_preorder(node->right);
}

void preorder() {
	printf("preorder : ");
	_preorder(root);
	printf("\n");
}  // 4 2 1 3 6 5 7 8 출력
```

  * 전위 순회의 결과는 별다른 의미가 없다.

* (2) 중위 순회 (inorder)의 구현
```C
static void _inorder(Node *node) {
	if (node == NULL)
		return;

	// 왼쪽 노드로 이동 --> 노드 방문 --> 오른쪽 노드로 이동
	_inorder(node->left);
	printf("%2d", node->data);
	_inorder(node->right);
}

void inorder() {
	printf("inorder : ");
	_inorder(root);
	printf("\n");
}  // 1 2 3 4 5 6 7 8 출력
```

  * 중위 순회의 결과는 노드들의 데이터가 오름차순으로 정렬되어 나타난다.

* (3) 후위 순회 (postorder)의 구현
```C
static void _postorder(Node *node) {
	if (node == NULL)
		return;

  // 왼쪽 노드로 이동 --> 오른쪽 노드로 이동 --> 노드 방문
	_postorder(node->left);
	_postorder(node->right);
	printf("%2d", node->data);
}

void postorder() {
	printf("postorder : ");
	_postorder(root);
	printf("\n");
}  // 1 3 2 5 8 7 6 4 출력
```

  * 후위 순회의 결과는 가장 아래 노드부터 루트까지 올라가며 검색하는 것이다.
  * 이는 후에 Node들에 대한 메모리 해제 순서이기도 하다.

<hr/>

<h2>#4. 트리의 data들을 출력해주는 함수의 구현</h2>

```C
static void fillArray(int(*arr)[10], int *row, int *col, Node *node) {
	if (node == NULL)
		return;

	++(*row);

	// 왼쪽 노드로 이동
	fillArray(arr, row, col, node->left);
	arr[*row][(*col)++] = node->data;

	// 오른쪽 노드로 이동
	fillArray(arr, row, col, node->right);
	--(*row);
}

void bstDisplay() {
	// 중위 순회 이용
	// 출력되는 데이터마다 임의의 좌표를 부여한다.
	int arr[10][10] = { 0, };
	int row = -1, col = 0;
	system("cls");
	fillArray(arr, &row, &col, root);

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (arr[i][j])
				printf("(%2d)", arr[i][j]);
			else
				printf("%4c", ' ');
		}
		printf("\n");
	}
	getchar();
}
```

<hr/>

<h2>#5. Tree의 데이터 검색 함수 구현</h2>

```C
int bstGet(int key) {
	if (root == NULL)
		return -1;

	Node *cur = root;
	while (cur != NULL) {
		if (key < cur->data)
			cur = cur->left;
		else if (key > cur->data)
			cur = cur->right;
		else
			return cur->data;
	}
	return -1;
}
```
<hr/>

<h2>#6. Tree의 노드의 삭제 구현</h2>

* (1) 삭제될 노드가 leaf node인 경우
  * 해당 노드를 삭제하고, 부모 노드가 해당 노드를 가리키는   
    left 또는 right 포인터를 NULL로 설정해야 한다.
* (2) 삭제될 노드가 1개의 자식 노드를 가지고 있는 경우
  * 해당 노드를 삭제하고, 삭제된 노드가 가진 자식 노드를 삭제된 노드의   
    부모 노도의 자식 노드로 연결해야 한다.
* (3) 삭제될 노드가 2개의 자식 노드를 가지고 있는 경우
  * 삭제될 노드를 대체할 후보 노드를 찾아야 한다.
  * 후보 노드의 선택 방법 (1): 삭제될 노드 기준 좌측 서브 트리에서 data가 가장 큰 노드를 선택한다.
  * 후보 노드의 선택 방법 (2) : 삭제될 노드 기준 우측 서브 트리에서 data가 가장 작은 노드를 선택한다.
    * 우선 삭제될 노드의 값을 후보 노드의 값으로 교체한다. 
    * 만약 후보 노드가 우측 자식 노드가 있다면, 후보 노드의 부모 노드가
      그 노드를 입양해야 한다.
    * 따라서 삭제될 노드의 우측 서브트리를 삭제될 노드의 부모 노드의 왼쪽에 연결해야 한다.

```C
int bstRemove(int key, int *out) {
	if (root == NULL) {
		fprintf(stderr, "bstRemove : root is NULL.\n");
		return -1;
	}

	// 우선 삭제할 노드를 찾는다.
	Node *parent = NULL;
	Node *target = root;
	while (target != NULL && target->data != key) {
		parent = target;
		if (key < target->data)
			target = target->left;
		else if (key > target->data)
			target = target->right;
	}

	// 삭제할 data를 가진 노드를 발견하지 못한 경우
	if (target == NULL) {
		fprintf(stderr, "bstRemove : Node doesn't exist.");
		return -1;
	}

	// (1) 삭제될 노드가 leaf node인 경우
	if ((target->left == NULL) && (target->right == NULL)) {

		// 부모 노드가 존재하는 경우
		if (parent != NULL) {

			// 삭제 대상이 부모 노드의 left자식일 경우
			if (target == parent->left) {
				parent->left = NULL;
			}

			// 삭제 대상이 부모 노드의 right자식일 경우
			else
				parent->right = NULL;
		}

		// 부모 노드가 존재하지 않는 경우
		// 즉, 삭제할 노드가 root노드일 경우
		else {
			root = NULL;
		}
		free(target);
		return 0;
	}

	// 삭제될 노드가 자식 노드 1개만 가질 경우
	else if ((target->left == NULL) || (target->right == NULL)) {

		// 삭제될 노드의 자식 포인터를 백업한다.
		Node *child;
		// left자식이 없다면 right자식 포인터 저장
		if (target->left == NULL)
			child = target->right;
		// right자식이 없다면 leftt자식 포인터 저장
		else
			child = target->left;

		// 자식 노드를 삭제될 노드의 부모 노드가 연결하게 한다.
		if (parent != NULL) {
			// 삭제될 노드가 부모 노드의 left자식인 경우
			if (target == parent->left)
				parent->left = child;
			// 삭제될 노드가 부모 노드의 rightt자식인 경우
			else
				parent->right = child;
		}

		// 삭제될 노드가 root일 경우
		else{
			root = child;
		}
	}

	// 삭제될 노드가 left, right 자식을 모두 가질 경우
	else {
		// 후보 node의 부모 노드 cParent
		Node *cParent = target;
		Node *candidate = target->right;
		
		// root기준 우측 트리의 가장 작은 값을 
		// candidate가 되게 한다.
		while (candidate->left != NULL) {
			cParent = candidate;
			candidate = candidate->left;
		}

		// candidate의 우측 자식을 candidate의 부모의 left자식으로 연결

		// cParent의 left가 candidate의 right자식 노드를 가리키게 한다.
		if (cParent->left == candidate)
			cParent->left = candidate->right;

		// 위의 while문을 돌지 않은 경우
		// 즉, 후보 노드의 left자식이 없는 경우
		else
			cParent->right = candidate->right;

		// 삭제될 노드의 data값을 후보 노드의 data로 교체한다.
		int temp = target->data;
		target->data = candidate->data;
		candidate->data = temp;
		target = candidate;
	}

	*out = target->data;
	free(target);
	return 0;
}
```

<hr/>

<h2>#7. 모든 타입 저장 가능하게 하기 및 데이터의 추상화, 지역 변수화</h2>

* 이 과정은 Node의 data를 void* 형으로 바꾸고, 전역 변수를 없애고 Tree라는 추상회된 타입을 정의하는 것이다.
* 이 과정은 이전 자료구조 구현 시 충분히 했으므로 생략하겠다.
* 이 repo에 BST의 라이브러리 코드와 테스트 코드를 모듈화하여 올려놓았다.