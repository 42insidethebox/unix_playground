#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

int	main(void)
{
	pid_t	pid;
	int		fd;

	fd = open("output2.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		close(fd);
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			close(fd);
			exit(EXIT_FAILURE);
		}
		close(fd);

		char	*args[] = {"/bin/ls", "-l", NULL};
		execve("/bin/ls", args, NULL);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else
	{
		close(fd);
		waitpid(pid, NULL, 0);
		printf("Done running /bin/ls, check output2.txt\n");
	}
	return (0);
}
