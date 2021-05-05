#include <stdio.h>
#include <stdlib.h>

int main()
{
	char *env = NULL;
	env = getenv("ABD");
	printf("1. first %s = %s\n","1.ABD",env);

	int ret = 0;
	ret = setenv("ABD","abc",0);
	printf("	setenv ret = %d\n",ret);
	env = getenv("ABD");
	printf("2. overwerite=0 %s = %s\n","2.ABD",env);

	ret = setenv("ABD","456",0);
	printf("	setenv ret = %d\n",ret);
	env = getenv("ABD");
	printf("3. overwerite=0 %s = %s\n","3.ABD",env);

	ret = setenv("ABD","456",1);
	printf("	setenv ret = %d\n",ret);
	env = getenv("ABD");
	printf("4. overwerite=1 %s = %s\n","3.ABD",env);
	
	ret = unsetenv("ABD");
	printf("unsetenv \"ABD\" ret = %d\n",ret);

	ret = unsetenv("adj");
	printf("unsetenv \"adj\" ret = %d\n",ret);

	ret = unsetenv("ABD=");
	printf("unsetenv \"ABD=\" ret = %d\n",ret);

	return 0;
}

