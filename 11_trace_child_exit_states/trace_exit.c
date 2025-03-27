#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void	trace_exit_status(pid_t pid, int status)
{
	if (WIFEXITED(status))
		printf("Child %d exited normally with status %d\n", pid, WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		printf("Child %d was killed by signal %d\n", pid, WTERMSIG(status));
		if (WCOREDUMP(status))
			printf(" (core dumped)");
		printf("\n");
	}
	else if (WIFSTOPPED(status))
		printf("Child %d was stopped by signal %d\n", pid, WSTOPSIG(status));
	else
		printf("? Child %d has unknown exit state %d\n", pid, status);
}