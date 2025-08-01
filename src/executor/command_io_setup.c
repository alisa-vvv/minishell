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
	while (redirection != NULL)
	{
		if (redirection->type == heredoc)
			redirection->dest_fd = create_here_doc(redirection->heredoc_delim);
		if (redirection->dest_fd < 0)
			return (-1);
		redirection = redirection->next;
	}
	return (0);
}

int	prepare_command_io(
	const t_exec_data *command,
	t_command_io *const command_io
)
{
	int	err_check;

	if (command->input_is_pipe == true)
	{
		command_io->in_pipe[READ_END] = command_io->out_pipe[READ_END];
		command_io->in_pipe[WRITE_END] = command_io->out_pipe[WRITE_END];
	}
	if (command->output_is_pipe == true)
	{
		err_check = pipe(command_io->out_pipe);
		if (err_check < 0)
			perror_and_return(NULL, PIPE_ERR, extern_err, -1);
	}
	err_check = find_and_create_heredocs(command->redirections);
	return (err_check);
}
