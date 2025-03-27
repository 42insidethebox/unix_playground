#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>

void	trace_exit_status(pid_t pid, int status);

void	normal_exit(void)
{
	exit(42);
}

void	signal_kill(void)
{
	raise(SIGSEGV);
}

int	main(void)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
	{
		//normal_exit();
		 signal_kill();
	}
	else
	{
		waitpid(pid, &status, 0);
		trace_exit_status(pid, status);
	}
	return (0);
}