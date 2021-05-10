#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

void sys_error(char *str)
{
	perror(str);
	exit(-1);
}

int main()
{
	int ret = 0,fd = 0;
	char buf[1024] = {0};
	int count = 0;

#if 0
	ret = mkfifo("testfifo",0664);
	if(ret == -1)
		sys_error("mkfifo error");
#endif
	
	fd = open("testfifo",O_WRONLY);
	if(fd == -1)
		sys_error("open error");

	while(1) {
		sprintf(buf, "count: %d\n",count++);
		ret = write(fd, buf, sizeof(buf));
		if(ret < 0)
			sys_error("write error");
		sleep(1);
	}
	close(fd);
	
	return 0;
}
