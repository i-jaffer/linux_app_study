#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

int main()
{
	int ret = 0;

	//abort();

	ret = raise(SIGKILL);
	if(ret != 0) {
		perror("raise error");
		exit(-1);
	}
	return 0;
}
