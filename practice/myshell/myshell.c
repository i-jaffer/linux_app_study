#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MAXARG	8	

typedef struct command_struct{
	char *arg[MAXARG];
	char *in;
	char *out;
}com_st;

int main()
{
	char buf[1024] = {0};
	com_st cmd;
	printf("cmd size:%ld\n",sizeof(cmd));

	while(1) {
		printf("myshell:");
		fgets(buf, 1024, stdin);
		printf("%s",buf);

		if(buf[strlen(buf)-1] == '\n')
			buf[strlen(buf)-1]='\0';
		if((strcasecmp("exit",buf)==0) || (strcasecmp("quit",buf)==0) ||
			(strcasecmp("bye",buf)==0))
			break;
	}

	return 0;
}
