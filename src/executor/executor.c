/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   executor.c                                        :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/06/03 15:24:57 by avaliull     #+#    #+#                  */
/*   Updated: 2025/06/09 19:17:50 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "executor.h"
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

// TEST - function with test_ in name will be either deleted or modified later
void	test_free_exec_data(t_exec_data	*exec_data)
{
	if (exec_data->argv)
		free_2d_arr((void **) exec_data->argv);
	if (exec_data->in_filename)
		free(exec_data->in_filename);
	if (exec_data->out_filename)
		free(exec_data->out_filename);
}

int	test_dummy_builtin(
	const t_exec_data *exec_data
)
{
	printf("Dummy built-in executed: %s\n", exec_data->argv[0]);
	return (0);
}

t_exec_data	*test_get_dummy_exec_data(void)
{
	t_exec_data	*exec_data;

	int i = 0;
	int len = 5;
	exec_data = ft_calloc(len, sizeof(t_exec_data));
	exec_data[i].len = len;
	exec_data[i].argv = ft_calloc(exec_data[i].len + 1, sizeof(char *));
	exec_data[i].argv[0] = ft_strdup("cat");
	//exec_data[i].argv[1] = ft_strdup("-l");
	exec_data[i].argv[1] = NULL;
	exec_data[i].argv[2] = NULL;
	exec_data[i].is_builtin = false;
	exec_data[i].input_type = custom_fd;
	//exec_data[i].input_type = heredoc;
	exec_data[i].heredoc_delim = ft_strdup("EOF");
	exec_data[i].output_type = pipe_write;
	exec_data[i].in_filename = ft_strdup("infile");
	i++;

	exec_data[i].len = len;
	exec_data[i].argv = ft_calloc(exec_data[i].len + 1, sizeof(char *));
	exec_data[i].argv[0] = ft_strdup("cat");
	exec_data[i].argv[1] = ft_strdup("-e");
	exec_data[i].argv[2] = NULL;
	exec_data[i].is_builtin = false;
	exec_data[i].input_type = pipe_read;
	exec_data[i].output_type = pipe_write;
	exec_data[i].redirect_type = append;
	exec_data[i].heredoc_delim = NULL;
	exec_data[i].out_filename = ft_strdup("midfile");
	i++;

	exec_data[i].len = len;
	exec_data[i].argv = ft_calloc(exec_data[i].len + 1, sizeof(char *));
	exec_data[i].argv[0] = ft_strdup("cat");
	exec_data[i].argv[1] = ft_strdup("-T");
	exec_data[i].argv[2] = NULL;
	exec_data[i].is_builtin = false;
	exec_data[i].input_type = pipe_read;
	exec_data[i].output_type = pipe_write;
	exec_data[i].redirect_type = append;
	exec_data[i].heredoc_delim = NULL;
	exec_data[i].out_filename = ft_strdup("midfile");
	i++;

	exec_data[i].len = len;
	exec_data[i].argv = ft_calloc(exec_data[i].len + 1, sizeof(char *));
	exec_data[i].argv[0] = ft_strdup("cat");
	exec_data[i].argv[1] = ft_strdup("-b");
	exec_data[i].argv[2] = NULL;
	exec_data[i].is_builtin = false;
	exec_data[i].input_type = pipe_read;
	exec_data[i].output_type = custom_fd;
	exec_data[i].redirect_type = append;
	exec_data[i].heredoc_delim = NULL;
	exec_data[i].out_filename = ft_strdup("outfile");
	i++;

	//exec_data[i].len = len;
	//exec_data[i].argv = ft_calloc(exec_data[i].len + 1, sizeof(char *));
	//exec_data[i].argv[0] = ft_strdup("cat");
	//exec_data[i].argv[1] = ft_strdup("-b");
	//exec_data[i].argv[2] = NULL;
	//exec_data[i].is_builtin = true;
	//exec_data[i].input_type = std_in;
	//exec_data[i].output_type = std_out;
	//exec_data[i].redirect_type = trunc;
	//exec_data[i].heredoc_delim = NULL;
	//exec_data[i].out_filename = ft_strdup("outfile");
	//i++;

	return (exec_data);
}

// CHILDREN

static int	cleanup_in_parent_process(
	const t_exec_data *command,
	t_command_io *const command_io
)
{
	if (command->input_type != std_in)
		close(command_io->in_fd);
	else if (command->input_type == heredoc || command->input_type == pipe_read)
	{
		close(command_io->in_pipe[READ_END]);
		close(command_io->in_pipe[WRITE_END]);
	}
	if (command->output_type != std_out && command->output_type != std_err)
		close(command_io->out_fd);
	test_free_exec_data((t_exec_data *) command);
	return (0);
}

static int	run_child_process(
	const t_exec_data *command,
	t_command_io *const command_io,
	const char **path
)
{
	int	err_check;

	if (command->input_type != std_in)
	{
		dup2(command_io->in_fd, STDIN_FILENO);
		close(command_io->in_fd);
	}
	if (command->output_type != std_out)
	{
		dup2(command_io->out_fd, STDOUT_FILENO);
		close(command_io->out_fd);
	}
	if (command->output_type == pipe_write)
		close(command_io->out_pipe[READ_END]);

	if (command->is_builtin == false)
		try_execve(path, command->argv);
	err_check = test_dummy_builtin(command);
	exit(err_check);
}

int	execute_command(
	const t_exec_data *command,
	t_command_io *const command_io,
	const char **path
)
{
	int		err_check;
	pid_t	process_id;

	err_check = 0;
	process_id = fork();
	if (process_id == 0)
		run_child_process(command, command_io, path);
	else if (process_id > 0)
		cleanup_in_parent_process(command, command_io);
	else
		perror_and_return(FORK_ERR, LIBFUNC_ERR);
	return (0);
}

int	executor(
	const t_exec_data *exec_data,
	int dummy_minishell_struct
)
{
	int				i;
	int				process_status;
	t_command_io	command_io;
	const char		**path = find_env_path();

	i = -1;
	while (++i < exec_data->len)
	{
		prepare_command_io(&exec_data[i], &command_io);
		if (execute_command(&exec_data[i], &command_io, path) != 0)
		{
			// ADD ERROR MANAGEMENT!!
			printf("PLACEHOLDER, THIS SHOULD ERROR\n");
		}
	}
	while (waitpid(-1, &process_status, 0) > 0) // check if there's more exit signals we need to handle here
	{
		if (WIFEXITED(process_status))
			if (WEXITSTATUS(process_status) != 0)
			{
				// ADD ERROR MANAGEMENT!!
				printf("PLACEHOLDER, THIS SHOULD ERROR\n");
				perror("Child process error\n");
			}
	}
	return (1);
}
