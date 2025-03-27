#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

int	main(void)
{
	int		pipe1[2];
	int		pipe2[2];
	pid_t	pid;
	char	buffer[BUFFER_SIZE];

	if (pipe(pipe1) == -1 || pipe(pipe2) == -2)
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
		close(pipe1[1]);
		close(pipe2[0]);
		read(pipe1[0], buffer, BUFFER_SIZE);
		printf("Child received %s", buffer);

		char	*response = "Roger that, parent \n";
		write(pipe2[1], response, strlen(response));
		close(pipe1[0]);
		close(pipe2[1]);
	}
	else
	{
		close(pipe1[0]);
		close(pipe2[1]);

		char	*msg = "Can you hear me child ? \n";
		write(pipe1[1], msg, strlen(msg));
		read(pipe2[0], buffer, BUFFER_SIZE);
		printf("Parent received: %s", buffer);
		close(pipe1[1]);
		close(pipe2[0]);
		waitpid(pid, NULL, 0);
	}
	return (0);
}