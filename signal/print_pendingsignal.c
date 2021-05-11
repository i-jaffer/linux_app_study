#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

void signal_alarm_callback(int signo)
{
	printf("signal alarm!\n");
}

/*
 * @brief print pending signal collection
 */
void print_pending(sigset_t *sigset)
{
	int ret = 0;
	for(int i=1;i<=64;i++) {
		ret = sigismember(sigset,i);	//pass: i>0 "i" representative SIG
		if(ret == -1) {
			perror("sigismember error");
			exit(-1);
		} else if(ret == 1) {
			printf("1");
		} else if(ret == 0) {
			printf("0");
		}

		if(i%8 == 0)
			printf(" ");
	}
	printf("\n");
}

int main()
{
	sigset_t set, oldset, ped;
	int ret = 0;

	/*	ctrl+c -> SIGINT
		ctrl+z -> SIGTSTP
		ctrl+\ -> SIGQUIT
		SIGKILL and SIGSTOP setting is invalid,because the system prohibits
	*/
	sigemptyset(&set);		//clean signal collection
	sigaddset(&set,SIGALRM);	//set signal collection bit
	sigaddset(&set,SIGINT);		//set signal collection bit 
	sigaddset(&set,SIGTSTP);	//set signal collection bit 
	sigaddset(&set,SIGQUIT);	//set signal collection bit
	sigaddset(&set,SIGKILL);	//set signal collection bit

	ret = sigprocmask(SIG_BLOCK, &set, &oldset);
	if(ret == -1) {
		perror("sigprocmask error");
		exit(-1);
	}

	signal(SIGALRM,signal_alarm_callback);
	struct itimerval new_value, old_value;
	new_value.it_interval.tv_sec = 3;
	new_value.it_interval.tv_usec = 0;
	new_value.it_value.tv_sec = 1;
	new_value.it_value.tv_usec = 0;
	setitimer(ITIMER_REAL,&new_value,&old_value);

	while(1) {
		ret = sigpending(&ped);
		if(ret == -1) {
			perror("sigpending error");
			exit(-1);
		}
		print_pending(&ped);
		sleep(1);
	}

	return 0;
}
