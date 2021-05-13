#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

int n = 0;
pid_t child_pid = 0;

void sys_error(char *str)
{
	perror(str);
	exit(-1);
}

void do_sig_child(int signo)
{
	n+=2;
	printf("n = %d, child process id:%d\n",n,getpid());
	kill(getppid(),SIGUSR2);
}

void do_sig_parent(int signo)
{
	n+=2;
	printf("n = %d, parent process id:%d\n",n,getpid());
	kill(child_pid,SIGUSR1);
}

int main()
{
	pid_t pid;
	struct sigaction new_set,old_set;

	pid = fork();
	if(pid == -1) {
		sys_error("fork error");
	} else if(pid == 0) {	//child process
		n = 2;
		new_set.sa_handler = do_sig_child;
		sigemptyset(&(new_set.sa_mask));
		new_set.sa_flags = 0;
		sigaction(SIGUSR1,&new_set,&old_set);

		sleep(1);

		while(1);
	} else {		//parent process
		child_pid = pid;
		n = 1;
		new_set.sa_handler = do_sig_parent;
		sigemptyset(&(new_set.sa_mask));
		new_set.sa_flags = 0;
		sigaction(SIGUSR2,&new_set,&old_set);

		sleep(1);

		do_sig_parent(0);
		while(1);
	}

	return 0;
}
