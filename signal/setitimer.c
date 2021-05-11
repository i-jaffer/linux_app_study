#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

unsigned int my_alarm(unsigned int sec)
{
	struct itimerval curr_value;
	struct itimerval old_value;
	int ret = 0;

	curr_value.it_interval.tv_sec = 0;
	curr_value.it_interval.tv_usec = 0;
	curr_value.it_value.tv_sec = sec;
	curr_value.it_value.tv_usec = 0;

	ret = setitimer(ITIMER_REAL, &curr_value, &old_value);

	return old_value.it_interval.tv_sec;
}

int main()
{
	my_alarm(1);
	for(int i=0; ;i++) {
		printf("%d\n",i);
	}
	return 0;
}
