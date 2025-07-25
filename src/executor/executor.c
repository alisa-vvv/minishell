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
#include <sys/wait.h>
#include <sys/types.h>
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
	return (process_id);
}

static int	wait_for_children(
	const int command_count,
	t_minishell_data *const minishell_data,
	const int *const p_ids,
	int *const p_exit_codes
)
{
	int	exit_status;
	int	i;
	int	error_check;

	i = command_count;
	while (--i >= 0)
	{
		//while (waitpid(p_ids[i], &p_exit_codes[i], 0) > 0) previously this was a loop, still not sure how waitpid works
		//to be tested further
		if (waitpid(p_ids[i], &p_exit_codes[i], 0) > 0) // check if there's some exit signals or codes we need to handle here
		{
			if (WIFEXITED(p_exit_codes[i]) == true)
			{
				exit_status = WEXITSTATUS(p_exit_codes[i]);
				if (exit_status != EXIT_SUCCESS &&
						minishell_data->last_pipeline_return == 0)
					minishell_data->last_pipeline_return = exit_status;
			}
		}
		else
		{
			printf("PLACEHOLDER ERROR, really freaky\n");
			return (EXIT_FAILURE);
		}
	}
	printf("pipeline return: %d\n", minishell_data->last_pipeline_return); // TEST ONLY, REMOVE LATER
	return (EXIT_SUCCESS);
}
// Idea for recording the return value of pipeline:
// do not free a command's exec_data. add a variable holding it's return value.
// after executing the entire pipeline, iterate through exec_data to check ////
// idea 2: use wait()
// record PID of each process, associate it with the command.
// while wait()ing, record the specific pid value in an array of return values
// then iterate through array from right to left until an error is found
// if no errors, return 0
int	executor(
	t_exec_data *exec_data,
	int command_count,
	t_minishell_data *const minishell_data
)
{
	int				i;
	int				*const p_ids = ft_calloc(sizeof(int), command_count);
	int				*const p_exit_codes = ft_calloc(sizeof(int), command_count);
	t_command_io	command_io;

	if (!p_ids || !p_exit_codes)
	{
		printf("PLACEHOLDER ERROR\n");
		return (EXIT_FAILURE);
	}
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
	minishell_data->last_pipeline_return = 0;
	wait_for_children(command_count, minishell_data, p_ids, p_exit_codes);
	free(p_ids);
	free(p_exit_codes);
	free(exec_data);
	return (EXIT_SUCCESS);
}
