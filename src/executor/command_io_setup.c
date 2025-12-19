/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   command_io_setup.c                                :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/06/09 18:12:11 by avaliull     #+#    #+#                  */
/*   Updated: 2025/06/12 18:20:40 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

static int	find_and_create_heredocs(
	t_msh_data *const msh_data,
	t_redir_list *redirection
)
{
	int	fd_or_exit_code;

	fd_or_exit_code = 0;
	while (redirection != NULL)
	{
		if (redirection->type == heredoc)
		{
			fd_or_exit_code = create_here_doc(msh_data,
					redirection->heredoc_delim);
			if (fd_or_exit_code > 0)
				redirection->dest_fd = fd_or_exit_code;
			else
				return (fd_or_exit_code);
		}
		redirection = redirection->next;
	}
	return (success);
}

int	prepare_command_io(
	t_msh_data *const msh_data,
	const t_exec_data *command,
	t_command_io *const comm_io,
	int i
)
{
	int	err_check;

	err_check = 0;
	if (command->input_is_pipe == true)
	{
		comm_io[i].in_pipe[READ_END] = comm_io[i - 1].out_pipe[READ_END];
		comm_io[i].in_pipe[WRITE_END] = comm_io[i - 1].out_pipe[WRITE_END];
	}
	if (command->output_is_pipe == true)
	{
		err_check = pipe(comm_io[i].out_pipe);
		if (err_check != success)
			return (msh_perror(NULL, PIPE_ERR, extern_err), pipe_err);
	}
	if (command->redirections)
		err_check = find_and_create_heredocs(msh_data, command->redirections);
	return (err_check);
}
