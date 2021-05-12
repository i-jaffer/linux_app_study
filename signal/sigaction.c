#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void sys_error(char *str)
{
	perror(str);
	exit(-1);
}

void sig_catch_cb(int signo)
{
	if(signo == SIGINT) {
		printf("------------- catch SIGINT ----------\n");
		sleep(2);
		printf("------------- catch SIGINT finish --------------\n");
	} else if(signo == SIGTSTP) {
		printf("------------- catch SIGTSTP ----------\n");
	}
}

int main()
{
	struct sigaction act, old_act;
	int ret = 0;

	act.sa_handler = sig_catch_cb;		//register call back
	sigemptyset(&(act.sa_mask));		//clear sa_mask
	sigaddset(&(act.sa_mask),SIGINT);	//add SIGINT to sa_mask
	act.sa_flags = 0;			//default value auto shield the signal in callback
	ret = sigaction(SIGINT,&act,&old_act);
	if(ret == -1)
		sys_error("sigaction error");
	ret = sigaction(SIGTSTP,&act,&old_act);
	if(ret == -1)
		sys_error("sigaction error");

	while(1);

	return 0;
}
