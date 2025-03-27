#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>

int	main(void)
{
	int	pipe_fd[2];
	pid_t	pid1, pid2;

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	pid1 = fork();
	if (pid1 == 0)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		//close(pipe_fd[0]);
		execlp("echo", "echo", "leaking...", NULL);
		perror("execlp");
		exit(1);
	}
	pid2 = fork();
	if (pid2 == 0)
	{
		//Child 2: Read from pipe
		dup2(pipe_fd[0], STDIN_FILENO);
		//close(pipe_fd[0]);
		//close (pipe_fd[1]);
		execlp("cat", "cat", NULL);
		perror("execlp");
		exit(1);
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
}