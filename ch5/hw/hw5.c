#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

/* Q. wait()를 사용하여 자식 프로세스가 종료되기를 기다리는 프로그램을 작성하라.
   wait()가 반환하는 것은 무엇인가? 자식 프로세스가 wait()를 호출하면 어떤
   결과가 발생하는가?
 */ 
/* A.

   result 1. parent가 child 기다릴 때.
   child proc
   ret for wait : 824 ----> pid of child
   parent proc

   result 2. child가 parent 기다릴 때.
   parent proc
   ret for wait : -1
   child proc는 종료되지 않음.
   shell은 끝남. shell은 fork한뒤 parent에서 child를 기다리고 있음.
   shell의 child : main의 parent이며 main의 parent가 끝났기 때문에 shell은 종료됨.
 */

int main(void)
{
	int pid = fork();

	if (pid < 0)
	{
		printf("error\n");
		abort();
	}
	else if (pid == 0) // child
	{	
		int ret = wait(NULL);
		printf("ret for wait : %d\n", ret);
		printf("child proc: pid %d\n", pid);
	}
	else // parent
	{	
		//int ret = wait(NULL);
		//printf("ret for wait : %d\n", ret);
		printf("parent proc: pid %d\n", pid);
	}

	return 0;
}
