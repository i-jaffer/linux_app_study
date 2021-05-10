#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

typedef struct test_struct {
	int id;
	char name[20];
	char sex;
}test_struct_typedef;

int main(int argc,char *argv[])
{
	test_struct_typedef human_struct;
	test_struct_typedef *test_struct_p;
	int fd = 0;
	int ret = 0;

	if(argc < 2)
	{
		printf("./a.out filename\n");
		exit(-1);
	}
	fd = open(argv[1],O_RDWR);
	if(fd == -1) {
		perror("open error");
		exit(-1);
	}
	ret = ftruncate(fd,sizeof(test_struct_typedef));
	if(ret == -1) {
		perror("ftruncate error");
		exit(-1);
	}
	test_struct_p = mmap(NULL,sizeof(test_struct_typedef),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if(test_struct_p == MAP_FAILED) {
		perror("mmap error");
		exit(-1);
	}
	close(fd);

	human_struct.id = 0;
	strcpy(human_struct.name,"tim");
	human_struct.sex = 2;

	while(1) {
		/* write data to sharefile (nonkinship communicate) */
		memcpy(test_struct_p,&human_struct,sizeof(test_struct_typedef));
		human_struct.id ++;
		printf("id = %d\n",human_struct.id);
		sleep(1);
	}

	munmap(test_struct_p,sizeof(test_struct_typedef));

	return 0;
}
