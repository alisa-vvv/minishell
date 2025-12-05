/* ************************************************************************** */
/*                                                                            */
/*                                                            ::::::::        */
/*   executor_utils.c                                        :+:    :+:       */
/*                                                          +:+               */
/*   By: avaliull <avaliull@student.codam.nl>              +#+                */
/*                                                        +#+                 */
/*   Created: 2025/11/06 15:40:46 by avaliull            #+#    #+#           */
/*   Updated: 2025/11/06 15:48:03 by avaliull            ########   odam.nl   */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "executor.h"

void	safe_close(
	int *fd
)
{
	if (*fd == CLOSED_FD
		|| *fd == STDOUT_FILENO || *fd == STDIN_FILENO || *fd == STDERR_FILENO)
	{
		return ;
	}
	test_close(*fd);
	*fd = CLOSED_FD;
}
