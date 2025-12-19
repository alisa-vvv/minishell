/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   executor_test.c                                   :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/06/10 17:19:35 by avaliull     #+#    #+#                  */
/*   Updated: 2025/06/10 17:38:01 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

void	test_close(int fd)
{
	int	err_check;

	err_check = close(fd);
	if (err_check != 0)
	{
		dprintf(STDERR_FILENO, "bad close in pid: %d\n", getpid());
		dprintf(STDERR_FILENO, "this fd: %d\n", fd);
		if (errno == EBADF)
			perror("Check if doing weird closes");
		if (errno == EIO)
			perror("Probably serious hardware error");
		if (errno == EINTR)
			perror("Signal error - check if program functions normally");
		perror("Error during close");
	}
}

void	test_dup2(int oldfd, int newfd)
{
	int	err_check;

	err_check = dup2(oldfd, newfd);
	if (err_check < 0)
	{
		printf("bad dup in pid: %d\n", getpid());
		perror(DUP2_ERR);
	}
}
