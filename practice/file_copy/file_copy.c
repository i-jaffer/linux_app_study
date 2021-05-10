#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>

int process_num = 14;	/* process number */

void sys_error(char *str)
{
	perror(str);
	exit(-1);
}

int main(int argc,char *argv[])
{
	int fd_src = 0, fd_dest = 0;
	int ret = 0;
	struct stat statbuf;
	int file_len = 0;
	pid_t pid;

	/* check input */
	if(argc < 3) {
		printf("./copyfile sourcefile destinationfile\n");
		exit(-1);
	}

	/* check source file size */
	ret = stat(argv[1],&statbuf);
	if(ret == -1) {
		sys_error("stat error");
	}
	printf("sourcefile size: %ld\n",statbuf.st_size);

	/* open source file and destination file or creat destination file */
	fd_src = open(argv[1],O_RDONLY);
	if(fd_src == -1) {
		sys_error("open source error");
	}
	fd_dest = open(argv[2],O_RDWR|O_CREAT|O_TRUNC,0664);
	if(fd_dest == -1) {
		sys_error("open destination error");
	}
	if(statbuf.st_size != 0) {
		ret = ftruncate(fd_dest,statbuf.st_size);
		if(ret == -1)
			sys_error("ftruncate error");
	}
	else {
		goto out;
	}

	/* creat mapping in the virtual address */
	char *dest_p = NULL;
	char *dest_head = NULL;
	char *src_p = NULL;
	char *src_head = NULL;
	src_head = mmap(NULL,statbuf.st_size,PROT_READ,MAP_PRIVATE,fd_src,0);
	src_p = src_head;
	if(src_head == MAP_FAILED) {
		sys_error("mmap sourcefile error");
	}
	dest_head = mmap(NULL,statbuf.st_size,PROT_WRITE|PROT_READ,MAP_SHARED,fd_dest,0);
	dest_p = dest_head;
	if(dest_head == MAP_FAILED) {
		sys_error("mmap destination error");
	}

	/* creat child process */
	int i = 0;
	int len = statbuf.st_size;
	if(process_num > len)
		process_num = len/2;
	for(i=0; i<process_num; i++) {
		pid = fork();
		if(pid == -1) {
			sys_error("fork error");
		} else if(pid == 0) {
			break;
		}
	}

	/* child process copy */
	if(i < process_num) {
		if(i != (process_num-1))
			memcpy((dest_p+((len/process_num)*i)), (src_p+((len/process_num)*i)), (len/process_num));
		else
			memcpy((dest_p+((len/process_num)*i)), (src_p+((len/process_num)*i)), ((len/process_num)+(len%process_num)));
	}

	/* release mapping */
	munmap(src_head,statbuf.st_size);
	munmap(dest_head,statbuf.st_size);

	/* recycling child process */
	for(i=0; i<process_num; i++) {
		wait(NULL);
	}
out:
	return 0;
}






