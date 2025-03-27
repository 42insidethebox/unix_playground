#include "execve_wrapper.h"

int	exec_cmd(char *path, char **argv, char **envp)
{
	if (!path || !argv || !argv[0])
	{
		write(2, "exec_cmd: invalid arguments\n", 28);
	}
	if (execve(path, argv, envp) == -1)
	{
		write(2, "exec_cmd", 10);
		perror(path);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}