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
#include <stdlib.h>
#include <fcntl.h>

static int	find_and_create_heredocs(
	t_redir_list *redirection
)	
{
	int	fd_or_exit_code;

	fd_or_exit_code = 0;
	while (redirection != NULL)
	{
		if (redirection->type == heredoc)
		{
			fd_or_exit_code = create_here_doc(redirection->heredoc_delim);	
			if (fd_or_exit_code > 0)
				redirection->dest_fd = fd_or_exit_code;
			else
				return (fd_or_exit_code);
		}
		redirection = redirection->next;
	}
	return (0);
}

int g_TEST_IO;
int	prepare_command_io(
	const t_exec_data *command,
	t_command_io *const command_io,
	int i
)
{
	int	err_check; // probably also change name

	//g_TEST_IO++;
	err_check = 0;
	if (command->input_is_pipe == true)
	{
		command_io[i].in_pipe[READ_END] = command_io[i - 1].out_pipe[READ_END];
		command_io[i].in_pipe[WRITE_END] = command_io[i - 1].out_pipe[WRITE_END];
		//printf("\033[36mpipe fds? %d\033[0m\n", command_io[i].in_pipe[READ_END]);
		//printf("\033[36mpipe fds write? %d\033[0m\n", command_io[i].in_pipe[WRITE_END]);
	}
	if (command->output_is_pipe == true)
	{
		err_check = pipe(command_io[i].out_pipe);
		if (err_check < 0)
			return (msh_perror(NULL, PIPE_ERR, extern_err), pipe_err); // do we need to reutrn here???
	}
	if (command->redirections)
		err_check = find_and_create_heredocs(command->redirections);
	//if (g_TEST_IO == 1)
	//	return (-1);
	return (err_check);
}
