#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

void	log_open_fds(const char *label)
{
	int	fd;

	printf("[%s] Open FDS: ", label);
	fd = 0;
	while (fd < 1024)
	{
		if (fcntl(fd, F_GETFD) != -1)
			printf("%d ", fd);
		fd++;
	}
		printf("\n");
}

void	close_all_except(int except_fd1, int except_fd2)
{
	int	fd = 0;

	while (fd < 1024)
	{
		if (fd != except_fd1 && fd != except_fd2)
			close(fd);
		fd++;
	}
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
		// Forget to close pipe1[0] intentionally
		dup2(pipe1[1], STDOUT_FILENO);
		close(pipe1[1]);
		close(pipe2[0]);
		close(pipe2[1]);
		log_open_fds("child1");
		execlp("echo", "echo", "hello from cmd1", NULL);
		perror("exec echo");
		exit(EXIT_FAILURE);
	}
	pid2 = fork();
	if (pid2 == 0)
	{
		dup2(pipe1[0], STDIN_FILENO);
		dup2(pipe2[1], STDOUT_FILENO);
		close(pipe1[1]);
		close(pipe2[0]);
		// Forgot to close pipe1[0] in child2
		// pipe1[0] kept open in both child1 and child 2 hangs at EOF
		log_open_fds("child2");
		execlp("cat", "cat", NULL);
		perror("exec cat");
		exit(EXIT_FAILURE);
	}
	pid3 = fork();
	if (pid3 == 0)
	{
		dup2(pipe2[0], STDIN_FILENO);
		close(pipe1[0]);
		close(pipe1[1]);
		close(pipe2[1]);
		log_open_fds("child3");
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
}