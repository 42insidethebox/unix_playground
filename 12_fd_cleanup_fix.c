#include <stdio.h>      // printf, perror
#include <stdlib.h>     // exit, EXIT_FAILURE
#include <unistd.h>     // pipe, fork, dup2, execlp, close
#include <sys/wait.h>   // waitpid
#include <fcntl.h>      // fcntl

// ─────────────────────────────────────────────────────────────
// 📋 Log all open file descriptors for debugging
void	log_open_fds(const char *label)
{
	int	fd = 0;

	printf("[%s] Open FDS: ", label);
	while (fd < 1024)
	{
		if (fcntl(fd, F_GETFD) != -1)
			printf("%d ", fd);
		fd++;
	}
	printf("\n");
}

// 🔒 Close all FDs except 1 or 2 special ones
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

// ─────────────────────────────────────────────────────────────
// 🚀 Entry Point: 3-process pipe chain: echo → cat → wc -l
int	main(void)
{
	int		pipe1[2];	// between echo and cat
	int		pipe2[2];	// between cat and wc
	pid_t	pid1, pid2, pid3;

	if (pipe(pipe1) == -1 || pipe(pipe2) == -1)
		return (perror("pipe"), EXIT_FAILURE);

	pid1 = fork();
	if (pid1 == 0)
	{
		// 🔁 child1: echo "hello from cmd1" → pipe1
		close(pipe1[0]);                            // Close read end
		dup2(pipe1[1], STDOUT_FILENO);              // stdout → pipe1[1]
		close(pipe1[1]);                            // Close original after dup
		close(pipe2[0]); close(pipe2[1]);           // Unused pipe2
		log_open_fds("child1");
		execlp("echo", "echo", "hello from cmd1", NULL);
		perror("exec echo");
		exit(EXIT_FAILURE);
	}

	pid2 = fork();
	if (pid2 == 0)
	{
		// 🔁 child2: cat < pipe1 → pipe2
		dup2(pipe1[0], STDIN_FILENO);               // stdin ← pipe1[0]
		close(pipe1[0]);                            // Close original
		close(pipe1[1]);                            // Close other pipe1 end
		dup2(pipe2[1], STDOUT_FILENO);              // stdout → pipe2[1]
		close(pipe2[1]);                            // Close after dup
		close(pipe2[0]);                            // Close read end
		log_open_fds("child2");
		execlp("cat", "cat", NULL);
		perror("exec cat");
		exit(EXIT_FAILURE);
	}

	pid3 = fork();
	if (pid3 == 0)
	{
		// 🔁 child3: wc -l < pipe2
		dup2(pipe2[0], STDIN_FILENO);               // stdin ← pipe2[0]
		close(pipe2[0]);                            // Close original after dup
		close(pipe2[1]);                            // Close write end
		close(pipe1[0]); close(pipe1[1]);           // Close unused pipe1
		log_open_fds("child3");
		execlp("wc", "wc", "-l", NULL);
		perror("exec wc");
		exit(EXIT_FAILURE);
	}

	// 🧹 Parent closes all pipe ends
	close(pipe1[0]); close(pipe1[1]);
	close(pipe2[0]); close(pipe2[1]);

	// 🧼 Wait for all children to exit
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	waitpid(pid3, NULL, 0);

	return (0);
}
