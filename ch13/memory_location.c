#include <stdio.h>
#include <stdlib.h>

/*	주소 공간에서 코드가 제일 먼저 등장하고, 다음에 힙이 배치되고, 마지막으로
	대규모 가상공간의 반대편에 스택이 위치한다는 것을 알 수 있다.

	이 모든 주소는 가상 주소이며, 저장된 실제 물리 메모리 위치에서 값을 반입하기
	위해 운영체제와 하드웨어에 의해 물리주소로 변환된다.
 */

int main(int argc, char* argv[])
{
	printf("location of code : %p\n", (void*) main);
	printf("location of heap : %p\n", (void*) malloc(1));
	int x = 3;
	printf("location of stack: %p\n", (void*) &x);
	return x;
}
