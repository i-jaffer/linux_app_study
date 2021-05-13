#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

void sys_error(char *str)
{
	perror(str);
	exit(-1);
}

void sig_catch_cb(int signo)
{
	printf("------- SIG catch %d ---------\n",signo);
}

void alarm_set(int sec)
{
	int ret = 0;
	struct itimerval new_value, old_value;
	new_value.it_interval.tv_sec = sec;
	new_value.it_interval.tv_usec = 0;
	new_value.it_value.tv_sec = sec;
	new_value.it_value.tv_usec = 0;
	ret = setitimer(ITIMER_REAL, &new_value, &old_value);
	if(ret == -1) {
		sys_error("setitimer error");
	}
}

//#define USE_SETITIMER
unsigned int mysleep(unsigned int sec)
{
	int ret = 0;

	struct sigaction act, old_act;
	act.sa_handler = sig_catch_cb;		//register call back
	sigemptyset(&(act.sa_mask));		//clear mask
	act.sa_flags = 0;			//fault,shield the corresponding signal in cb
	sigaction(SIGALRM, &act, &old_act);

#ifdef USE_SETITIMER
	alarm_set(sec);
#else
	alarm(sec);
#endif
	pause();				//suspend process	

#ifdef USE_SETITIMER
	struct itimerval cur_value;
	alarm_set(0);				//close alarm and get time remain
	ret = getitimer(ITIMER_REAL,&cur_value);
	if(ret == -1)
		sys_error("timer_gettimer");
	ret = (int)cur_value.it_value.tv_sec;
#else
	ret = alarm(0);				//close alarm and get time remain
#endif
	sigaction(SIGALRM, &old_act, NULL);	//restore SIGALRM deal

	return ret;
}

int main()
{
	while(1) {
		mysleep(2);
		printf("finish!\n");
	}

	return 0;
}
