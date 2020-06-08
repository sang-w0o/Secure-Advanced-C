#include <stdio.h>
#include <stdlib.h>
#include "Bitmap.h"

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