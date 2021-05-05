#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc,char *argv[])
{
	if(argc < 2)
	{
		printf("./a.out filename\n");
		goto out;
	}
	struct stat st;
	int ret = stat(argv[1], &st);
	if(ret == -1)
	{
		perror("stat");
		goto out;
	}
	printf("filename size: %d\n",(int)st.st_size);

out:
	return 0;
}


