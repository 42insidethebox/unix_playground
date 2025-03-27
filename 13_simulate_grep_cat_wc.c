#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

void	log_open_fd(const char *label)
{
	int	fd = 0;

	printf("[%s] Open FDs ", label);
	while (fd < 1024)
	{
		if (fcntl(fd, F_GETFD) != -1)
			printf("%d ", fd);
		fd++;
	}
	printf("\n");
}

int	main(void)
{
	int		pipe1[2];
	int		pipe2[2];
	pid_t	pid1, pid2, pid3;

	if (pipe(pipe1) == -1 || pipe(pipe2) == -1)
		return (perror("pipe"), EXIT_FAILURE);
	pid1 = fork();
	if (pid1 == 0)
	{
		int	file_fd = open ("file.txt", O_RDONLY);
		if (file_fd == -1)
			return (perror("open file.txt"), EXIT_FAILURE);
		
		dup2(file_fd, STDIN_FILENO);
		close(file_fd);
		
		close(pipe1[0]);
		dup2(pipe1[1], STDOUT_FILENO);
		close(pipe1[1]);
		
		close(pipe2[0]);
		close(pipe2[1]);

		log_open_fd("child1 (grep)");
		execlp("grep", "grep", "pattern", NULL);
		perror("exec grep");
		exit(EXIT_FAILURE);
	}
	pid2 = fork();
	if (pid2 == 0)
	{
		dup2(pipe1[0], STDIN_FILENO);
		close(pipe1[0]);
		close(pipe1[1]);

		dup2(pipe2[1], STDOUT_FILENO);
		close(pipe2[1]);
		close(pipe2[0]);

		log_open_fd("child2 (cat)");
		execlp("cat", "cat", NULL);
		perror("exec cat");
		exit(EXIT_FAILURE);
	}
	pid3 = fork();
	if (pid3 == 0)
	{
		dup2(pipe2[0], STDIN_FILENO);
		close(pipe2[0]);
		close(pipe2[1]);

		close(pipe1[0]);
		close(pipe1[1]);

		log_open_fd("child3 (wc)");
		execlp("wc", "wc", "-l", NULL);
		perror("exec wc");
		exit(EXIT_FAILURE);
	}
	close(pipe1[0]);
	close(pipe1[1]);
	close(pipe2[0]);
	close(pipe2[1]);

	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	waitpid(pid3, NULL, 0);

	return (0);
}