#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

#if 1		//仅适用于linux操作环境
int main()
{
	pid_t pid;
	int ret = 0;
	int *p = NULL;
	int var = 100;

	p = mmap(NULL,4,PROT_WRITE|PROT_READ,MAP_SHARED|MAP_ANONYMOUS,-1,0);	//创建匿名映射区
	if(p == MAP_FAILED) {
		perror("mmap error");
		exit(-1);
	}

	pid = fork();					//创建子进程
	if(pid == -1) {
		perror("fork error");
		exit(-1);
	} else if(pid == 0) {
		*p = 10000;
		var = 10;
		printf("child:*p=%d var=%d\n",*p,var);
	} else {
		sleep(1);
		printf("child:*p=%d var=%d\n",*p,var);
		wait(NULL);	//回收子进程
		ret = munmap(p,4);
		if(ret == -1) {
			perror("munmap error");
			exit(-1);
		}
	}

	return 0;
}

#else  		//适用于所有类unix操作系统

int main()
{
	pid_t pid;
	int fd = 0;
	int ret = 0;
	int *p = NULL;
	int var = 100;

	fd = open("/dev/zero",O_RDWR);
	if(fd == -1) {
		perror("open error");
		exit(-1);
	}

	p = mmap(NULL,4,PROT_WRITE|PROT_READ,MAP_SHARED,fd,0);	//创建匿名映射区
	if(p == MAP_FAILED) {
		perror("mmap error");
		exit(-1);
	}

	pid = fork();					//创建子进程
	if(pid == -1) {
		perror("fork error");
		exit(-1);
	} else if(pid == 0) {
		*p = 10000;
		var = 10;
		printf("child:*p=%d var=%d\n",*p,var);
	} else {
		sleep(1);
		printf("child:*p=%d var=%d\n",*p,var);
		wait(NULL);	//回收子进程
		ret = munmap(p,4);
		if(ret == -1) {
			perror("munmap error");
			exit(-1);
		}
	}

	return 0;
}

#endif
