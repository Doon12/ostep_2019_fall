#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>

/* Q. 두 개의 자식 프로세스를 생성하고 pipe() 시스템 콜을 사용하여 한 자식의 표준
   출력을 다른 자식의 입력으로 연결하는 프로그램을 작성하라.
 */ 
/* A.
   -- pipe 함수 설명 --
   int pipe(int fds[2]);

Parameters :
fd[0] will be the fd(file descriptor) for the 
read end of pipe.
fd[1] will be the fd for the write end of pipe.
Returns : 0 on Success.
-1 on error

-- 참고 사이트 --
www.geeksforgeeks.org/pipe-system-call/

-- 결과 --

CHILD1 => print in to stdin : CHILD2 => print out to stdout : HELLO WORLD!
END~

END~
END~


 */
#define BUFSIZE	100

int main(int argc, char *argv[]) {
	int fd[2];
	char str[BUFSIZE];
	char buf[] = "HELLO WORLD!";
	if(pipe(fd)){
		perror("pipe");
		return -1;
	}
	switch(fork()){
		case -1:
			perror("fork1");
			return -1;
		case 0:
			// child 2
			close(fd[0]);
			dup2(fd[1], STDOUT_FILENO); // redirect stdout to fd[1]
			printf("CHILD2 => print out to stdout : %s\n", buf);
			close(fd[1]);
			break;
		default:
			// parent
			wait(NULL);
			switch(fork()){
				case -1:
					perror("fork2");
					return -1;
				case 0:
					// child 1
					close(fd[1]);
					dup2(fd[0], STDIN_FILENO); // redirect fd[0] to stdin
					read(STDIN_FILENO, str, BUFSIZE);
					printf("CHILD1 => print in to stdin : %s\n", str);
					close(fd[0]);
					break;
				default:
					//parent
					wait(NULL);
					break;
			}			
	}
	printf("END~\n");
	return 0;
}  
