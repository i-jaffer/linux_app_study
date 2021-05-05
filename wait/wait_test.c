#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	pid_t pid;
	pid_t wait_pid;
	printf("xxxxxxxxxxxxx\n");

	pid = fork();
	if(pid == -1) {
		perror("fork error:");
		exit(-1);
	} else if(pid == 0) {	//子进程
		printf("this is child!\n");
		printf("this is child, process pid:%d, the parent process pid:%d\n",getpid(),
		getppid());
//		sleep(40);
		execl("./child_process_test","child_process_test",NULL);
		printf("child process finish!\n");
	} else {		//父进程
		int wstatus = 0;
		wait_pid = wait(&wstatus);
		if(wait_pid == -1) {
			perror("wait error:");
			exit(-1);
		}
		if(WIFEXITED(wstatus)) {	//子进程正常退出
			printf("child process normal exit,wstatus:%d\n",WEXITSTATUS(wstatus));
		}
		if(WIFSIGNALED(wstatus)) {
			printf("child process abnormal exit,wstatus:%d\n",WTERMSIG(wstatus));
		}
		sleep(0.5);
		printf("this is parent, process pid:%d, the parent process pid:%d\n",getpid(),
		getppid());
	}
	return 0;
}
