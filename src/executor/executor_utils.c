/* ************************************************************************** */
/*                                                                            */
/*                                                            ::::::::        */
/*   executor_utils.c                                        :+:    :+:       */
/*                                                          +:+               */
/*   By: avaliull <avaliull@student.codam.nl>              +#+                */
/*                                                        +#+                 */
/*   Created: 2025/11/06 15:40:46 by avaliull            #+#    #+#           */
/*   Updated: 2025/12/16 17:57:06 by avaliull            ########   odam.nl   */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void	safe_close(
	int *fd)
{
	if (*fd == CLOSED_FD || *fd == STDOUT_FILENO || *fd == STDIN_FILENO
		|| *fd == STDERR_FILENO)
	{
		return ;
	}
	test_close(*fd);
	*fd = CLOSED_FD;
}

int		wait_for_children(
	const int command_count,
	t_msh_data *const msh_data,
	const int *const p_ids,
	int *const p_exit_codes)
{
	int	last_exit;
	int	i;

	i = -1;
	while (++i < command_count)
	{
		if (p_ids[i] < 0)
			continue ;
		waitpid(p_ids[i], &p_exit_codes[i], 0);
		if (WIFEXITED(p_exit_codes[i]) == true)
		{
			last_exit = WEXITSTATUS(p_exit_codes[i]);
			msh_data->last_pipeline_return (= last_exit);
		}
	}
	return (EXIT_SUCCESS);
}
