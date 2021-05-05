#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
	pid_t pid;
	int i = 0;

	printf("xxxxxxxxxxxxxxx\n");

	for(i=0; i<5; i++) {
		pid = fork();
		if(pid == -1) {
			perror("fork error");
			exit(-1);
		} else if(pid == 0) {
			break;
		}
	}
	if(i < 5) {
		sleep(i);
		printf("This is child %dth, the process pid:%d, the parent process pid:%d\n",i,getpid(),getppid());
	} else {
		sleep(i);
		printf("This is parent, the process pid:%d, the parent process pid:%d\n",getpid(),getppid());
	}

	printf("yyyyyyyyyyyyyyyyy\n");

	return 0;
}
