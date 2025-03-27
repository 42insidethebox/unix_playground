#include "utils.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

void	log_fd_table(const char *label)
{
	int	fd;

	fd = 0;
	while (fd < 1024)
	{
		if (fcntl(fd, F_GETFD) != -1)
			printf("[%s] FD %d is OPEN\n", label, fd);
		fd++;
	}
	printf("\n");
}
