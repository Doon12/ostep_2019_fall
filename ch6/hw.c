#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/time.h>
#include <errno.h>
#include <assert.h>
#include <sched.h>
#include <stdbool.h>

/* Q. 시스템 콜과 문맥 교환의 비용 측정하기.
   step 1. syscall	
   비용이 적게 드는 syscall 여러번 호출한 후 횟수로 나누기.
   step 2. context switching\

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
void print_affinity(char* name);

int main(int argc, char *argv[]) {
	
	printf("---------- measure syscall time -----------\n");
	int syscallCnt = 10000; // how many time syscall call repeatively.
	int i;
	int fd_empty = 0;
	char str[BUFSIZE];
	struct timeval t0, t1;
	gettimeofday(&t0, 0);
	for(i=0; i<syscallCnt; i++)
		read(fd_empty, str, 0); // read 0 byte
	gettimeofday(&t1, 0);
	long elapsed_total_us = (t1.tv_sec - t0.tv_sec) * 1000000 + t1.tv_usec - t0.tv_usec;
	float elapsed_avg = elapsed_total_us / (float)syscallCnt;
	printf("syscall elapsed : %f us\n", elapsed_avg);
	printf("---------- measure syscall done -----------\n\n\n");


	printf("---------- measure context switching time -----------\n");
	// how many time fd1[0] and fd1[1] echo messages repeatively.
	int commCnt = 10000; 	
	// message
	char buf1[] = "MSG1";
	char buf2[] = "MSG2";


	// step1. read/write in one proc.
	struct timeval t_start, t_finish;
	int f1, f2;
	f1 = open("foo.txt", O_RDWR);
	f2 = open("bar.txt", O_RDWR);
	// start
	gettimeofday(&t_start, 0);

	int sz1 = write(f1, buf1, sizeof(buf1));
	sz1 = read(f1, str, BUFSIZE);
	int sz2 = write(f2, buf2, sizeof(buf2));
	sz2 = read(f2, str, BUFSIZE);

	// finish
	gettimeofday(&t_finish, 0);

	// measure time
	long elapsed_total_us_1 = (t_finish.tv_sec - t_start.tv_sec) * 1000000 + t_finish.tv_usec - t_start.tv_usec;
	float elapsed_avg_1 = elapsed_total_us_1 ;
	printf("two read/write elapsed : %f us\n", elapsed_avg_1);



	// declare pipe.
	int fd1[2];
	int fd2[2];

	if(pipe(fd1)){
		perror("pipe1 err");
		return -1;
	}

	if(pipe(fd2)){
		perror("pipe2 err");
		return -1;
	}

	switch(fork()){
		case -1:
			perror("fork1");
			return -1;
		case 0:
			;
			// child 2 process.
			cpu_set_t mask;
			print_affinity("child2");
			printf("sched_getcpu2 = %d\n", sched_getcpu());
			CPU_ZERO(&mask);
			CPU_SET(3, &mask);
			if (sched_setaffinity(0, sizeof(cpu_set_t), &mask) == -1)
			{
				perror("sched_setaffinity");
				assert(false);
			}
			print_affinity("child2");
			printf("sched_getcpu2 = %d\n", sched_getcpu());
			// now child2 uses cpu3 only.
			
			struct timeval t_context_1, t_context_2;

			// start
			gettimeofday(&t_context_1, 0);


			close(fd1[0]);
			close(fd2[1]);

			// write1
			//dup2(fd1[1], STDOUT_FILENO); // redirect stdout to fd1[1]
			//printf("write1 : %s\n", buf1); 
			write(fd1[1], buf1, sizeof(buf1));
			close(fd1[1]);

			// read2
			//dup2(fd2[0], STDIN_FILENO); // redirect fd1[0] to stdin
			//read(STDIN_FILENO, str, BUFSIZE);
			read(fd2[0], str, BUFSIZE);
			printf("read2: %s\n", str);
			close(fd2[0]);


			// finish
			gettimeofday(&t_context_2, 0);

			// measure time
			long elapsed_total_us_c = (t_context_2.tv_sec - t_context_1.tv_sec) * 1000000 + t_context_2.tv_usec - t_context_1.tv_usec;
			float elapsed_avg_c = elapsed_total_us_c ;
			printf("total context switch elapsed : %f us\n", elapsed_avg_c);
			printf("context switch only: %f us\n", elapsed_avg_c - elapsed_avg_1);

			break;
		default:
			// parent
			switch(fork()){
				case -1:
					perror("fork2");
					return -1;
				case 0:
					;
					// child 1 process.
					cpu_set_t mask;
					print_affinity("child1");
					printf("sched_getcpu1 = %d\n", sched_getcpu());
					CPU_ZERO(&mask);
					CPU_SET(3, &mask);
					if (sched_setaffinity(0, sizeof(cpu_set_t), &mask) == -1)
					{
						perror("sched_setaffinity");
						assert(false);
					}
					print_affinity("child1");
					printf("sched_getcpu1 = %d\n", sched_getcpu());
					// now child1 uses cpu3 only.


					close(fd1[1]);
					close(fd2[0]);

					// read1
					//dup2(fd1[0], STDIN_FILENO); // redirect fd1[0] to stdin
					//read(STDIN_FILENO, str, BUFSIZE);
					read(fd1[0], str, BUFSIZE);
					printf("read1: %s\n", str);
					close(fd1[0]);

					// write2
					//dup2(fd2[1], STDOUT_FILENO); // redirect stdout to fd1[1]
					//printf("write2: %s\n", buf2);
					write(fd2[1], buf2, sizeof(buf2));
					close(fd2[1]);

					break;
				default:
					//parent
					wait(NULL);
					break;
			}			
	}

	printf("---------- measure context switching done -----------\n");
	return 0;
}  

void print_affinity(char* name)
{
	cpu_set_t mask;
	long nproc;
	long i;

	if (sched_getaffinity(0, sizeof(cpu_set_t), &mask) == -1)
	{
		perror("sched_getaffinity");
		assert(false);
	}
	else
	{
		nproc = sysconf(_SC_NPROCESSORS_ONLN);
		printf("sched_getaffinity name : %s = ", name);
		for (i=0; i<nproc; i++)
		{
			printf("%d ", CPU_ISSET(i, &mask));
		}
		printf("\n");
	}

}
