#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

/* Q. 자식 프로세스를 생성하고 자식 프로세스가 표준 출력 (STDOUT_FILENO)을 닫는
   프로그램을 생성하라. 자식이 설명자를 닫은 후에 아무거나 출력하기 위하여
   printf()를 호출하면 무슨 일이 생기는가
 */ 
/* A.
  child process는 출력이 안된다. 하지만 parent process는 출력이 잘 된다.
  stdout은 process마다 배정이 되는 것 같다.

	result
	hi I am parent
 */

int main(void)
{
	pid_t pid = fork();

	if (pid < 0)
	{
		perror ("fork() error");
	}
	else if (pid == 0)
	{
		close (STDOUT_FILENO);
		printf("hi I am child\n");
	}
	else
	{
		wait(NULL);
		printf("hi I am parent\n");
	}
	
	return 0;
}
