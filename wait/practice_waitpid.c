#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int main()
{
	pid_t pid,pid_buf[3] = {0};
	int i = 0;
	int fd = open("practice_test.txt",O_WRONLY|O_CREAT,0664);
	if(fd == -1) {
		perror("open error:");
		exit(-1);
	}
	for(i=0;i<3;i++) {
		pid = fork();
		if(pid == -1) {
			perror("fork error:");
			exit(-1);
		} else if(pid == 0) {
			break;
		} else {
			pid_buf[i] = pid;
		}
	}
	if(i < 3) {
		sleep(i);
		printf("this is %dth child,the process id:%d,the parent process id:%d\n",
		i,getpid(),getppid());
		if(i == 0) {
			dup2(fd,STDOUT_FILENO);	//重定向
			execlp("ps","ps","aux",NULL);
		} else if(i == 1) {
			execl("./waitpid_child_normal","./waitpid_child_normal",NULL);
		} else if(i == 2) {
			execl("./waitpid_child_abnormal","./waitpid_child_abnormal",NULL);
		}
	} else if(i == 3) {
		int wstatus = 0,wret = 0;
		int wait_count = 0;
		do {
			wret = waitpid(-1,&wstatus,WNOHANG);
			if(wret == -1) {
				perror("waitpid error:");
				exit(-1);
			} else if(wret > 0) {
				wait_count++;
				printf("recycling process %d\n",wret);
				if(WIFEXITED(wstatus))
					printf("exit status:%d\n",WEXITSTATUS(wstatus));
				if(WIFSIGNALED(wstatus))
					printf("signal number:%d\n",WTERMSIG(wstatus));
			}
		}while(wait_count < 3);
		printf("this is parent,the process id:%d,the parent process id:%d\n",
		getpid(),getppid());
	}

	return 0;
}
