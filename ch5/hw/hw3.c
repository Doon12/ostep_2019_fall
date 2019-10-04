#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <pthread.h>
/* Q. fork()를 사용하는 다른 프로그램을 작성하라. 자식 프로세스는 "hello"를 출력하고
   부모 프로세스는 "goodbye"를 출력해야 한다. 항상 자식 프로세스가 먼저 출력하게 하라.
   부모가 wait()를 호출하지 않고 할 수 있는가?
 */

/* A. child process는 parent process가 stop할 때까지 기다리고, parent process는 stop한다.
   이후 child process는 실행되어 hello를 출력하고, process를 kill해서 parent process를 resume한다.
   그러면 wait을 사용하지 않고 child process 이후 parent process를 사용 할 수 있다.

   result.

   wait 안하고 하는 법
   1. semaphore 주의 : ipc semaphore 쓰자. 그냥 semaphore는 thread semaphore.
   2. cond variable
   3. sleep


 @DESKTOP-A0DKAKE:~/os_2019_fall/ch5/hw$ ./hw3
 
 [2]+  Stopped                 ./hw3
 hello
 goodbye
 
 */

sem_t mutex;

void* child(void *arg)
{
	printf("child\n");
	sem_post(&mutex);
	return NULL;
}

int main(void)
{
	if ( sem_init(&mutex, 0, 0) != 0)
		perror("sema init error");
	/*
	pthread_t c;
	pthread_create (&c, NULL, child, NULL);
	sem_wait (&mutex);
	printf("parent : end\n");
	*/
	pid_t pid = fork();

	if (pid < 0)
	{
		perror("fork error");
	}
	else if (pid == 0)
	{
		printf("child process\n");
		sem_post(&mutex);
	}
	else
	{
		sem_wait(&mutex);
		printf("parent process\n");
	}
	
	sem_destroy(&mutex);
	return 0;
}
