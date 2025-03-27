#include "execve_wrapper.h"

int	main(void)
{
	char	*cmd[] = {"/bin/ls", "-la", NULL};

	exec_cmd(cmd[0], cmd, NULL);
	write(2, "exec_cmd: should not return\n", 29);
	return (EXIT_FAILURE);
}