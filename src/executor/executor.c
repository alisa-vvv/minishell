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
	t_exec_data *command,
	t_command_io *const command_io
)
{
	if (command->input_is_pipe == true)
	{
		test_close(command_io->in_pipe[READ_END]);
		command_io->in_pipe[READ_END] = CLOSED_FD;
		(command_io - 1)->out_pipe[READ_END] = CLOSED_FD;
	}
	if (command->output_is_pipe == true)
	{
		test_close(command_io->out_pipe[WRITE_END]);
		command_io->out_pipe[WRITE_END] = CLOSED_FD;
		(command_io + 1)->in_pipe[WRITE_END] = CLOSED_FD;
	}
	free_and_close_exec_data(command);
	return (0);
}

static int	run_child_process(
	t_exec_data *const command,
	t_command_io *const command_io,
	t_minishell_data *const minishell_data
)
{
	int				err_check;

	if (command->input_is_pipe == true)
	{
		test_dup2(command_io->in_pipe[READ_END], STDIN_FILENO);
		test_close(command_io->in_pipe[READ_END]);
		command_io->in_pipe[READ_END] = CLOSED_FD;
	}
	if (command->output_is_pipe == true)
	{
		test_dup2(command_io->out_pipe[WRITE_END], STDOUT_FILENO);
		test_close(command_io->out_pipe[WRITE_END]);
		command_io->out_pipe[WRITE_END] = CLOSED_FD;
	}
	if (command->redirections)
		err_check = perform_redirections(command->redirections, command_io);
	if (command->builtin_name == not_builtin && command->argv[0])
		err_check = try_execve(minishell_data->env, command->argv);
	else
		err_check = exec_builtin(command, minishell_data);
	exit(err_check);
}

static int	execute_command(
	t_exec_data *command,
	t_command_io *const command_io,
	t_minishell_data *const minishell_data
)
{
	pid_t	process_id;
	int		err_check;

	err_check = 0;
	process_id = 0;
	if (command->input_is_pipe == true || command->output_is_pipe == true ||
		command->builtin_name == not_builtin)
	{
		process_id = fork();
		if (process_id == 0)
			err_check = run_child_process(command, command_io, minishell_data);
		else if (process_id > 0)
		{
			err_check = cleanup_in_parent_process(command, command_io);
			if (err_check == 0)
				return (process_id);
			else
			{
				printf("PLACEHOLDER, ADD ERROR MANAGEMENT\n");
				return (err_check);
			}
		}
		else if (process_id < 0)
			perror_and_return(NULL, FORK_ERR, extern_err, -1);
	}
	else if (command->builtin_name != not_builtin)
	{
		err_check = exec_builtin(command, minishell_data);
		return (err_check);
	}
	return (err_check);
}

static int	wait_for_children(
	const int command_count,
	t_minishell_data *const minishell_data,
	const int *const p_ids,
	int *const p_exit_codes
)
{
	int	last_exit;
	int	i;
	int	error_check;

	i = -1;
	printf("command count: %d\n", command_count);
	while (++i < command_count)
	{
		printf("\np_ids[%d]: %d\n", i, p_ids[i]);
		printf("p_exit_codes[%d]: %d\n\n", i, p_exit_codes[i]);
		if (waitpid(p_ids[i], &p_exit_codes[i], 0) > 0) // check if there's some exit signals or codes we need to handle here
		{
			//https://tldp.org/LDP/abs/html/exitcodes.html - good source for exit codes testing
			if (WIFEXITED(p_exit_codes[i]) == true)
			{
				last_exit = WEXITSTATUS(p_exit_codes[i]);
				if (last_exit != EXIT_SUCCESS)
					minishell_data->last_pipeline_return = last_exit;
			}
			else if (WIFSIGNALED(p_exit_codes[i]) == true)
			{
				last_exit = 128 + WTERMSIG(p_exit_codes[i]);
				if (WCOREDUMP(p_exit_codes[i]))
					ft_putstr_fd("Core dumped\n", STDERR_FILENO);
			}
		}
		else
		{
			// we probably don't need this check at all
			perror("waitpid -1");
		}
	}
	return (EXIT_SUCCESS);
}

static void	executor_cleanup(
	t_minishell_data *const minishell_data,
	t_exec_data *exec_data,
	t_command_io *command_io,
	int *p_ids,
	int *p_exit_codes
)
{
	int	i;

	i = -1;
	while (++i < minishell_data->command_count)
	{
		//marking these as closed is most likely useless. remove unless proven otherwise.
		free_and_close_exec_data(&exec_data[i]);
		if (command_io[i].out_pipe[READ_END] > 2)
		{
			printf("which command: %d\n", i);
			printf("is it this one?\n");
			test_close(command_io[i].out_pipe[READ_END]);
			//command_io[i].out_pipe[READ_END] = CLOSED_FD;
		}
		if (command_io[i].out_pipe[WRITE_END] > 2)
		{
			test_close(command_io[i].out_pipe[WRITE_END]);
			//command_io[i].out_pipe[WRITE_END] = CLOSED_FD;
		}
		if (command_io[i].in_pipe[READ_END] > 2)
		{
			test_close(command_io[i].in_pipe[READ_END]);
			//command_io[i].in_pipe[READ_END] = CLOSED_FD;
		}
		if (command_io[i].in_pipe[WRITE_END] > 2)
		{
			test_close(command_io[i].in_pipe[WRITE_END]);
			//command_io[i].in_pipe[WRITE_END] = CLOSED_FD;
		}
	}
	free(command_io);
	free(p_ids);
	free(p_exit_codes);
	free(exec_data);
}

int	build_pipeline(
	t_exec_data *exec_data,
	t_command_io *command_io,
	int command_count
)
{
	int	i;

	i = -1;
	while (++i < command_count)
	{
		if (prepare_command_io(&exec_data[i], command_io, i) < 0)
			return (i);
	}
	return (command_count);
}

void	execute_commands(
	t_minishell_data *const minishell_data,
	t_exec_data *command,
	t_command_io *command_io,
	int *p_id
)
{
	int	i;

	i = -1;
	while (++i < minishell_data->command_count)
		p_id[i] = execute_command(&command[i], &command_io[i], minishell_data);
}

int	executor(
	t_minishell_data *const minishell_data,
	t_exec_data *exec_data,
	int command_count
)
{
	int				*p_id;
	int				*p_exit_codes;
	int				pipeline_elem_count;
	t_command_io	*command_io;

	p_id = ft_calloc(sizeof(int), command_count);
	p_exit_codes = ft_calloc(sizeof(int), command_count);
	command_io = ft_calloc(sizeof(t_command_io), command_count);
	if (!p_id || !p_exit_codes || !command_io)
	{
		minishell_data->last_pipeline_return = errno;
		perror_and_return(NULL, MALLOC_ERR, extern_err, errno);
	}
	minishell_data->last_pipeline_return = 0;
	pipeline_elem_count = build_pipeline(exec_data, command_io, command_count);
	if (pipeline_elem_count == command_count)
		execute_commands(minishell_data, exec_data, command_io, p_id);
	else
		command_count = pipeline_elem_count;
	if (command_count > 1 ||
		(command_count == 1 && exec_data->builtin_name == not_builtin))
		wait_for_children(command_count, minishell_data, p_id, p_exit_codes);
	executor_cleanup(minishell_data, exec_data, command_io, p_id, p_exit_codes);
	return(EXIT_SUCCESS);
}
