#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	pid_t pid;
	pid = fork();
	if(pid == -1) {
		perror("fork error:");
		exit(-1);
	} else if(pid == 0) {
		printf("this is child process,the process id:%d,the parent process id:%d\n",
		getpid(),getppid());
	} else {
		int ret = 0;
		int wstatus = 0;
		do {
			ret = waitpid(pid,&wstatus,WNOHANG);
			if(ret == -1) {
				perror("wait error:");
				exit(-1);
			}
		} while(ret == 0);
		
		sleep(6);
		printf("this is parent process,the process id:%d,the parent process id:%d\n",
		getpid(),getppid());
	}
	return 0;
}

