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
	// again, (likely) no reason to actuallly free in pipes
	//if (command->input_is_pipe == true)
	//{
	//	test_close(command_io->in_pipe[READ_END]);
	//	command_io->in_pipe[READ_END] = CLOSED_FD;
	//	(command_io - 1)->out_pipe[READ_END] = CLOSED_FD;
	//}
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
	if (command->builtin_name == not_builtin && command->argv[0])
		err_check = try_execve(minishell_data->env, command->argv);
	else
		err_check = exec_builtin(command, minishell_data);
	exit(err_check); // this needs to return for proper cleanup i think
	// double check
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
	if (command->input_is_pipe == true || command->output_is_pipe == true
		|| command->builtin_name == not_builtin)
	{
		process_id = fork();
		// this handle child process
		if (process_id == 0)
		{
			err_check = run_child_process(command, command_io, minishell_data);
			if (err_check == child_fd_err) // replace with a function that amtches all return error cases
				return (err_check);
		}
		// endof handle child process
		// this is handle parent process
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
		// endof handle parent process
		else if (process_id < 0)
			perror_and_return(NULL, FORK_ERR, extern_err, -1);
	}
	else if (command->builtin_name != not_builtin)
	{
		//err_check = exec_builtin(command, minishell_data);
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

void	heredoc_cleanup(
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
	free(exec_data);
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
	// question: can;t we omit clising in_pipes? they are always fro melft to right
	// and alwats equal to prevoius out_pipes right?
	while (++i < minishell_data->command_count)
	{
		free_and_close_exec_data(&exec_data[i]);
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
		//if (command_io[i].in_pipe[READ_END] > 2)
		//{
		//	test_close(command_io[i].in_pipe[READ_END]);
		//	//command_io[i].in_pipe[READ_END] = CLOSED_FD;
		//}
		//if (command_io[i].in_pipe[WRITE_END] > 2)
		//{
		//	test_close(command_io[i].in_pipe[WRITE_END]);
		//	//command_io[i].in_pipe[WRITE_END] = CLOSED_FD;
		//}
	}
	//(void) (command_io);
	//(void) (p_ids);
	//(void) (p_exit_codes);
	//(void) (exec_data);

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
	int	status_check; // change name? / why?
	int	i;

	i = -1;
	while (++i < command_count)
	{
		status_check = prepare_command_io(&exec_data[i], command_io, i);
		if (status_check < 0)
		{
			if (status_check == child_heredoc)
				return (child_heredoc);
			return (i);
		}
	}
	return (command_count);
}

int	execute_commands(
	t_minishell_data *const minishell_data,
	t_exec_data *command,
	t_command_io *command_io,
	int *p_id_arr // replace with pid_t
)
{
	int			i;
	t_msh_errno	err_check;

	i = -1;
	while (++i < minishell_data->command_count)
	{
		err_check = execute_command(&command[i], &command_io[i], minishell_data);
		printf("\033[36mexecuted command's child id: %d\033[0m\n", err_check);
		p_id_arr[i] = err_check;
		if (err_check < 0)
		{
			if (err_check == child_fd_err)
				return (err_check);
			//printf("PLACEHOLDER, ADD PROPER ERROR MANAGEMENT\n");
			// here, should check for when we actually need to stop. never questionmark?
		}
	}
	return (success);
}

// maybe rework this for more clarity on what happens on different exit situations
int	executor(
	t_minishell_data *const minishell_data,
	t_exec_data *exec_data,
	int command_count
)
{
	int				*p_id_arr;
	int				*p_exit_codes;
	int				pipeline_elem_count;
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
		perror_and_return(NULL, MALLOC_ERR, extern_err, errno);
	}
	minishell_data->last_pipeline_return = 0;
	/*	please explain to me this nonsense, past me	*/
	pipeline_elem_count = build_pipeline(exec_data, command_io, command_count); // this bad
	if (pipeline_elem_count == command_count)
		err = execute_commands(minishell_data, exec_data, command_io, p_id_arr); // very bad
	else
		command_count = pipeline_elem_count;
	if (err != success)
	{
		printf("\033[31error during command execution\033[0m\n"); // please kill me	
		executor_cleanup(minishell_data, exec_data, command_io, p_id_arr, p_exit_codes);
		return (err);
	}
	/*	endof explain nonsense	*/
	else if (command_count > 1 ||
		(command_count == 1 && exec_data->builtin_name == not_builtin))
		wait_for_children(command_count, minishell_data, p_id_arr, p_exit_codes);
	executor_cleanup(minishell_data, exec_data, command_io, p_id_arr, p_exit_codes);
	if (pipeline_elem_count < 0)
		return (pipeline_elem_count);
	return(success);
}
