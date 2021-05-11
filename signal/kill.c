#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

int main()
{
	int ret = 0;
	ret = kill(getpid(),SIGKILL);
	if(ret == -1) {
		perror("kill error");
		exit(-1);
	}
	return 0;
}

