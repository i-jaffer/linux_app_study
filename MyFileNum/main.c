#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

int get_file_count(char *root)
{
	int total = 0;
	DIR *dir = opendir(root);
	if(dir == NULL)
	{
		perror("opendir");
		exit(1);
	}
	struct dirent *dir_str = NULL;
	while((dir_str = readdir(dir)) != NULL)
	{
		//跳过 . 和 .. 干扰
		if((strcmp(dir_str->d_name,".") == 0) ||
		   (strcmp(dir_str->d_name,"..") == 0))
			continue;
		//如果是文件夹 递归
		if(dir_str->d_type == DT_DIR)
		{
			char buf[1024]={0};
			sprintf(buf,"%s/%s",root,dir_str->d_name);
			total += get_file_count(buf);
		}
		//如果是普通文件
		if(dir_str->d_type == DT_REG)
		{
			total ++;
		}
	}
	closedir(dir);
	return total;
}

int main(int argc,char *argv[])
{
	int total = 0;
	if(argc < 2)
	{
		printf("./a.out directory\n");
		exit(1);
	}
	total = get_file_count(argv[1]);
	printf("%s file num = %d\n",argv[1],total);
	return 0;
}


