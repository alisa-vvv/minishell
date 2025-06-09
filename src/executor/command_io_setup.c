/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   command_io_setup.c                                :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/06/09 18:12:11 by avaliull     #+#    #+#                  */
/*   Updated: 2025/06/09 19:25:35 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include <fcntl.h>

static int	set_in_fd(
	const t_exec_data *command,
	int *const in_fd,
	int heredoc_pipe[2]
)
{
	int	err_check;

	err_check = 0;
	if (command->input_type == custom_fd)
		*in_fd = open(command->in_filename, O_RDONLY);
	else if (command->input_type == heredoc)
	{
		err_check = create_here_doc(command, heredoc_pipe);
		if (err_check != 0)
			return (err_check);
		*in_fd = heredoc_pipe[READ_END];
	}
	if (*in_fd < 0)
		perror_and_return(FD_ERR, LIBFUNC_ERR);
	return (err_check);
}

static void	set_out_fd(
	const t_exec_data *command,
	int *const out_fd
)
{
	const int	truncate_flags = O_WRONLY | O_CREAT | O_TRUNC;
	const int	append_flags = O_WRONLY | O_CREAT | O_APPEND;

	if (command->output_type == custom_fd
		&& command->redirect_type == trunc)
		*out_fd = open(command->out_filename, truncate_flags, 0664);
	else if (command->output_type == custom_fd
		&& command->redirect_type == append)
		*out_fd = open(command->out_filename, append_flags, 0664);
	else if (command->output_type == std_err)
		*out_fd = STDERR_FILENO;
}

int	prepare_command_io(
	const t_exec_data *command,
	t_command_io *const command_io
)
{
	int	err_check;

	if (command->input_type == pipe_read)
	{
		command_io->in_fd = command_io->out_pipe[READ_END];
		command_io->in_pipe[READ_END] = command_io->out_pipe[READ_END];
		command_io->in_pipe[WRITE_END] = command_io->out_pipe[WRITE_END];
	}
	else
		err_check = set_in_fd(command, &command_io->in_fd, command_io->in_pipe);
	if (command_io->in_fd < 0 || err_check < 0)
		return (1);

	if (command->output_type == pipe_write)
	{
		err_check = pipe(command_io->out_pipe);
		if (err_check < 0)
		{
			// ADD ERROR MANAGEMENT!!
			printf("PLACEHOLDER, THIS SHOULD ERROR\n");
		}
		command_io->out_fd = command_io->out_pipe[WRITE_END];
	}
	else
		set_out_fd(command, &command_io->out_fd);
	if (command_io->in_fd < 0)
	{
		// ADD ERROR MANAGEMENT!!
		printf("PLACEHOLDER, THIS SHOULD ERROR\n");
	}
	return (err_check);
}
