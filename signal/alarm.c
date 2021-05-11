#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	int i = 0;
	alarm(1);

	for(i=0; ;i++) {
		printf("%d\n",i);
	}

	return 0;
}
