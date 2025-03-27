#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

#define MSG "I broke out of Pipex to build a better system\n"
#define BUFFER_SIZE 1024

int	main(void)
{
	int	pipe_fd[2];
	pid_t	pid;
	char	buffer[BUFFER_SIZE];

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		close(pipe_fd[1]);
		ssize_t	bytes_read = read(pipe_fd[0], buffer, BUFFER_SIZE - 1);
		if (bytes_read == -1)
		{
			perror("read");
			exit(EXIT_FAILURE);
		}
		buffer[bytes_read] = '\0';
		printf("Child received message: %s", buffer);
		printf("pid: %d, getpid: %d\n", pid, getpid());
		printf("Lets experiment to see if this works\n");
	}
	else
	{
		printf("pid: %d, getpid: %d\n", pid, getpid());
		close(pipe_fd[0]);
		if (write(pipe_fd[1], MSG, strlen(MSG)) == -1)
		{
			close(pipe_fd[1]);
			perror("write");
			exit(EXIT_FAILURE);
		}
		waitpid(pid, NULL, 0);
	}
	return (0);
}