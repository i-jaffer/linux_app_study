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
	int fd = 0,ret = 0;
	char buf[1024] = {0};
	int len = 0;
	
	fd = open("testfifo",O_RDONLY);
	if(fd == -1)
		sys_error("open error");

	while(1) {
		len = read(fd, buf, sizeof(len));
		if(len == -1)
			sys_error("read error");

		/* 将读取内容输出到屏幕 */
		ret = write(STDOUT_FILENO, buf, len);
		if(ret == -1)
			sys_error("write error");
		
	}

	return 0;	
}

