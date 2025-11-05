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

	err_check = 0;
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
		err_check = perform_redirections(command->redirections);
	if (err_check != success) // it looks like the processs does not run if it fails
	{
		if (err_check == fd_err) // add reasnable system for just adding child process
			return (child_fd_err);
	}
	// to redirect stuff, so we need to clean and propogate to exit
	if (command->builtin_name == not_builtin && command->argv) // whats up with sechond condition>?
		err_check = try_execve(minishell_data->env, command->argv);
	else
		err_check = exec_builtin(command, minishell_data);
	return (err_check); // this needs to return for proper cleanup i think
	// double check
}

static int	execute_command(
	t_exec_data *command,
	t_command_io *const command_io,
	t_minishell_data *const minishell_data,
	pid_t *pid
)
{
	int		err_check;

	err_check = 0;
	*pid = 0;
	if (command->input_is_pipe == true || command->output_is_pipe == true
		|| command->builtin_name == not_builtin)
	{
		*pid = fork();
		// this is handle child process
		if (*pid == 0)
		{
			err_check = run_child_process(command, command_io, minishell_data);
			//if (err_check != child_fd_err) // replace with a function that amtches all return error cases
			return (err_check); // this should never be reached unless there is an error I think
		}
		// endof handle child process
		// this is handle parent process
		else if (*pid > 0)
		{
			err_check = cleanup_in_parent_process(command, command_io);
			return (err_check);
			//if (err_check == 0)
			//	return (*pid);
			//else
			//{
			//	printf("PLACEHOLDER, ADD ERROR MANAGEMENT\n");
			//	return (err_check);
			//}
		}
		// endof handle parent process
		else if (*pid < 0)
			return (msh_perror(NULL, FORK_ERR, extern_err), fork_err); // check prefix
	}
	else if (command->builtin_name != not_builtin)
	{
		err_check = exec_builtin(command, minishell_data);
		//here we probably don't exit either. check.
		//most likely we send an error msg locally and then proceed with execution
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
	int	err_check;

	i = -1;
	//printf("command count: %d\n", command_count);
	while (++i < command_count)
	{
	//	printf("\np_ids[%d]: %d\n", i, p_ids[i]);
	//	printf("p_exit_codes[%d]: %d\n\n", i, p_exit_codes[i]);
		if (p_ids[i] < 0)
			continue ;
		err_check = waitpid(p_ids[i], &p_exit_codes[i], 0);
		if (err_check > 0) // check if there's some exit signals or codes we need to handle here
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
	t_command_io *command_io,
	int *p_ids,
	int *p_exit_codes
)
{
	int	i;

	i = -1;
	// double check if there are ever cases where we need to close input pipes.
	while (++i < minishell_data->command_count)
	{
		free_and_close_exec_data(&minishell_data->exec_data[i]);
		if (command_io[i].out_pipe[READ_END] > 2)
		{
			test_close(command_io[i].out_pipe[READ_END]);
			//command_io[i].out_pipe[READ_END] = CLOSED_FD;
		}
		if (command_io[i].out_pipe[WRITE_END] > 2)
		{
			test_close(command_io[i].out_pipe[WRITE_END]);
			//command_io[i].out_pipe[WRITE_END] = CLOSED_FD;
		}
	}
	free(command_io);
	free(p_ids);
	free(p_exit_codes);
	free(minishell_data->exec_data);
	minishell_data->exec_data = NULL;
}

// this functiohn will return either the amount of commands in the pipeline,
// or an error code.
// if a pipe creation fails, it will return the number of pipes built.
// this will tell the caller function that not all pipes were created.
// currently, what happens on fail is unclear
int	build_pipeline(
	t_exec_data *exec_data,
	t_command_io *command_io,
	int command_count,
	int *elem_count
)
{
	int	status_check;
	int	i;

	i = -1;
	status_check = success;
	while (++i < command_count)
	{
		status_check = prepare_command_io(&exec_data[i], command_io, i);
		if (status_check != success)
			break ;
	}
	*elem_count = i;
	return (status_check);
}

int	execute_commands(
	t_minishell_data *const minishell_data,
	t_exec_data *command,
	t_command_io *command_io,
	int *p_id_arr // replace with pid_t
)
{
	int			i;
	t_msh_errno	err;

	i = -1;
	printf("hello? what is command count? %d\n", minishell_data->command_count);
	while (++i < minishell_data->command_count)
	{
		err = execute_command(&command[i], &command_io[i], minishell_data, &p_id_arr[i]);
		printf("\033[36mexecuted command's child id: %d\033[0m\n", err);
		if (err != success)
		{
			printf("what is err check here? %d\n", err);
			if (err == child_heredoc || err == malloc_err
				|| err == no_command)
			{
				//printf("PLACEHOLDER, ADD PROPER ERROR MANAGEMENT\n");
				// here, should check for when we actually need to stop. never questionmark?
				return (err);
			}
		}
	}
	return (success);
}

// maybe rework this for more clarity on what happens on different exit situations
static int	execute_pipeline(
	t_minishell_data *const minishell_data,
	int				*p_id_arr,
	int				*p_exit_codes,
	t_command_io	*command_io
)
{
	int			elem_count;
	t_exec_data	*const exec_data = minishell_data->exec_data;
	const int	command_count = minishell_data->command_count;
	int			err;

	err = success;
	elem_count = 0;
	err = build_pipeline(exec_data, command_io, command_count, &elem_count);
	if (err != success)
	{
		printf("returning from process: %d, getpid(), return value: %d\n", getpid(), err);
		return (err); // current logic is simply do not execuote if can;t establish pipeline. may change to execute parts of it
	}
	err = execute_commands(minishell_data, exec_data, command_io, p_id_arr);
	if (err != success)
	{
		printf("\033[31error during command execution\033[0m\n"); // please kill me	
		return (err);
	}
	if (command_count > 1 ||
		(command_count == 1 && exec_data->builtin_name == not_builtin))
		wait_for_children(elem_count, minishell_data, p_id_arr, p_exit_codes);
	return (err);
}

int	executor(
	t_minishell_data *const minishell_data,
	int command_count
)
{
	int				*p_id_arr;
	int				*p_exit_codes;
	t_command_io	*command_io;
	int				err;

	err = success;
	p_id_arr = malloc(sizeof(int) * command_count);
	ft_memset(p_id_arr, -1, command_count);
	p_exit_codes = ft_calloc(sizeof(int), command_count);
	command_io = ft_calloc(sizeof(t_command_io), command_count);
	if (!p_id_arr || !p_exit_codes || !command_io)
	{
		minishell_data->last_pipeline_return = errno;
		executor_cleanup(minishell_data, command_io, p_id_arr, p_exit_codes);
		return (msh_perror(NULL, MALLOC_ERR, extern_err), errno);
	}
	minishell_data->last_pipeline_return = 0;
	err = execute_pipeline(minishell_data, p_id_arr, p_exit_codes, command_io);
	executor_cleanup(minishell_data, command_io, p_id_arr, p_exit_codes);
	return (err);
}
