#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	int fd = 0;
	int ret = 0;
	char *p = NULL;
	fd = open("test.txt",O_RDWR|O_CREAT,0664);
	if(fd == -1) {
		perror("open error:");
		exit(-1);
	}
	ret = ftruncate(fd, 4);
	if(ret == -1) {
		perror("ftruncate error:");
		exit(-1);
	}
	p = mmap(NULL,4,PROT_WRITE|PROT_READ,MAP_SHARED,fd,0);
	if(p == MAP_FAILED) {
		perror("mmap error:");
		exit(-1);
	}
	strcpy(p, "abc");
	munmap(p, 4);
	close(fd);
	return 0;
}
