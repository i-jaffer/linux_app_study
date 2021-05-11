#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>

void sign_alarm_callback(int signao)
{
	printf("sign alarm \n");
}

int main()
{
	struct itimerval new_value;
	struct itimerval old_value;
	int ret = 0;

	signal(SIGALRM,sign_alarm_callback);	//注册SIGALRM回调函数

	new_value.it_interval.tv_sec = 3;
	new_value.it_interval.tv_usec = 0;
	new_value.it_value.tv_sec = 5;
	new_value.it_value.tv_usec = 0;
	ret = setitimer(ITIMER_REAL,&new_value,&old_value);
	if(ret == -1) {
		perror("setitimer error");
		exit(-1);
	}
	while(1);

	return 0;
}
