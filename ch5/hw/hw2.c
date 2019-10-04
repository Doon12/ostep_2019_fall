#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* Q. open() 시스템 콜을 사용하여 파일을 여는 프로그램을 작성하고 새 프로세스를
   생성하기 위하여 fork()를 호출하라. 자식과 부모가 open()에 의해 반환된 파일
   디스크립터에 접근할 수 있는가? 부모와 자식 프로세스가 동시에 파일에 쓰기 작업을
   할 수 있는가?
 */

/* A. 동일한 file descriptor에 접근할 수 있다. 그리고 동시에 파일에 쓰기 작업을
   할 수 있다.

result :

 I am parent ,fd is 3
 I am child ,fd is 3
 wr parent is 13
 wr child is 12
 junhan@DESKTOP-A0DKAKE:~/os_2019_fall/ch5/hw$ ls
 -pthread  hw1  hw1.c  hw2  hw2.c  hw2.txt
 junhan@DESKTOP-A0DKAKE:~/os_2019_fall/ch5/hw$ cat hw2.txt
 hello parent
 hello child<F9>
 */

int main(void)
{
	int fd = open ("hw2.txt", O_RDWR | O_CREAT);
	int pid = fork();
	
	if (pid < 0)
	{
		printf("error\n");
	}
	else if (pid == 0) // child
	{
		printf("I am child ,");
		printf("fd is %d\n", fd);
		// let's write
		char* char_child;
		char_child = "hello child\n";
		ssize_t wr = write (fd, (void*)char_child, 12);
		printf("wr child is %ld\n", wr);

	}
	else // parent
	{
		printf("I am parent ,");
		printf("fd is %d\n", fd);
		// let's write
		char* char_parent;
		char_parent = "hello parent\n";
		ssize_t wr = write (fd, (void*)char_parent, 13);
		printf("wr parent is %ld\n", wr);
	}
}
