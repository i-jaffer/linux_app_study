#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void sig_catch_cb(int signo)
{
	/* recycle child process */
	int wstatus = 0;
	pid_t pid;
	while((pid = waitpid(0, &wstatus, WNOHANG)) > 0) {
		if(WIFEXITED(wstatus))
			printf("pid:%d exit status:%d\n",pid,WEXITSTATUS(wstatus));
		if(WIFSIGNALED(wstatus))
			printf("pid:%d signal number:%d\n",pid,WTERMSIG(wstatus));
	}
}

int main()
{
	pid_t pid;
	int i = 0;

	/* shield SIGCHLD signal */
	sigset_t new_set,old_set;
	sigemptyset(&new_set);
	sigaddset(&new_set,SIGCHLD);
	sigprocmask(SIG_BLOCK,&new_set,&old_set);

	for(i=0; i<10; i++) {
		pid = fork();
		if(pid == -1) {
			perror("fork error");
			exit(-1);
		} else if(pid == 0) {
			break;
		}
	}
	if(i < 10) {
		sleep(1);
		printf("this is %dth child,pid:%d\n",i,getpid());
		return i;
	} else {
		/* register SIGCHLD signal call back */
		struct sigaction new_act,old_act;
		new_act.sa_handler = sig_catch_cb;
		sigemptyset(&(new_act.sa_mask));
		new_act.sa_flags = 0;
		sigaction(SIGCHLD, &new_act, &old_act);

		/* cancel shield SIGCHLD signal */
		sigprocmask(SIG_SETMASK,&old_set,NULL);

		while(1);
	}

	return 0;
}
