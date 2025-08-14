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
		test_close(command_io->in_pipe[READ_END]);
	if (command->output_is_pipe == true)
		test_close(command_io->out_pipe[WRITE_END]);
	free_and_close_exec_data(command);
	return (0);
}

static int	run_child_process(
	t_exec_data *const command,
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
	err_check = perform_redirections(command->redirections, command_io);
	if (command->is_builtin == false)
		err_check = try_execve(minishell_data->env, command->argv);
	else if (command->is_builtin == true)
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
		command->is_builtin == false)
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
	else if (command->is_builtin == true)
	{
		err_check = exec_builtin(command, minishell_data);
		return (err_check);
	}
	// this should never reach unless error
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
		//while (waitpid(p_ids[i], &p_exit_codes[i], 0) > 0) previously this was a loop, still not sure how waitpid works
		//to be tested further
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
		else // this entire thing is questionable. i dont understand this
		{
			// we probably don't need this check at all
			perror("waitpid -1");
		}
	}
	return (EXIT_SUCCESS);
}

void	executor_cleanup(
	t_minishell_data *const minishell_data,
	t_exec_data *exec_data,
	int *p_ids,
	int *p_exit_codes
)
{
	int	i;

	i = -1;
	while (++i < minishell_data->command_count)
		free_and_close_exec_data(&exec_data[i]);
	free(p_ids);
	free(p_exit_codes);
	free(exec_data);
}
//
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
	int				*p_ids = ft_calloc(sizeof(int), command_count);
	int				*p_exit_codes = ft_calloc(sizeof(int), command_count);
	t_command_io	command_io;

	if (!p_ids || !p_exit_codes)
	{
		minishell_data->last_pipeline_return = errno;
		perror_and_return(NULL, MALLOC_ERR, extern_err, errno);
	}
	i = -1;
	while (++i < command_count)
	{
		if (prepare_command_io(&exec_data[i], &command_io) < 0)
		{
			// so current logic is that if pipe breaks it will stop further execution
			// and adjust command_count so witpid oesnt wait for fake children
			// feels kinda wacky, probably bad solutuibn
			command_count = i; // this is weird but it's for waiting
			break ;
		}
		p_ids[i] = execute_command(&exec_data[i], &command_io, minishell_data);
	}
	minishell_data->last_pipeline_return = 0;
	// will wait only if there is more than 1 command and/or of the one command is not builtin
	// question: should single commands be executed in shell?
	if (command_count > 1 ||
		(command_count == 1 && exec_data->is_builtin == false))
		wait_for_children(command_count, minishell_data, p_ids, p_exit_codes);
	executor_cleanup(minishell_data, exec_data, p_ids, p_exit_codes);
	return(EXIT_SUCCESS);
}
