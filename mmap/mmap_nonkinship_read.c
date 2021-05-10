#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>

typedef struct test_struct {
 	int id;
	char name[20];
 	char sex;
}test_struct_typedef;

void perror_output(char *argc)
{
	perror(argc);
	exit(-1);
}

int main(int argc,char *argv[])
{
	test_struct_typedef *test_struct_p;
	int fd = 0;
	if(argc < 2) {
		printf("./a.out filename\n");
		exit(-1);
	}
	fd = open(argv[1],O_RDONLY);
	if(fd == -1) 
		perror_output("open error");
	test_struct_p = mmap(NULL,sizeof(test_struct_typedef),PROT_READ,MAP_SHARED,fd,0);
	if(test_struct_p == MAP_FAILED)
		perror_output("mmap error");
	
	while(1) {
		sleep(2);
		printf("id = %d, name = %s, sex = %d\n",test_struct_p->id,test_struct_p->name,test_struct_p->sex);
	}

	return 0;
}
