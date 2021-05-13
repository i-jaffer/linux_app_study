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

unsigned int mysleep(unsigned int sec)
{
	int ret = 0;
	struct sigaction act, old_act;
	sigset_t new_set,old_set;
	sigset_t suspend_mask;

	act.sa_handler = sig_catch_cb;		//register call back
	sigemptyset(&(act.sa_mask));		//clear mask
	act.sa_flags = 0;			//fault,shield the corresponding signal in cb
	sigaction(SIGALRM, &act, &old_act);

	/* shield SIGALRM SIGNAL */
	sigemptyset(&new_set);
	sigaddset(&new_set,SIGALRM);
	sigprocmask(SIG_BLOCK,&new_set,&old_set);

	/* set alarm */
	alarm(sec);

	/* suspend process and use new mask */
	suspend_mask = old_set;
	sigdelset(&suspend_mask,SIGALRM);	//mask sure the signal is not masked
	sigsuspend(&suspend_mask);		//suspend the process and replace the signal mask

	ret = alarm(0);				//close alarm and get time remain

	sigaction(SIGALRM, &old_act, NULL);	//restore SIGALRM deal
	sigprocmask(SIG_SETMASK,&old_set,NULL);	//restore signal mask

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
