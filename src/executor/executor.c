/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   executor.c                                        :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/06/03 15:24:57 by avaliull     #+#    #+#                  */
/*   Updated: 2025/07/18 20:30:51 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "executor.h"
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

static int	execute_pipeline(
	t_msh_data *const msh_data,
	int *p_id_arr,
	int *p_exit_codes,
	t_command_io *command_io
)
{
	int			elem_count;
	t_exec_data	*const exec_data = msh_data->exec_data;
	const int	command_count = msh_data->command_count;
	int			err;

	err = success;
	elem_count = 0;
	err = build_pipeline(msh_data, command_io, &elem_count);
	if (err != success)
		return (err);
	err = execute_commands(msh_data, exec_data, command_io, p_id_arr);
	if (err != success || msh_data->is_parent == false)
	{
		return (err);
	}
	if (command_count > 1 ||
		(command_count == 1 && exec_data->builtin_name == not_builtin))
		wait_for_children(elem_count, msh_data, p_id_arr, p_exit_codes);
	return (err);
}

static void	executor_cleanup(
	t_msh_data *const msh_data,
	t_command_io *command_io,
	int *p_ids,
	int *p_exit_codes
)
{
	int	i;

	i = -1;
	while (++i < msh_data->command_count)
	{
		free_and_close_exec_data(&msh_data->exec_data[i]);
		if (i == msh_data->command_count - 1
			|| command_io[i + 1].in_pipe[READ_END] != CLOSED_FD)
			safe_close(&command_io[i].out_pipe[READ_END]);
		if (i == msh_data->command_count - 1
			|| command_io[i + 1].in_pipe[WRITE_END] != CLOSED_FD)
			safe_close(&command_io[i].out_pipe[WRITE_END]);
	}
	free(command_io);
	free(p_ids);
	free(p_exit_codes);
	free(msh_data->exec_data);
	msh_data->exec_data = NULL;
}

int	executor(
	t_msh_data *const msh_data,
	int command_count
)
{
	int				*p_id_arr;
	int				*p_exit_codes;
	t_command_io	*command_io;
	int				err;

	if (msh_data->command_count == 0)
		return (success);
	handle_signals_non_interactive();
	err = success;
	p_id_arr = malloc(sizeof(int) * command_count);
	ft_memset(p_id_arr, -1, command_count);
	p_exit_codes = ft_calloc(sizeof(int), command_count);
	command_io = ft_calloc(sizeof(t_command_io), command_count);
	if (!p_id_arr || !p_exit_codes || !command_io)
	{
		msh_data->last_pipeline_return = errno;
		executor_cleanup(msh_data, command_io, p_id_arr, p_exit_codes);
		return (msh_perror(NULL, MALLOC_ERR, extern_err), malloc_err);
	}
	msh_data->last_pipeline_return = 0;
	err = execute_pipeline(msh_data, p_id_arr, p_exit_codes, command_io);
	executor_cleanup(msh_data, command_io, p_id_arr, p_exit_codes);
	return (err);
}
