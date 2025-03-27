#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define YELL "\033[0;33m"
#define RESET "\033[0m"

void	print_error_exit(const char *label)
{
	perror(label);
	if (errno == ENOENT)
		exit(127); //command not found
	else
		exit(126); //permission denied
}

char	**build_args(const char *cmd)
{
	char	**args;
	args = malloc(sizeof(char *) * 3);
	if (!args)
		exit(EXIT_FAILURE);
	args[0] = strdup(cmd);
	args[1] = NULL;
	return (args);
}

void	run_cmd(const char *label, const char *cmd)
{
	pid_t	pid;
	char	**args;
	int		status;

	args = build_args(cmd);
	pid = fork();
	if (pid == 0)
	{
		printf(GREEN "[%s] execve('%s')...\n" RESET, label, cmd);
		execve(cmd, args, NULL);
		print_error_exit(cmd);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			int	exit_code = WEXITSTATUS(status);
			printf(RED   "[%s] exited with status: %d\n" RESET, label, exit_code);
		}
	}
	else
		perror("fork");
	free(args[0]);
	free(args);
}

int	main(void)
{
	printf(YELL "Testing pipex_execve error logic\n" RESET);

	//A. Command doesnt exist. (ENOENT)
	run_cmd("Test ENOENT ", "/not/a/real/cmd");

	//B. Script with no +x permission (EACCESS)
	system("echo 'echo denied' > ./noexec.sh");
	system("chmod 644 ./noexec.sh");
	run_cmd("Test EACCESS", "./noexec.sh");

	//C Bad binary. (ENEXEC) invalid ELF
	system("echo 'this is not a binary' > ./fakebin");
	system("chmod +x ./fakebin");
	run_cmd("Test ENOEXEC", "./fakebin");
	return (0);

}