/* ************************************************************************** */
/*                                                                            */
/*                                                            ::::::::        */
/*   executor_pipeline.c                                     :+:    :+:       */
/*                                                          +:+               */
/*   By: avaliull <avaliull@student.codam.nl>              +#+                */
/*                                                        +#+                 */
/*   Created: 2025/12/16 17:49:01 by avaliull            #+#    #+#           */
/*   Updated: 2025/12/16 18:01:46 by avaliull            ########   odam.nl   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"
#include "builtins.h"
#include <unistd.h>

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
		err_check = do_redirections(command->redirections, NULL, false);
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
		handle_signals_child_process();
		msh_data->is_parent = false;
		err_check = run_child_process(command, command_io, msh_data);
	}
	else if (*pid > 0)
	{
		if (command->output_is_pipe == true)
			safe_close(&command_io->out_pipe[WRITE_END]);
		free_and_close_exec_data(command);
	}
	else if (*pid < 0)
		return (msh_perror(NULL, FORK_ERR, extern_err), fork_err);
	return (err_check);
}

static int	execute_command(
	t_exec_data *command,
	t_command_io *const command_io,
	t_msh_data *const msh_data,
	pid_t *pid
)
{
	int				err;
	t_undup_list	**undup_list_head;
	t_undup_list	*undup_list;

	err = success;
	*pid = 0;
	if (command->input_is_pipe == true || command->output_is_pipe == true
		|| command->builtin_name == not_builtin)
		return (execute_in_child(command, command_io, msh_data, pid));
	if (command->redirections)
	{
		undup_list = NULL;
		undup_list_head = &undup_list;
		err = do_redirections(command->redirections, undup_list_head, true);
	}
	if (command->builtin_name != not_builtin && err == success)
		err = exec_builtin(command, msh_data);
	if (command->redirections)
		undup_redirections(undup_list_head);
	return (err);
}

// this functiohn will return either the amount of commands in the pipeline,
// or an error code.
// if a pipe creation fails, it will return the number of pipes built.
// this will tell the caller function that not all pipes were created.
// currently, what happens on fail is unclear
int	build_pipeline(
	t_msh_data *const msh_data,
	t_command_io *command_io,
	int *elem_count
)
{
	t_exec_data *const	exec_data = msh_data->exec_data;
	const int			command_count = msh_data->command_count;
	int	status_check;
	int	i;

	i = -1;
	status_check = success;
	while (++i < command_count)
	{
		status_check = prepare_command_io(msh_data,
									&exec_data[i], command_io, i);
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
