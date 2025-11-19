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
#include <unistd.h>
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
		safe_close(&command_io->out_pipe[WRITE_END]);
		// this line below might be obsolete now. CHECK. go through all cases.
		//(command_io + 1)->in_pipe[WRITE_END] = CLOSED_FD; // fix invalid read here /?
	}
	free_and_close_exec_data(command);
	return (success);
}

static int	run_child_process(
	t_exec_data *const command,
	t_command_io *const command_io,
	t_msh_data *const msh_data
)
{
	int				err_check;

	err_check = success;
	if (command->input_is_pipe == true)
	{
		test_dup2(command_io->in_pipe[READ_END], STDIN_FILENO);
		safe_close(&command_io->in_pipe[READ_END]);
	}
	if (command->output_is_pipe == true)
	{
		test_dup2(command_io->out_pipe[WRITE_END], STDOUT_FILENO);
		safe_close(&command_io->out_pipe[WRITE_END]);
	}
	if (command->redirections)
		err_check = perform_redirections(command->redirections, NULL, false);
	if (err_check != success)
		return (err_check);
	if (command->builtin_name == not_builtin)
		err_check = try_execve(msh_data->env, command->argv);
	else
		err_check = exec_builtin(command, msh_data);
	return (err_check);
}

static int	execute_in_child(
	t_exec_data *command,
	t_command_io *const command_io,
	t_msh_data *const msh_data,
	pid_t *pid
)
{
	int	err_check;

	err_check = success;
	*pid = fork();
	if (*pid == 0)
	{
		msh_data->is_parent = false;
		err_check = run_child_process(command, command_io, msh_data);
	}
	else if (*pid > 0)
	{
		//dprintf(STDERR_FILENO,"what is *pid when command is %s: %d\n", command->argv[0], *pid);
		err_check = cleanup_in_parent_process(command, command_io);
	}
	else if (*pid < 0)
		return (msh_perror(NULL, FORK_ERR, extern_err), fork_err); // check prefix
	return (err_check);
}

static int	execute_command(
	t_exec_data *command,
	t_command_io *const command_io,
	t_msh_data *const msh_data,
	pid_t *pid
)
{
	int				err_check;
	t_undup_list	**undup_list_head;
	t_undup_list	*undup_list;

	err_check = success;
	*pid = 0;
	if (command->input_is_pipe == true || command->output_is_pipe == true
		|| command->builtin_name == not_builtin)
	{
		return (execute_in_child(command, command_io, msh_data, pid));
	}
	if (command->redirections)
	{
		undup_list = NULL;
		undup_list_head = &undup_list;
		perform_redirections(command->redirections, undup_list_head, true); // add error checking here
	}
	if (command->builtin_name != not_builtin)
		err_check = exec_builtin(command, msh_data);
	if (command->redirections)
		undup_redirections(undup_list_head);
	return (err_check);
}

static int	wait_for_children(
	const int command_count,
	t_msh_data *const msh_data,
	const int *const p_ids,
	int *const p_exit_codes
)
{
	int	last_exit;
	int	i;
	int	err_check;

	i = -1;
	while (++i < command_count)
	{
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
					msh_data->last_pipeline_return = last_exit;
			}
			else if (WIFSIGNALED(p_exit_codes[i]) == true)
			{
				last_exit = 128 + WTERMSIG(p_exit_codes[i]);
				if (WCOREDUMP(p_exit_codes[i]))
					ft_putstr_fd("Core dumped\n", STDERR_FILENO);
			}
		}
		else
			perror("waitpid -1"); // check necessity
	}
	return (EXIT_SUCCESS);
}

static void	executor_cleanup(
	t_msh_data *const msh_data,
	t_command_io *command_io,
	int *p_ids,
	int *p_exit_codes
)
{
	int	i;

	i = -1;
	while (++i < msh_data->command_count)
	{
		free_and_close_exec_data(&msh_data->exec_data[i]);
		if (i == msh_data->command_count - 1
			|| command_io[i + 1].in_pipe[READ_END] != CLOSED_FD)
			safe_close(&command_io[i].out_pipe[READ_END]);
		if (i == msh_data->command_count - 1
			|| command_io[i + 1].in_pipe[WRITE_END] != CLOSED_FD)
			safe_close(&command_io[i].out_pipe[WRITE_END]);
	}
	free(command_io);
	free(p_ids);
	free(p_exit_codes);
	free(msh_data->exec_data);
	msh_data->exec_data = NULL;
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
	t_msh_data *const msh_data,
	t_exec_data *command,
	t_command_io *command_io,
	int *p_id_arr // replace with pid_t
)
{
	int			i;
	t_msh_errno	err;

	i = -1;
	while (++i < msh_data->command_count)
	{
		err = execute_command(&command[i], &command_io[i], msh_data, &p_id_arr[i]);
		if (msh_data->is_parent == false)
			return (err);
	}
	return (success);
}

// maybe rework this for more clarity on what happens on different exit situations
static int	execute_pipeline(
	t_msh_data *const msh_data,
	int *p_id_arr,
	int *p_exit_codes,
	t_command_io *command_io
)
{
	int			elem_count;
	t_exec_data	*const exec_data = msh_data->exec_data;
	const int	command_count = msh_data->command_count;
	int			err;

	err = success;
	elem_count = 0;
	err = build_pipeline(exec_data, command_io, command_count, &elem_count);
	if (err != success)
		return (err); // current logic is simply do not execuote if can;t establish pipeline. may change to execute parts of it
	err = execute_commands(msh_data, exec_data, command_io, p_id_arr);
	if (err != success || msh_data->is_parent == false)
		return (err);
	if (command_count > 1 ||
		(command_count == 1 && exec_data->builtin_name == not_builtin))
		wait_for_children(elem_count, msh_data, p_id_arr, p_exit_codes);
	return (err);
}

//static int	validate_redirections(
//	t_exec_data *const exec_data,
//	int command_count
//)
//{
//	t_redir_list	*cur_redir;
//	int				i;
//	int				access_mode;
//	int				err;
//
//	err = success;
//	i = 0;
//	while (i < command_count)
//	{
//		dprintf(STDERR_FILENO, "huh\n");
//		cur_redir = exec_data[i].redirections;
//		while (cur_redir != NULL)
//		{
//			if (!cur_redir->dest_filename) // check for with closed STDIN/STDOUT!
//			{
//				cur_redir = cur_redir->next;
//				continue ;
//			}
//			if (cur_redir->type == trunc || cur_redir->type == append)
//				access_mode = W_OK;
//			else if (cur_redir->type == input)
//				access_mode = R_OK;
//			if (access(cur_redir->dest_filename, access_mode) != success)
//			{
//				msh_perror(NULL, cur_redir->dest_filename, extern_err);
//				err = fd_err;
//			}
//			cur_redir = cur_redir->next;
//		}
//		i++;
//	}
//	return (err);
//}

int	executor(
	t_msh_data *const msh_data,
	int command_count
)
{
	int				*p_id_arr;
	int				*p_exit_codes;
	t_command_io	*command_io;
	int				err;

	err = success;
//	err = validate_redirections(msh_data->exec_data, command_count);
	//if (err != success)
	//	return (err);
	p_id_arr = malloc(sizeof(int) * command_count);
	ft_memset(p_id_arr, -1, command_count);
	p_exit_codes = ft_calloc(sizeof(int), command_count);
	command_io = ft_calloc(sizeof(t_command_io), command_count);
	if (!p_id_arr || !p_exit_codes || !command_io)
	{
		msh_data->last_pipeline_return = errno;
		executor_cleanup(msh_data, command_io, p_id_arr, p_exit_codes);
		return (msh_perror(NULL, MALLOC_ERR, extern_err), errno);
	}
	msh_data->last_pipeline_return = 0;
	err = execute_pipeline(msh_data, p_id_arr, p_exit_codes, command_io);
	executor_cleanup(msh_data, command_io, p_id_arr, p_exit_codes);
	return (err);
}
