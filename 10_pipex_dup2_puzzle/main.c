#include "utils.h"
#include <sys/wait.h>

int	main(void)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	pid1 = fork();
	if (pid1 == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid1 == 0)
	{
		// First child: executes `cat`
		log_fd_table("child1 before dup2");

		close(pipefd[0]); // close unused read end
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2 child1");
			exit(EXIT_FAILURE);
		}
		log_fd_table("child1 after dup2");

		close(pipefd[1]);
		execlp("cat", "cat", NULL);
		perror("exec cat");
		exit(EXIT_FAILURE);
	}

	pid2 = fork();
	if (pid2 == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid2 == 0)
	{
		// Second child: executes `wc`
		log_fd_table("child2 before dup2");

		close(pipefd[1]); // close unused write end
		if (dup2(pipefd[0], STDIN_FILENO) == -1)
		{
			perror("dup2 child2");
			exit(EXIT_FAILURE);
		}
		log_fd_table("child2 after dup2");

		close(pipefd[0]);
		execlp("wc", "wc", NULL);
		perror("exec wc");
		exit(EXIT_FAILURE);
	}

	// Parent process
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);

	return (0);
}
