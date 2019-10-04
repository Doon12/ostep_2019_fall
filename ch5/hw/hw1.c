#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Q. fork()를 호출하는 프로그램을 작성하라. fork()를 호출하기 전에 메인 프로세스는
   변수에 접근하고 (예, x) 변수에 값을 지정하라 (예, 100). 자식 프로세스에서 그 변수의
   값은 무엇인가? 부모와 자식이 변수 x를 변경한 후에 변수는 어떻게 변했는가?
 */

/* A. 자식 프로세스에서 변수의 값은 부모 프로세스의 값과 같다.

   자식과 부모 프로세스에서 값을 바꾸면 각자 메모리 스페이스가 있기 때문에 각각 변한다.

   결과 :

	prent: x is 100
	child: x is 100
	parent: change x val to 99
	child: change x val to 101
	parent: x is 99
	child: x is 101 
 */

int main(void)
{
	int x = 100;
	int pid = fork();

	if (pid < 0)
	{
		printf("error\n");
	}
	else if (pid == 0) // child
	{	
		printf("child: x is %d\n", x);
		printf("child: change x val to 101\n");
		x = 101;
		printf("child: x is %d\n", x);


	}
	else // parent
	{	
		printf("parent: x is %d\n", x);
		printf("parent: change x val to 99\n");
		x = 99;
		printf("parent: x is %d\n", x);
	}
}
