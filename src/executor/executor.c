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
#include "builtins.h"
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

static int	cleanup_in_parent_process(
	const t_exec_data *command,
	t_command_io *const command_io
)
{
	t_redir_list	*redirection;

	redirection = command->redirections;
	if (command->input_is_pipe == true)
		test_close(command_io->in_pipe[READ_END]);
	if (command->output_is_pipe == true)
		test_close(command_io->out_pipe[WRITE_END]);
	free_and_close_exec_data((t_exec_data *) command);
	return (0);
}

static int	run_child_process(
	const t_exec_data *const command,
	const t_command_io *const command_io,
	t_minishell_data *const minishell_data
)
{
	int				err_check;

	if (command->input_is_pipe == true)
	{
		test_dup2(command_io->in_pipe[READ_END], STDIN_FILENO);
		test_close(command_io->in_pipe[READ_END]);
	}
	if (command->output_is_pipe == true)
	{
		test_dup2(command_io->out_pipe[WRITE_END], STDOUT_FILENO);
		test_close(command_io->out_pipe[WRITE_END]);
	}
	perform_redirections(command->redirections, command_io);
	if (command->is_builtin == false)
		try_execve(minishell_data->env, command->argv);
	else if (command->is_builtin == true)
		err_check = exec_builtin(command, minishell_data);
	exit(err_check);
}

static int	execute_command(
	const t_exec_data *command,
	t_command_io *const command_io,
	t_minishell_data *const minishell_data
)
{
	pid_t	process_id;
	int		err_check;

	err_check = 0;
	if (command->input_is_pipe == true || command->output_is_pipe == true ||
		command->is_builtin == false)
	{
		process_id = fork();
		if (process_id == 0)
			run_child_process(command, command_io, minishell_data);
		else if (process_id > 0)
			cleanup_in_parent_process(command, command_io);
		else if (process_id < 0)
			perror_and_return(FORK_ERR, LIBFUNC_ERR, EXIT_FAILURE);
	}
	else if (command->is_builtin == true)
		err_check = exec_builtin(command, minishell_data);
	return (err_check);
}

int	executor(
	t_exec_data *exec_data,
	int command_count,
	t_minishell_data *const minishell_data
)
{
	int				i;
	int				process_status;
	t_command_io	command_io;

	i = -1;
	// CURRENT LOGIC: fail in a process wil never stop execution of other commands. last exittable fail check was during pipe setup
	while (++i < command_count)
	{
		if (prepare_command_io(&exec_data[i], &command_io) < 0)
		{
			// QUESTION: WHAT IS THE BEHAVIOUR IF HEREDOC CAN'T BE CREATED?
			printf("PLACEHOLDER ERROR\n");
			return (EXIT_FAILURE);
		}
		execute_command(&exec_data[i], &command_io, minishell_data);
	}
	while (waitpid(-1, &process_status, 0) > 0) // check if there's some exit signals or codes we need to handle here
	{
		if (WIFEXITED(process_status) == true)
			if (WEXITSTATUS(process_status) != EXIT_SUCCESS)
			{
				// this should probably be removed and only relevant things need to be shown
				// check gnu manual? I don;t know where to find things that are re;evan0t
				//printf("exit status: %d\n", process_status == EBADF);
				//perror("PLACEHOLDER, Child process sent an error");
			}
	}
	return (EXIT_SUCCESS);
}
