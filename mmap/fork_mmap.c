#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

int main()
{
	pid_t pid = 0;
	int fd = 0,ret = 0;
	int *p = NULL;
	int var = 100;

	fd = open("temp",O_RDWR|O_CREAT,0664);		//创建临时文件
	if(fd == 0) {
		perror("open error");
		exit(-1);
	}
	ret = unlink("temp");				//删除文件目录项，使之具备释放条件
	if(ret == -1) {
		perror("unlink error");
		exit(-1);
	}
	ret = ftruncate(fd,4);
	if(ret == -1) {
		perror("ftruncate error");
		exit(-1);
	}

	p = mmap(NULL,4,PROT_WRITE|PROT_READ,MAP_SHARED,fd,0);	//创建映射区
	if(p == MAP_FAILED) {
		perror("mmap error");
		exit(-1);
	}
	close(fd);

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
