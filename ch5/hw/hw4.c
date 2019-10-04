#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/* Q. fork()를 출력하고 /bin/ls를 실행하기 위하여 exec() 계열의 함수를 호출하는
   프로그램을 작성해라. exec()의 변형 execl(), execle(), execlp(), execv(),
   execvp(), execve() 모두를 사용할 수 있는지 시도해 보라. 기본적으로는
   동일한 기능을 수행하는 시스템 콜에 여러 변형이 있는 이유를 생각해보라. */

/* A.  
   잘 모르겠음. 링크

https://linuxhint.com/exec_linux_system_call_c/
	특이할 점 : execlp, execvp 등은 lib call.
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
		printf("child proc, print ls -lh home\n");
		char* binaryPath = "/bin/ls";
		char* arg1 = "-lh";
		char* arg2 = "/home";
		execlp( binaryPath, binaryPath, arg1, arg2, NULL);
	}
	else // parent
	{	
		printf("parent proc, print ls -lh home/junhan\n");
		char* binaryPath = "/bin/ls";
		char* arg1 = "-lh";
		char* arg2 = "/home/junhan";
		execlp( binaryPath, binaryPath, arg1, arg2, NULL);

	}

	return 0;
}
