#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

/* Q. wait() 대신 waitpid()를 써라. 언제 이게 더 좋은가?
 */ 
/* A.

   wait 하는 프로세스의 상태를 파악할  수 있고,
   기다리는 동안 다른 동작을 진행할 수 있다.

   result

   ./hw6
   child is still running at Wed Sep 11 11:01:19 2019
   child is still running at Wed Sep 11 11:01:20 2019
   child is still running at Wed Sep 11 11:01:21 2019
   child is still running at Wed Sep 11 11:01:22 2019
   child is still running at Wed Sep 11 11:01:23 2019
   child exited with status of 1

 */

int main(void)
{
	pid_t pid;
	time_t t;
	int status;

	if ((pid = fork()) < 0)
		perror ("fork() error");
	else if (pid == 0)
	{
		sleep(5);
		exit(1);
	}
	else do
	{
		if ((pid = waitpid(pid, &status, WNOHANG)) == -1)
			perror("wait() error");
		else if (pid == 0)
		{
			time(&t);
			printf("child is still running at %s", ctime(&t));
			sleep(1);
		}
		else
		{
			if (WIFEXITED(status))
				printf("child exited with status of %d\n", WEXITSTATUS(status));
			else
				puts("child did not exit successfully");
		}
	} while (pid == 0);

	
	return 0;
}
