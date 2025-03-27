#ifndef EXEC_WRAPPER_H
# define EXEC_WRAPPER_H

# include <unistd.h>     
# include <stdlib.h>     
# include <stdio.h>     
# include <errno.h>     
# include <string.h>    

int	exec_cmd(char *path, char **argv, char **envp);

#endif
