#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	pid_t pid;

	pid = fork();
	if(pid == -1) {
		perror("fork error");
		exit(-1);
	} else if(pid == 0) {
		sleep(1);
		printf("this is parent!\n");
	} else {
#if 0
		execlp("ls","ls","-a","-l",NULL);
#elif 0
		execl("/bin/ls","ls","-a","-l",NULL);
#elif 0
		char *argv[]={"ls","-a","-l",NULL};
		execv("/bin/ls",argv);
#elif 1
		char *argv[]={"ls","-a","-l",NULL};
		execvp("ls",argv);
#endif
	}
	return 0;
}
